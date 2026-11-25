#!/usr/bin/env python3
"""Synthetic-only tests for the host portable-source check."""
from __future__ import annotations

import sys
import tempfile
import unittest
import unittest.mock
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstport as H
import hstpublish

CLANG = H.DEFAULT_NDK / H.NDK_CLANG_RELATIVE
HOST_CLANG = H.DEFAULT_HOST_CLANG if H.DEFAULT_HOST_CLANG.is_file() else CLANG


def make_root(tmp: str) -> Path:
    root = Path(tmp)
    (root / "src" / "eboot").mkdir(parents=True)
    (root / "src" / "sharing_main").mkdir(parents=True)
    (root / "include").mkdir()
    (root / "data").mkdir()
    (root / "data" / "functions-EBOOT.BIN.txt").write_text(
        "# synthetic function ranges\n", encoding="utf-8")
    (root / "data" / "functions-hstp_trans_main.prx.txt").write_text(
        "# synthetic function ranges\n", encoding="utf-8")
    (root / "data" / "modules.json").write_text(
        '{"modules":[{"name":"EBOOT.BIN","load_segments":[{"start":4096,"memory_end":12288}]},'
        '{"name":"hstp_trans_main.prx","load_segments":[{"start":4096,"memory_end":12288}]}]}\n',
        encoding="utf-8")
    return root


def write_source(root: Path, relative: str, module: str, start: int, end: int, code: str,
                 lang: str = "c99", cc: str | None = None) -> Path:
    path = root / relative
    path.parent.mkdir(parents=True, exist_ok=True)
    cc_field = f" cc={cc}" if cc else ""
    path.write_text(
        f"/* HSTMATCH module={module} start=0x{start:08x} end=0x{end:08x} "
        f"symbol=synthetic lang={lang}{cc_field} */\n{code}\n",
        encoding="utf-8",
    )
    return path


def write_ranges(root: Path, module: str, ranges: list[tuple[int, int]]) -> None:
    path = root / "data" / f"functions-{module}.txt"
    lines = ["# synthetic function ranges"]
    lines += [f"{start:08x} {end:08x}" for start, end in ranges]
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


@unittest.skipUnless(CLANG.is_file(), f"Android NDK Clang not found: {CLANG}")
class PortableSourceTests(unittest.TestCase):
    def run_check(self, root: Path, targets=H.TARGETS) -> dict:
        return H.run_check(root=root, targets=targets, parallel=2, host_clang=HOST_CLANG, android_ndk=CLANG)

    def test_clean_c_source_passes_all_five_targets(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            write_source(root, "src/eboot/clean.c", "EBOOT.BIN", 0x1000, 0x1008,
                         "#include <stdint.h>\n#include <stddef.h>\n#include <stdarg.h>\n"
                         "#include <limits.h>\n#include <float.h>\n#include <stdio.h>\n"
                         "#include <stdlib.h>\n#include <string.h>\n#include <varargs.h>\n"
                         "uintptr_t get_address(void *p) { return (uintptr_t)p; }\n"
                         "void *from_uintptr(uintptr_t address) { return (void *)address; }\n"
                         "int *null_pointer(void) { return (int *)0; }\n"
                         "void *null_macro(void) { return NULL; }")
            write_ranges(root, "EBOOT.BIN", [(0x1000, 0x1008)])
            result = self.run_check(root)
            for view in ("public", "private"):
                self.assertEqual(result[view]["sources"], 1)
                self.assertEqual(result[view]["compiling_all_targets"], 1)
                self.assertEqual(result[view]["code_bytes"], 8)
                self.assertEqual(result[view]["compiling_bytes"], 8)
                self.assertTrue(all(count == {"ok": 1, "fail": 0}
                                    for count in result[view]["per_target"].values()))
                self.assertEqual(result[view]["numeric_address_findings"], {"src/eboot/clean.c": 0})

    def test_literal_folded_and_enum_addresses_fail_every_target(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            write_source(root, "src/eboot/numeric.c", "EBOOT.BIN", 0x1000, 0x1020,
                         "enum { MODULE_ADDRESS = 0x08800000 };\n"
                         "int *literal(void) { return (int *)0x08800000; }\n"
                         "int *folded(void) { return (int *)(0x08000000 + 0x10); }\n"
                         "int *enumerated(void) { return (int *)MODULE_ADDRESS; }\n"
                         "int *implicit(void) { return 0x08800000; }")
            write_ranges(root, "EBOOT.BIN", [(0x1000, 0x1020)])
            result = self.run_check(root)["private"]
            self.assertEqual(result["numeric_address_findings"], {"src/eboot/numeric.c": 4})
            self.assertEqual(result["census"], {"numeric-address": 1})
            self.assertTrue(all(count == {"ok": 0, "fail": 1}
                                for count in result["per_target"].values()))
            failure = result["failures"][0]
            self.assertEqual(failure["reason"], "numeric-address")
            self.assertEqual(failure["diagnostic"]["category"], "numeric-address")

    def test_cpp_reinterpret_cast_literal_fails_every_target(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            write_source(root, "src/eboot/numeric.cpp", "EBOOT.BIN", 0x1000, 0x1008,
                         "int *literal() { return reinterpret_cast<int *>(0x08800000); }", lang="c++")
            write_ranges(root, "EBOOT.BIN", [(0x1000, 0x1008)])
            result = self.run_check(root)["private"]
            self.assertEqual(result["numeric_address_findings"], {"src/eboot/numeric.cpp": 1})
            self.assertEqual(result["failures"][0]["reason"], "numeric-address")
            self.assertTrue(all(count == {"ok": 0, "fail": 1}
                                for count in result["per_target"].values()))

    def test_integer_to_pointer_cast_fails_only_on_64_bit_targets(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            write_source(root, "src/eboot/cast.c", "EBOOT.BIN", 0x1000, 0x1008,
                         "void *from_integer(int value) { return (void *)value; }")
            write_ranges(root, "EBOOT.BIN", [(0x1000, 0x1008)])
            result = self.run_check(root)["private"]
            for target in (H.TARGETS[0], H.TARGETS[2], H.TARGETS[4]):
                self.assertEqual(result["per_target"][target], {"ok": 0, "fail": 1})
            for target in (H.TARGETS[1], H.TARGETS[3]):
                self.assertEqual(result["per_target"][target], {"ok": 1, "fail": 0})
            failure = result["failures"][0]
            self.assertEqual(failure["target"], H.TARGETS[0])
            self.assertEqual(failure["diagnostic"]["category"], "int-to-pointer-cast")
            self.assertIn("int-to-void-pointer-cast", failure["diagnostic"]["flag"])

    def test_host_intptr_width_exposes_a_32_bit_assumption(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            write_source(root, "src/eboot/intptr.c", "EBOOT.BIN", 0x1000, 0x1008,
                         "#include <stdint.h>\ntypedef char psp_intptr_is_32[(sizeof(intptr_t) == 4) ? 1 : -1];\nint f(void) { return 0; }")
            write_ranges(root, "EBOOT.BIN", [(0x1000, 0x1008)])
            result = self.run_check(root)["private"]
            for target in (H.TARGETS[0], H.TARGETS[2], H.TARGETS[4]):
                self.assertEqual(result["per_target"][target], {"ok": 0, "fail": 1})
            for target in (H.TARGETS[1], H.TARGETS[3]):
                self.assertEqual(result["per_target"][target], {"ok": 1, "fail": 0})

    def test_inline_mips_assembly_is_rejected_with_policy_reason(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            write_source(root, "src/eboot/mips_asm.c", "EBOOT.BIN", 0x1000, 0x1008,
                         'void return_to_ra(void) { __asm__ volatile ("jr $ra"); }')
            write_ranges(root, "EBOOT.BIN", [(0x1000, 0x1008)])
            result = self.run_check(root)["private"]
            self.assertEqual(result["per_target"][H.TARGETS[4]], {"ok": 0, "fail": 1})
            diagnostic = result["failures"][0]["diagnostic"]
            self.assertEqual(diagnostic["category"], "mips-inline-asm")
            self.assertEqual(diagnostic["flag"], "policy:mips-inline-asm")
            self.assertEqual(diagnostic["location"], "src/eboot/mips_asm.c:2")

    def test_cxx17_source_compiles_for_all_targets(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            write_source(root, "src/eboot/clean.cpp", "EBOOT.BIN", 0x1000, 0x1008,
                         "class Sample { public: int increment(int value) const { return value + 1; } };\n"
                         "extern \"C\" int use_sample(int value) { Sample sample; return sample.increment(value); }",
                         lang="c++")
            write_ranges(root, "EBOOT.BIN", [(0x1000, 0x1008)])
            result = self.run_check(root)["private"]
            self.assertEqual(result["compiling_all_targets"], 1)
            self.assertIn("-std=c++17", result["flags"][H.TARGETS[0]]["c++"])

    def test_public_private_byte_weighting_uses_excluded_origin_classes(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = make_root(tmp)
            write_source(root, "src/eboot/public.c", "EBOOT.BIN", 0x1000, 0x1004,
                         "int public_function(void) { return 1; }")
            write_source(root, "src/eboot/excluded.c", "EBOOT.BIN", 0x1004, 0x100C,
                         "int excluded_function(void) { return 2; }", cc="gcc171-O2")
            write_ranges(root, "EBOOT.BIN", [(0x1000, 0x1004), (0x1004, 0x100C)])
            result = self.run_check(root)
            public = result["public"]
            private = result["private"]
            self.assertIn("gcc-unattributed", hstpublish.EXCLUDED_CLASSES)
            self.assertEqual((public["sources"], public["code_bytes"], public["compiling_bytes"]), (1, 4, 4))
            self.assertEqual((private["sources"], private["code_bytes"], private["compiling_bytes"]), (2, 12, 12))
            self.assertEqual(public["compiling_all_targets"], 1)
            self.assertEqual(private["compiling_all_targets"], 2)


def port_report(sources: int, compiling: int, findings: dict[str, int]) -> dict:
    return {"all_five_targets": True, "generated_at": "2000-02-03T04:05:06+00:00", "elapsed_seconds": 75.5,
            "numeric_address_scan_seconds": 12.25, "sources": sources, "compiling_all_targets": compiling,
            "code_bytes": 1000, "compiling_bytes": 250, "targets": list(H.TARGETS),
            "per_target": {t: {"ok": compiling, "fail": sources - compiling} for t in H.TARGETS},
            "numeric_address_findings": findings}


class PortingDocTests(unittest.TestCase):
    def test_measurement_block_is_rendered_from_the_reports(self) -> None:
        text = H.render_measurement(port_report(10, 4, {"a.c": 3, "b.c": 0, "c.c": 2}), port_report(12, 5, {}))
        self.assertTrue(text.startswith("The last full run, on 2000-02-03, analyzed and measured all 12 sources for all "
                                        "five targets and took 75.500 seconds (1 minute 15.500 seconds), including "
                                        "12.250 seconds for the AST numeric-address scan. The detector reported 5 "
                                        "findings across 2 published sources."))
        self.assertIn("| Public | 10 | 4 | 1,000 | 250 | 25.000% |", text)
        self.assertIn("| `x86_64-pc-windows-msvc` | 4 / 6 | 5 / 7 |", text)

    def test_a_target_subset_cannot_write_the_measurement(self) -> None:
        subset = dict(port_report(10, 4, {}), all_five_targets=False)
        with self.assertRaises(ValueError):
            H.render_measurement(subset, port_report(12, 5, {}))

    def test_write_doc_with_a_target_subset_is_refused_before_anything_is_measured(self) -> None:
        calls = []
        with tempfile.TemporaryDirectory() as tmp, \
                unittest.mock.patch.object(H, "run_check", lambda **kwargs: calls.append(kwargs)), \
                unittest.mock.patch.object(H, "write_report"), unittest.mock.patch("sys.stderr"):
            try:
                H.main(["--root", tmp, "--targets", H.TARGETS[0], "--write-doc"])
            except SystemExit as exc:
                self.assertEqual(exc.code, 2)
            except (TypeError, ValueError):
                pass                                       # main's run went on past the stand-in measurement
        self.assertEqual(calls, [])

    def test_write_doc_replaces_only_the_block_and_needs_one(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "docs").mkdir()
            doc = root / "docs" / "PORTING.md"
            doc.write_text("Before.\n\n<!-- hstport:begin measurement -->\nold\n<!-- hstport:end -->\n\nAfter.\n",
                           encoding="utf-8")
            self.assertTrue(H.write_doc(root, port_report(10, 4, {}), port_report(12, 5, {})))
            text = doc.read_text(encoding="utf-8")
            self.assertTrue(text.startswith("Before.\n\n<!-- hstport:begin measurement -->\n\nThe last full run"))
            self.assertTrue(text.endswith("|\n\n<!-- hstport:end -->\n\nAfter.\n"))
            self.assertFalse(H.write_doc(root, port_report(10, 4, {}), port_report(12, 5, {})))   # idempotent
            doc.write_text("no block\n", encoding="utf-8")
            with self.assertRaises(SystemExit):
                H.write_doc(root, port_report(10, 4, {}), port_report(12, 5, {}))


if __name__ == "__main__":
    unittest.main()
