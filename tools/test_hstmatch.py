#!/usr/bin/env python3
"""Unit checks of tools/hstmatch.py's same-file helper support (helpers=) that need no toolchain: the header syntax,
the section rename of the placed object copy, and the function-to-section mapping. The end-to-end placement (compile,
link, compare, promotion, refusal of a mismatching helper) is in tools/test_hstpromote.py (HelperPlacement).

    python tools/test_hstmatch.py [-v]
"""
from __future__ import annotations

import json
import os
import shlex
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstmatch as M  # noqa: E402


def tiny_object(funcs: list[tuple[str, bytes]], extra_in_first: str | None = None, undefined: tuple[str, ...] = (),
                base: int = 0) -> bytes:
    """A little-endian ELF32 relocatable object shaped like mwccpsp output: one `.text` section per function, a symtab
    with one FUNC symbol at offset `base` of each (and optionally a second FUNC symbol in the first section, and
    undefined names)."""
    names = b"\0.text\0.symtab\0.strtab\0.shstrtab\0"
    n_text, n_sym, n_str, n_shs = 1, 7, 15, 23
    strtab = b"\0"
    syms = [struct.pack("<IIIBBH", 0, 0, 0, 0, 0, 0)]
    blobs, sections = [], [(0, 0, 0, 0, 0, 0, 0, 0, 0, 0)]
    off = 52
    for i, (name, code) in enumerate(funcs):
        blobs.append(code)
        sections.append((n_text, 1, 6, 0, off, len(code), 0, 0, 4, 0))
        off += len(code)
        syms.append(struct.pack("<IIIBBH", len(strtab), base, len(code), 0x12, 0, i + 1))
        strtab += name.encode() + b"\0"
    if extra_in_first:
        syms.append(struct.pack("<IIIBBH", len(strtab), 4, 4, 0x12, 0, 1))
        strtab += extra_in_first.encode() + b"\0"
    for name in undefined:
        syms.append(struct.pack("<IIIBBH", len(strtab), 0, 0, 0x10, 0, 0))
        strtab += name.encode() + b"\0"
    symdata = b"".join(syms)
    for data, sec in ((symdata, (n_sym, 2, 0, 0, 0, len(symdata), len(funcs) + 2, 1, 4, 16)),
                      (strtab, (n_str, 3, 0, 0, 0, len(strtab), 0, 0, 1, 0)),
                      (names, (n_shs, 3, 0, 0, 0, len(names), 0, 0, 1, 0))):
        blobs.append(data)
        sections.append(sec[:4] + (off,) + sec[5:])
        off += len(data)
    off += -off % 4
    body = b"".join(blobs)
    body += b"\0" * (off - 52 - len(body))
    ehdr = struct.pack("<16sHHIIIIIHHHHHH", b"\x7fELF\x01\x01\x01" + b"\0" * 9, 1, 8, 1, 0, 0, off, 0, 52, 0, 0, 40,
                       len(sections), len(sections) - 1)
    return ehdr + body + b"".join(struct.pack("<10I", *s) for s in sections)


class BuildKey(unittest.TestCase):
    """Regression: build products were keyed `<folder>__<stem>`, so try/gen-a1/v0/FUN_x.c and try/gen-a2/v0/FUN_x.c
    shared one directory and each could read the other's text.bin."""

    def test_same_folder_and_stem_in_different_trees_get_different_keys(self) -> None:
        a = M.ROOT / "try" / "gen-a1" / "v0" / "FUN_00010000.c"
        b = M.ROOT / "try" / "gen-a2" / "v0" / "FUN_00010000.c"
        self.assertNotEqual(M.build_key(a), M.build_key(b))
        self.assertTrue(M.build_key(a).startswith("v0__FUN_00010000__"))
        with tempfile.TemporaryDirectory() as d:     # outside the checkout: keyed by the absolute path
            c, e = Path(d) / "x" / "v0" / "FUN_00010000.c", Path(d) / "y" / "v0" / "FUN_00010000.c"
            self.assertNotEqual(M.build_key(c), M.build_key(e))
            self.assertNotIn(M.build_key(c), (M.build_key(a), M.build_key(b)))

    def test_key_is_stable_and_independent_of_the_checkout_location(self) -> None:
        p = M.ROOT / "src" / "eboot" / "FUN_00010000.c"
        self.assertEqual(M.build_key(p), M.build_key(Path(os.path.relpath(p))))     # relative spelling, same key
        import hashlib
        rel = os.path.normcase("src/eboot/FUN_00010000.c")
        self.assertEqual(M.build_key(p), "eboot__FUN_00010000__" + hashlib.sha1(rel.encode()).hexdigest()[:10])

    def test_matcher_and_independent_verifier_agree(self) -> None:
        import hstxverify
        with tempfile.TemporaryDirectory() as d:
            for p in (M.ROOT / "src" / "eboot" / "a_00010000.c", M.ROOT / "try" / "gen-a1" / "v0" / "FUN_00010000.cpp",
                      Path(d) / "v0" / "FUN_00010000.c"):
                self.assertEqual(M.build_key(p), hstxverify.build_key(p))

    def test_every_tool_uses_the_shared_key(self) -> None:
        old = "parent.name}__{"
        for f in sorted(M.ROOT.joinpath("tools").glob("hst*.py")):
            if f.name in ("hstmatch.py", "hstxverify.py"):
                continue
            self.assertNotIn(old, f.read_text(encoding="utf-8"), f"{f.name} builds its own key")


class HelperHeader(unittest.TestCase):
    def header(self, extra: str) -> dict:
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "f.c"
            p.write_text(f"/* HSTMATCH module=EBOOT.BIN start=0x00291434 end=0x002914c0 symbol=FUN_00291434 "
                         f"lang=c99 {extra} */\n", encoding="utf-8")
            return M.parse_header(p)

    def test_parsed_and_ordered_target_first(self) -> None:
        h = self.header("helpers=Log@0x0028fb44-0x0028fb70,FUN_0028fb70@0x0028fb70-0x0028fb80")
        self.assertEqual(h["helpers"], [("Log", 0x28fb44, 0x28fb70), ("FUN_0028fb70", 0x28fb70, 0x28fb80)])
        self.assertEqual(M.placed_functions(h)[0], ("FUN_00291434", 0x291434, 0x2914c0))
        self.assertEqual(M.placed_functions(self.header("")), [("FUN_00291434", 0x291434, 0x2914c0)])

    def test_bad_entries_are_rejected(self) -> None:
        for bad in ("helpers=Log", "helpers=Log@0x0028fb44", "helpers=Log@0x0028fb46-0x0028fb70",
                    "helpers=Log@0x0028fb70-0x0028fb44",
                    "helpers=Log@0x00291430-0x00291440",                                   # overlaps the target
                    "helpers=A@0x0028fb44-0x0028fb70,B@0x0028fb60-0x0028fb80",              # helpers overlap
                    "helpers=FUN_00291434@0x0028fb44-0x0028fb70",                           # the target's own name
                    "helpers=A@0x0028fb44-0x0028fb70,A@0x0028fb70-0x0028fb80"):             # a name twice
            with self.subTest(bad=bad), self.assertRaises(ValueError):
                self.header(bad)


class SectionPlacement(unittest.TestCase):
    def test_rename_changes_only_section_names(self) -> None:
        obj = tiny_object([("Log", b"\x08\x00\xe0\x03" b"\0\0\0\0"), ("Target", b"\x01\x02\x03\x04")])
        out = M.rename_sections(obj, {1: ".hst_fn0", 2: ".hst_fn1"})
        before, after = M.elf_sections(obj), M.elf_sections(out)
        self.assertEqual([n for n, _ in before][1:3], [".text", ".text"])
        self.assertEqual([n for n, _ in after][1:3], [".hst_fn0", ".hst_fn1"])
        self.assertEqual([n for n, _ in after][3:], [n for n, _ in before][3:])
        for (_, a), (_, b) in zip(before, after):
            if a[1] != 3:   # every section but the string tables: same header fields apart from the name
                self.assertEqual(a[1:], b[1:])
                self.assertEqual(obj[a[4]:a[4] + a[5]], out[b[4]:b[4] + b[5]])
        self.assertEqual(list(M.elf_symbols(obj)), list(M.elf_symbols(out)))
        shoff = struct.unpack_from("<I", obj, 0x20)[0]
        self.assertEqual(out[:shoff], obj[:shoff])   # every byte before the section headers is untouched (the new name
                                                     # table is appended after them)

    def test_function_sections(self) -> None:
        obj = tiny_object([("Log", b"\0" * 8), ("Target", b"\0" * 4)])
        got, why = M.function_sections(obj, [("Target", 0x100, 0x104), ("Log", 0x40, 0x48)])
        self.assertEqual((got, why), ({2: ("Target", 0x100, 0x104), 1: ("Log", 0x40, 0x48)}, ""))

    def test_function_sections_refusals(self) -> None:
        obj = tiny_object([("Log", b"\0" * 8), ("Target", b"\0" * 4)])
        _, why = M.function_sections(obj, [("Target", 0x100, 0x104)])
        self.assertIn("Log defined but not placed", why)                 # every function must be placed
        _, why = M.function_sections(obj, [("Target", 0x100, 0x104), ("Missing", 0x40, 0x48)])
        self.assertIn("Missing is not defined", why)
        shared = tiny_object([("Log", b"\0" * 8), ("Target", b"\0" * 4)], extra_in_first="Other")
        _, why = M.function_sections(shared, [("Target", 0x100, 0x104), ("Log", 0x40, 0x48)])
        self.assertIn("shares its .text section with Other", why)        # one function per section only

    def test_word_diffs(self) -> None:
        self.assertEqual(M.word_diffs(b"\0" * 8, b"\0" * 8), [])
        self.assertEqual(M.word_diffs(b"\0" * 8, b"\0\0\0\0\1\0\0\0"), [1])
        self.assertEqual(M.word_diffs(b"\0" * 4, b"\0" * 8), [1])          # a shorter body differs in the rest


class WSLRunner(unittest.TestCase):
    def test_run_wsl_cleanup_on_subprocess_failure(self) -> None:
        # run_wsl writes ROOT/build/<name>.<pid>.sh; ROOT points at a temp dir so the tree is not touched.
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            build_dir = root / "build"
            build_dir.mkdir()
            script_path = build_dir / f"test_runner.{os.getpid()}.sh"
            seen = []

            def failing_run(*args, **kwargs):
                seen.append(script_path.exists())
                raise RuntimeError("wsl execution failed")

            with patch.object(M, "ROOT", root), patch("subprocess.run", side_effect=failing_run) as mock_run:
                with self.assertRaises(RuntimeError):
                    M.run_wsl(["echo test"], "test_runner")

            mock_run.assert_called_once()
            self.assertEqual(seen, [True])                                      # the script existed during the run
            self.assertFalse(script_path.exists(), "run_wsl must unlink its script even if subprocess.run raises")


class CompilePhaseFailure(unittest.TestCase):
    def test_compile_phase_failure_reports_compile_failed(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            tmp = Path(d)
            src = tmp / "src" / "test.c"
            src.parent.mkdir(parents=True)
            src.write_text("/* HSTMATCH module=EBOOT.BIN start=0x00100000 end=0x00100010 symbol=FUN_00100000 lang=c99 */\nvoid FUN_00100000(void) { syntax_error }\n", encoding="utf-8")

            build_root = tmp / "build"
            json_out = tmp / "results.json"

            def fake_run_wsl(lines, name, native=None, **_kw):
                if name == "compile":
                    out_dir = build_root / "EBOOT.BIN" / M.build_key(src)
                    out_dir.mkdir(parents=True, exist_ok=True)
                    (out_dir / "compile.log").write_text("error: 'syntax_error' undeclared\n", encoding="utf-8")

            with patch.dict(os.environ, {"HSTMATCH_BUILD_ROOT": str(build_root), "HST_MWCC_NATIVE": "/fake/native"}), \
                 patch.object(M, "ROOT", tmp), \
                 patch.object(M, "run_wsl", side_effect=fake_run_wsl), \
                 patch.object(sys, "argv", ["hstmatch.py", "--quiet", str(src), "--json", str(json_out)]):
                ret = M.main()

            self.assertEqual(ret, 1)
            self.assertTrue(json_out.exists())
            data = json.loads(json_out.read_text(encoding="utf-8"))
            self.assertEqual(len(data), 1)
            self.assertEqual(data[0]["status"], "COMPILE_FAILED")
            self.assertIn("syntax_error", data[0]["detail"])

    def test_jobfile_cleanup_on_compile_failure(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            tmp = Path(d)
            src = tmp / "test.c"
            src.write_text("/* HSTMATCH module=EBOOT.BIN start=0x00100000 end=0x00100010 symbol=FUN_00100000 lang=c99 */\nint x;\n", encoding="utf-8")

            build_root = tmp / "build"                                          # also ROOT/build: ROOT is tmp
            jobfile_existed = False
            jobfile_path = tmp / "build" / f"compile.{os.getpid()}.jobs"

            def fake_run_wsl(lines, name, native=None, **_kw):
                nonlocal jobfile_existed
                if name == "compile":
                    jobfile_existed = jobfile_path.exists()
                    raise RuntimeError("WSL compile process failed")

            with patch.dict(os.environ, {"HSTMATCH_BUILD_ROOT": str(build_root), "HST_MWCC_NATIVE": "/fake/native"}), \
                 patch.object(M, "ROOT", tmp), \
                 patch.object(M, "run_wsl", side_effect=fake_run_wsl), \
                 patch.object(sys, "argv", ["hstmatch.py", "--quiet", str(src)]):
                with self.assertRaises(RuntimeError):
                    M.main()

            self.assertTrue(jobfile_existed)
            self.assertFalse(jobfile_path.exists())


CODE = b"\x08\x00\xe0\x03"      # jr ra: the whole 4-byte target of the end-to-end fixture below
TARGET_HEADER = "/* HSTMATCH module=EBOOT.BIN start=0x00100000 end=0x00100004 symbol=FUN_00100000 lang=c99 */\n"


def run_matcher(tmp: Path, obj: bytes, argv_extra: tuple[str, ...] = (), build_name: str = "build",
                symbols_txt: str | None = None) -> tuple[int, list[dict], list]:
    """hstmatch.main() over one source, with the WSL steps faked: compile writes `obj`, link writes a linked ELF whose
    .text is CODE at the retail address, and the retail bytes are CODE. Returns (exit code, JSON rows, run_wsl calls)."""
    src = tmp / "src" / "FUN_00100000.c"
    src.parent.mkdir(parents=True, exist_ok=True)
    src.write_text(TARGET_HEADER + "void FUN_00100000(void) {}\n", encoding="utf-8")
    if symbols_txt is not None:
        (tmp / "symbols").mkdir(exist_ok=True)
        (tmp / "symbols" / "EBOOT.BIN.txt").write_text(symbols_txt, encoding="utf-8")
    build_root = tmp / build_name
    (tmp / "build").mkdir(exist_ok=True)       # ROOT/build holds the compile job file
    calls = []

    def fake_run_wsl(lines, name, native=None, **kw):
        calls.append((name, list(lines), kw))
        out = build_root / "EBOOT.BIN" / M.build_key(src)
        if name == "compile":
            (out / "compile.log").write_text("", encoding="utf-8")
            (out / "obj.o").write_bytes(obj)
        elif name == "link":
            (out / "linked.elf").write_bytes(tiny_object([("FUN_00100000", CODE)], base=0x100000))
            (out / "text.bin").write_bytes(CODE)

    env = {"HSTMATCH_BUILD_ROOT": str(build_root), "HST_MWCC_NATIVE": "/fake/native", "HST_PSPDEV_BIN": "/fake/pspdev"}
    with patch.dict(os.environ, env), patch.object(M, "ROOT", tmp), patch.object(M, "EXTRA_SYMBOLS", []), \
            patch.object(M, "run_wsl", side_effect=fake_run_wsl), \
            patch.object(M, "module_bytes", return_value=(CODE, {"file_offset": 0, "start": 0x100000})), \
            patch.object(sys, "argv", ["hstmatch.py", "--quiet", str(src), "--json", str(tmp / "r.json"), *argv_extra]):
        rc = M.main()
    return rc, json.loads((tmp / "r.json").read_text(encoding="utf-8")), calls


class HeaderValidation(unittest.TestCase):
    """Audit 2026-11-17: header values reached the `sh -c` compile line (lang=, extra=) and the build path and link
    lines (module=) unchecked, so a header could run a command in WSL or write outside build/."""

    def header(self, fields: str) -> dict:
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "f.c"
            p.write_text(f"/* HSTMATCH start=0x00100000 end=0x00100010 symbol=FUN_00100000 {fields} */\n",
                         encoding="utf-8")
            return M.parse_header(p)

    def test_values_that_could_run_a_command_or_leave_build_are_refused(self) -> None:
        for bad in ("module=EBOOT.BIN lang=c99;touch${IFS}x;", "module=EBOOT.BIN lang=pascal",
                    'module=EBOOT.BIN extra="-O2;touch x"', 'module=EBOOT.BIN extra="$(id)"',
                    'module=EBOOT.BIN extra="-o/tmp/x"', 'module=EBOOT.BIN extra="obj.o"',
                    "module=EBOOT.BIN extra=-O2`id`", "module=../../x", "module=..", "module=EBOOT.BIN;id",
                    "module=$(id)", "module=C:/x"):
            with self.subTest(bad=bad), self.assertRaises(ValueError):
                self.header(bad)

    def test_the_values_in_use_are_accepted(self) -> None:
        for module in ("EBOOT.BIN", "hstp_trans_main.prx", "scePsmfP_library.prx", "libfont.prx"):
            self.assertEqual(self.header(f"module={module}")["module"], module)
        for lang in M.LANGS:
            self.assertEqual(self.header(f"module=EBOOT.BIN lang={lang}")["lang"], lang)
        h = self.header('module=EBOOT.BIN extra="-fno-omit-frame-pointer -mext"')
        self.assertEqual(M.extra_args(h), "-fno-omit-frame-pointer -mext")
        self.assertEqual(M.extra_args(self.header("module=EBOOT.BIN")), "")

    def test_every_path_on_the_link_lines_is_quoted(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            tmp = Path(d)
            # a build root whose name holds shell syntax (legal in a Windows directory name)
            _rc, rows, calls = run_matcher(tmp, tiny_object([("FUN_00100000", CODE)]), build_name="b $(touch x);y")
            self.assertEqual(rows[0]["status"], "EXACT")
            link = [lines for name, lines, _kw in calls if name == "link"][0]
            out = next((tmp / "b $(touch x);y" / "EBOOT.BIN").iterdir())      # the source's build directory
            for f in ("linked.elf", "syms.ld", "link.log", "text.bin", "obj.o"):
                self.assertIn(shlex.quote(M.to_wsl(out / f)), link[0], f)


class SymbolMap(unittest.TestCase):
    """Audit 2026-11-17: the map could bind FUN_/DAT_xxxxxxxx to another address, and an EXACT that needed
    --symbols-extra entries was indistinguishable from one on committed symbols."""

    def test_an_address_bearing_name_bound_elsewhere_is_refused(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            (root / "symbols").mkdir()
            committed = root / "symbols" / "EBOOT.BIN.txt"
            extra = root / "extra.txt"
            with patch.object(M, "ROOT", root), patch.object(M, "EXTRA_SYMBOLS", []):
                committed.write_text("FUN_00012340 = 0x00012340;\nfoo = 0x00056780;\n", encoding="utf-8")
                self.assertEqual(M.load_symbols("EBOOT.BIN"), {"FUN_00012340": 0x12340, "foo": 0x56780})
                committed.write_text("FUN_00012340 = 0x00056780;\n", encoding="utf-8")
                with self.assertRaisesRegex(ValueError, "contradicts the address its name carries"):
                    M.load_symbols("EBOOT.BIN")
                committed.write_text("", encoding="utf-8")
                extra.write_text("DAT_00012340 = 0x00056780;\n", encoding="utf-8")
                M.EXTRA_SYMBOLS.append(extra)
                with self.assertRaisesRegex(ValueError, "contradicts the address its name carries"):
                    M.load_symbols("EBOOT.BIN")
                self.assertEqual(M.load_symbols("EBOOT.BIN", include_extra=False), {})

    def test_an_exact_that_needs_uncommitted_entries_says_so(self) -> None:
        obj = tiny_object([("FUN_00100000", CODE)], undefined=("foo",))
        with tempfile.TemporaryDirectory() as d:
            tmp = Path(d)
            (tmp / "extra.txt").write_text("foo = 0x00001234;\n", encoding="utf-8")
            _rc, rows, _calls = run_matcher(tmp, obj, ("--symbols-extra", str(tmp / "extra.txt")), symbols_txt="")
            self.assertEqual(rows[0]["status"], "EXACT")
            self.assertEqual(rows[0]["extra_symbols"], ["foo"])
            self.assertIn("uncommitted --symbols-extra entries: foo", rows[0]["detail"])
        with tempfile.TemporaryDirectory() as d:
            tmp = Path(d)      # the same name committed: a plain EXACT
            _rc, rows, _calls = run_matcher(tmp, obj, symbols_txt="foo = 0x00001234;\n")
            self.assertEqual((rows[0]["status"], rows[0]["detail"], rows[0]["extra_symbols"]), ("EXACT", "", []))


class StepTimeout(unittest.TestCase):
    """Audit 2026-11-17: the compile and link steps ran with no time limit while the build-directory locks were held,
    so a hung wibo/mwccpsp or WSL instance blocked this run and every run on the same sources forever."""

    def test_a_step_that_does_not_return_is_a_tool_failure(self) -> None:
        def hung(*args, **kwargs):
            if kwargs.get("timeout") is None:
                raise AssertionError("subprocess.run without a timeout: a hung step would block forever")
            raise subprocess.TimeoutExpired(args[0], kwargs["timeout"])

        with tempfile.TemporaryDirectory() as d, patch.object(M, "ROOT", Path(d)), \
                patch("subprocess.run", side_effect=hung) as run:
            with self.assertRaisesRegex(M.ToolFailure, "did not finish within 240s"):
                M.run_wsl(["true"], "compile", timeout=240)
            argv = run.call_args[0][0]
            self.assertEqual(argv[:6], ["wsl.exe", "-e", "timeout", "-k", "10", "240"])  # bounded in WSL too
            self.assertEqual(list((Path(d) / "build").glob("*.sh")), [], "the script must be removed")

    def test_a_step_stopped_by_the_linux_side_limit_is_a_tool_failure(self) -> None:
        done = subprocess.CompletedProcess([], 124, stdout=b"")
        with tempfile.TemporaryDirectory() as d, patch.object(M, "ROOT", Path(d)), \
                patch("subprocess.run", return_value=done), patch.object(M.time, "monotonic", side_effect=[0.0, 241.0]):
            with self.assertRaises(M.ToolFailure):
                M.run_wsl(["true"], "link", timeout=240)

    def test_both_steps_of_a_run_are_bounded(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            _rc, _rows, calls = run_matcher(Path(d), tiny_object([("FUN_00100000", CODE)]))
            self.assertEqual([(name, kw.get("timeout")) for name, _lines, kw in calls],
                             [("compile", M.step_timeout(1, 4)), ("link", M.step_timeout(1))])
        self.assertEqual(M.step_timeout(9, 4), M.STEP_TIMEOUT_SECONDS * 4)


class TargetReferences(unittest.TestCase):
    """Audit 2026-11-17: for a multi-function run (header span longer than the target symbol), only the target's own
    .text section was scanned, so names and data used only by the later functions in the span were not required."""

    def test_a_span_that_runs_into_later_text_sections_requires_everything(self) -> None:
        obj = tiny_object([("FUN_00100000", CODE), ("helper", b"\0" * 8)])
        run = {"symbol": "FUN_00100000", "start": 0x100000, "end": 0x10000c}
        self.assertEqual(M.target_references(obj, run), (None, None))
        alone = dict(run, end=0x100004)         # the span is the target alone: the per-section scan stands
        self.assertEqual(M.target_references(obj, alone), (set(), set()))


class Parallelism(unittest.TestCase):
    """Audit 2026-11-17: AGENTS.md (Shared resources) caps hstmatch.py at --parallel 4; the default was 6 and the lane
    harvest passed 6."""

    def test_default_stays_within_the_cap(self) -> None:
        with tempfile.TemporaryDirectory() as d:
            _rc, _rows, calls = run_matcher(Path(d), tiny_object([("FUN_00100000", CODE)]))
            self.assertIn("xargs -0 -P 4 ", calls[0][1][0])

    @unittest.skipUnless((M.ROOT / "tools" / "hstlane.py").is_file(), "private lane driver is not in the public export")
    def test_lane_calls_stay_within_the_cap(self) -> None:
        import re
        lane = (M.ROOT / "tools" / "hstlane.py").read_text(encoding="utf-8")
        values = [int(v) for v in re.findall(r'"--parallel", "(\d+)"', lane)]
        self.assertTrue(values)
        self.assertTrue(all(v <= 4 for v in values), values)


if __name__ == "__main__":
    unittest.main()
