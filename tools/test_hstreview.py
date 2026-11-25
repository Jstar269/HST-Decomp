#!/usr/bin/env python3
"""Tests for hstreview.py (local review web app and helper functions)."""
from __future__ import annotations

import json
import os
import sys
import tempfile
import threading
import time
import unittest
from pathlib import Path
from unittest.mock import patch
from urllib.request import Request, urlopen
from urllib.error import HTTPError

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstreview as R


class ReviewHelperTests(unittest.TestCase):
    def test_sha(self) -> None:
        text = "test"
        expected = "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"
        self.assertEqual(R.sha(text), expected)

    def test_kind_of(self) -> None:
        self.assertEqual(R.kind_of("Clone of src/eboot/fn1.c"), "clone")
        self.assertEqual(R.kind_of("Library source code from SDK"), "library")
        self.assertEqual(R.kind_of("Mechanical translation from disassembly"), "generated")
        self.assertEqual(R.kind_of("Source code corrected by hand"), "corrected")
        self.assertEqual(R.kind_of("Generated with hsttmpl.py tag"), "template")
        self.assertEqual(R.kind_of("Hand-written reconstruction"), "hand")
        self.assertEqual(R.kind_of("Uncategorized note"), "unknown")


class SourceLoadingAndDecisionsTests(unittest.TestCase):
    def test_decisions(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            dec_file = Path(tmp) / "decisions.jsonl"
            lines = [
                json.dumps({"source": "src/a.c", "decision": "needs-work", "sha256": "111"}),
                json.dumps({"source": "src/a.c", "decision": "accepted", "sha256": "222"}),
                json.dumps({"source": "src/b.c", "decision": "rejected", "sha256": "333"}),
            ]
            dec_file.write_text("\n".join(lines) + "\n", encoding="utf-8")

            old_decisions = R.DECISIONS
            try:
                R.DECISIONS = dec_file
                dec = R.decisions()
                self.assertEqual(len(dec), 2)
                self.assertEqual(dec["src/a.c"]["decision"], "accepted")
                self.assertEqual(dec["src/a.c"]["sha256"], "222")
                self.assertEqual(dec["src/b.c"]["decision"], "rejected")
            finally:
                R.DECISIONS = old_decisions

    def test_load_sources_and_file_cache(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            src_dir = root / "src"
            src_dir.mkdir()

            file_a = src_dir / "func_a.c"
            file_a.write_text(
                "/* HSTMATCH module=EBOOT.BIN start=0x00010000 end=0x00010020 symbol=func_a */\n"
                "/* Hand-written reconstruction */\n"
                "int func_a(void) { return 0; }\n",
                encoding="utf-8",
            )

            file_b = src_dir / "func_b.cpp"
            file_b.write_text(
                "/* HSTMATCH module=EBOOT.BIN start=0x00010020 end=0x00010040 symbol=func_b */\n"
                "/* Clone of src/func_a.c */\n"
                "int func_b(void) { return 0; }\n",
                encoding="utf-8",
            )

            old_root = R.M.ROOT
            old_cache = dict(R._FILE_CACHE)
            try:
                R.M.ROOT = root
                R._FILE_CACHE.clear()

                items = R.load_sources()
                self.assertEqual(len(items), 2)

                item_a = next(i for i in items if i["path"] == "src/func_a.c")
                self.assertEqual(item_a["module"], "EBOOT.BIN")
                self.assertEqual(item_a["kind"], "hand")
                self.assertIsNone(item_a["exemplar"])

                item_b = next(i for i in items if i["path"] == "src/func_b.cpp")
                self.assertEqual(item_b["kind"], "clone")
                self.assertEqual(item_b["exemplar"], "src/func_a.c")

                # Test cache hit
                cache_size = len(R._FILE_CACHE)
                self.assertEqual(cache_size, 2)
                items_cached = R.load_sources()
                self.assertEqual(len(items_cached), 2)

                # Test cache invalidation on modified file
                time.sleep(0.01)
                file_a.write_text(
                    "/* HSTMATCH module=EBOOT.BIN start=0x00010000 end=0x00010020 symbol=func_a */\n"
                    "/* Source code corrected by hand */\n"
                    "int func_a(void) { return 1; }\n",
                    encoding="utf-8",
                )
                items_updated = R.load_sources()
                item_a_updated = next(i for i in items_updated if i["path"] == "src/func_a.c")
                self.assertEqual(item_a_updated["kind"], "corrected")

            finally:
                R.M.ROOT = old_root
                R._FILE_CACHE.clear()
                R._FILE_CACHE.update(old_cache)


class AnnotatedNoteTests(unittest.TestCase):
    def test_the_note_after_a_data_tables_pointer_is_the_note(self) -> None:
        header = "/* HSTMATCH module=EBOOT.BIN start=0x{0:08x} end=0x{1:08x} symbol=FUN_{0:08x} lang=c99 */\n"
        pointer = "/* Table reader: row count (this+8); see docs/DATA-TABLES.md */\n"
        hand = "/* Hand-written reconstruction from the disassembly; exact match verified. */"
        clone = "/* Clone of src/ex.c (hstclone.py tag t1): addresses substituted. */"
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "src").mkdir()
            (root / "src" / "ex.c").write_text(header.format(0x10000, 0x10020) + pointer + hand + "\nint x;\n",
                                               encoding="utf-8")
            (root / "src" / "tw.c").write_text(header.format(0x10020, 0x10040) + pointer + clone + "\nint y;\n",
                                               encoding="utf-8")
            old_root, old_cache = R.M.ROOT, dict(R._FILE_CACHE)
            try:
                R.M.ROOT = root
                R._FILE_CACHE.clear()
                items = {it["path"]: it for it in R.load_sources()}
            finally:
                R.M.ROOT = old_root
                R._FILE_CACHE.clear()
                R._FILE_CACHE.update(old_cache)
        self.assertEqual((items["src/ex.c"]["kind"], items["src/ex.c"]["note"]), ("hand", hand[3:-3].strip()))
        # "Accept with clones" finds the clone through its note
        self.assertEqual((items["src/tw.c"]["kind"], items["src/tw.c"]["exemplar"]), ("clone", "src/ex.c"))

    def test_the_kind_filter_offers_templates(self) -> None:
        self.assertIn("<option>template</option>", R.PAGE)


class DisassemblyTests(unittest.TestCase):
    def test_retail_disasm(self) -> None:
        old_module_bytes = R.M.module_bytes
        old_disasm = R.D.disasm
        old_root = R.M.ROOT
        old_cache = dict(R.CACHE)

        with tempfile.TemporaryDirectory() as tmp:
            try:
                R.M.ROOT = Path(tmp)
                (R.M.ROOT / "build").mkdir()
                R.CACHE.clear()

                def mock_module_bytes(cache, module):
                    data = b"\x00" * 0x100
                    seg = {"file_offset": 0x10, "start": 0x00010000}
                    return data, seg

                def mock_disasm(bin_path, start):
                    return [f"0x{start:08x}: nop"]

                R.M.module_bytes = mock_module_bytes
                R.D.disasm = mock_disasm

                res = R.retail_disasm("EBOOT.BIN", 0x00010000, 0x00010010)
                self.assertEqual(res, ["0x00010000: nop"])
            finally:
                R.M.module_bytes = old_module_bytes
                R.D.disasm = old_disasm
                R.M.ROOT = old_root
                R.CACHE.clear()
                R.CACHE.update(old_cache)


class HttpHandlerTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.tmp_dir = tempfile.TemporaryDirectory()
        cls.root = Path(cls.tmp_dir.name)
        cls.src_dir = cls.root / "src"
        cls.src_dir.mkdir()
        (cls.root / "build").mkdir()

        cls.file_ex = cls.src_dir / "exemplar.c"
        cls.text_ex = (
            "/* HSTMATCH module=EBOOT.BIN start=0x00010000 end=0x00010010 symbol=exemplar */\n"
            "/* Hand-written reconstruction */\n"
            "int exemplar(void) { return 1; }\n"
        )
        cls.file_ex.write_text(cls.text_ex, encoding="utf-8")
        cls.sha_ex = R.sha(cls.text_ex)

        cls.file_clone = cls.src_dir / "clone.c"
        cls.text_clone = (
            "/* HSTMATCH module=EBOOT.BIN start=0x00010010 end=0x00010020 symbol=clone */\n"
            "/* Clone of src/exemplar.c */\n"
            "int clone(void) { return 1; }\n"
        )
        cls.file_clone.write_text(cls.text_clone, encoding="utf-8")
        cls.sha_clone = R.sha(cls.text_clone)

        cls.decisions_file = cls.root / "reviews" / "decisions.jsonl"

        cls.old_root = R.M.ROOT
        cls.old_decisions = R.DECISIONS
        cls.old_cache = dict(R._FILE_CACHE)
        cls.old_disasm_cache = dict(R.CACHE)
        cls.old_disasm = R.D.disasm
        cls.old_module_bytes = R.M.module_bytes

        R.M.ROOT = cls.root
        R.DECISIONS = cls.decisions_file
        R._FILE_CACHE.clear()
        R.CACHE.clear()

        R.D.disasm = lambda path, start: ["0x00010000: nop"]
        R.M.module_bytes = lambda cache, module: (b"\x00" * 0x100, {"file_offset": 0, "start": 0x00010000})

        cls.server = R.ThreadingHTTPServer(("127.0.0.1", 0), R.Handler)
        cls.port = cls.server.server_address[1]
        cls.base_url = f"http://127.0.0.1:{cls.port}"

        cls.server_thread = threading.Thread(target=cls.server.serve_forever, daemon=True)
        cls.server_thread.start()

    @classmethod
    def tearDownClass(cls) -> None:
        cls.server.shutdown()
        cls.server.server_close()
        cls.server_thread.join()

        R.M.ROOT = cls.old_root
        R.DECISIONS = cls.old_decisions
        R.D.disasm = cls.old_disasm
        R.M.module_bytes = cls.old_module_bytes
        R._FILE_CACHE.clear()
        R._FILE_CACHE.update(cls.old_cache)
        R.CACHE.clear()
        R.CACHE.update(cls.old_disasm_cache)

        cls.tmp_dir.cleanup()

    def setUp(self) -> None:
        R._FILE_CACHE.clear()
        if self.decisions_file.exists():
            self.decisions_file.unlink()

    def _get(self, path: str) -> tuple[int, dict | str]:
        req = Request(f"{self.base_url}{path}")
        with urlopen(req) as resp:
            code = resp.status
            content_type = resp.headers.get("Content-Type", "")
            body = resp.read().decode("utf-8")
            if "application/json" in content_type:
                return code, json.loads(body)
            return code, body

    def _post(self, path: str, payload: dict) -> tuple[int, dict]:
        data = json.dumps(payload).encode("utf-8")
        req = Request(f"{self.base_url}{path}", data=data, headers={"Content-Type": "application/json"}, method="POST")
        try:
            with urlopen(req) as resp:
                return resp.status, json.loads(resp.read().decode("utf-8"))
        except HTTPError as err:
            with err:
                return err.code, json.loads(err.read().decode("utf-8"))

    def test_get_page(self) -> None:
        code, body = self._get("/")
        self.assertEqual(code, 200)
        self.assertIn("<!doctype html>", body)
        self.assertIn("<title>HST review</title>", body)

    def test_get_api_list(self) -> None:
        code, data = self._get("/api/list")
        self.assertEqual(code, 200)
        self.assertIn("items", data)
        self.assertIn("stats", data)
        paths = [item["path"] for item in data["items"]]
        self.assertEqual(paths, ["src/exemplar.c", "src/clone.c"])

        # Filter by kind
        code, data_kind = self._get("/api/list?kind=hand")
        self.assertEqual(code, 200)
        self.assertEqual([i["path"] for i in data_kind["items"]], ["src/exemplar.c"])

        # Filter by module
        code, data_mod = self._get("/api/list?module=EBOOT.BIN")
        self.assertEqual(code, 200)
        self.assertEqual(len(data_mod["items"]), 2)

    def test_get_api_item(self) -> None:
        code, data = self._get("/api/item?path=src/exemplar.c")
        self.assertEqual(code, 200)
        self.assertEqual(data["path"], "src/exemplar.c")
        self.assertEqual(data["kind"], "hand")
        self.assertEqual(data["clones"], 1)
        self.assertIn("int exemplar(void)", data["source"])
        self.assertEqual(data["disasm"], ["0x00010000: nop"])

    def test_get_api_item_not_found(self) -> None:
        with self.assertRaises(HTTPError) as cm:
            self._get("/api/item?path=src/nonexistent.c")
        with cm.exception as err:
            self.assertEqual(err.code, 404)
            data = json.loads(err.read().decode("utf-8"))
            self.assertEqual(data["error"], "unknown source")

    def test_post_api_decide_valid(self) -> None:
        payload = {
            "path": "src/exemplar.c",
            "sha256": self.sha_ex,
            "decision": "accepted",
            "note": "looks good",
        }
        code, resp = self._post("/api/decide", payload)
        self.assertEqual(code, 200)
        self.assertTrue(resp["ok"])
        self.assertEqual(resp["recorded"], 1)

        dec = R.decisions()
        self.assertIn("src/exemplar.c", dec)
        self.assertEqual(dec["src/exemplar.c"]["decision"], "accepted")

    def test_post_api_decide_with_clones(self) -> None:
        payload = {
            "path": "src/exemplar.c",
            "sha256": self.sha_ex,
            "decision": "accepted",
            "note": "accept all clones",
            "with_clones": True,
        }
        code, resp = self._post("/api/decide", payload)
        self.assertEqual(code, 200)
        self.assertTrue(resp["ok"])
        self.assertEqual(resp["recorded"], 2)

        dec = R.decisions()
        self.assertIn("src/exemplar.c", dec)
        self.assertIn("src/clone.c", dec)
        self.assertEqual(dec["src/clone.c"]["decision"], "accepted")

    def test_post_api_decide_stale_sha(self) -> None:
        payload = {
            "path": "src/exemplar.c",
            "sha256": "0000000000000000000000000000000000000000000000000000000000000000",
            "decision": "accepted",
        }
        code, resp = self._post("/api/decide", payload)
        self.assertEqual(code, 409)
        self.assertFalse(resp["ok"])
        self.assertIn("the source changed", resp["error"])

    def test_post_api_decide_bad_decision(self) -> None:
        payload = {
            "path": "src/exemplar.c",
            "sha256": self.sha_ex,
            "decision": "invalid-choice",
        }
        code, resp = self._post("/api/decide", payload)
        self.assertEqual(code, 400)
        self.assertFalse(resp["ok"])
        self.assertEqual(resp["error"], "bad decision")

    def test_404_not_found(self) -> None:
        with self.assertRaises(HTTPError) as cm:
            self._get("/api/unknown")
        with cm.exception as err:
            self.assertEqual(err.code, 404)


class LoadSourcesTests(unittest.TestCase):
    """load_sources over a synthetic src/ tree: only .c/.cpp files, a fixed order, and re-reading only changed files."""

    SOURCES = ["src/top.c", "src/a/B_upper.cpp", "src/a/a_lower.c", "src/b/Zeta.c", "src/b/alpha.cpp"]
    OTHERS = ["src/a/header.h", "src/a/notes.txt", "src/b/readme.c.txt", "src/b/Upper.H"]

    def setUp(self) -> None:
        tmp = tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        self.root = Path(tmp.name)
        for k, rel in enumerate(self.SOURCES + self.OTHERS):
            path = self.root / rel
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(f"/* HSTMATCH module=EBOOT.BIN start=0x{0x10000 + 0x20 * k:08x} "
                            f"end=0x{0x10020 + 0x20 * k:08x} symbol=f{k} */\n"
                            f"/* Hand-written reconstruction */\nint f{k}(void) {{ return {k}; }}\n", encoding="utf-8")
        root_patch = patch.object(R.M, "ROOT", self.root)
        root_patch.start()
        self.addCleanup(root_patch.stop)
        saved = dict(R._FILE_CACHE)
        self.addCleanup(lambda: (R._FILE_CACHE.clear(), R._FILE_CACHE.update(saved)))
        R._FILE_CACHE.clear()

    def test_only_c_and_cpp_files_in_a_fixed_order(self) -> None:
        # directory by directory from src/ down, directories and names in code-point order (upper case first)
        paths = [item["path"] for item in R.load_sources()]
        self.assertEqual(paths, self.SOURCES)
        R._FILE_CACHE.clear()
        self.assertEqual([item["path"] for item in R.load_sources()], paths)

    def test_a_changed_file_is_described_again_and_the_rest_come_from_the_cache(self) -> None:
        first = {item["path"]: item for item in R.load_sources()}
        changed = self.root / "src" / "a" / "a_lower.c"
        changed.write_text(changed.read_text(encoding="utf-8").replace("Hand-written reconstruction",
                                                                       "Source code corrected by hand"),
                           encoding="utf-8")
        with patch.object(R, "_describe", wraps=R._describe) as describe:
            second = {item["path"]: item for item in R.load_sources()}
        self.assertEqual([call.args[0] for call in describe.call_args_list], [changed])
        self.assertEqual(second["src/a/a_lower.c"]["kind"], "corrected")
        for path in self.SOURCES:
            if path != "src/a/a_lower.c":
                self.assertIs(second[path], first[path])                       # the cached item itself

    def test_an_mtime_change_alone_is_noticed(self) -> None:
        R.load_sources()
        touched = self.root / "src" / "b" / "Zeta.c"
        st = touched.stat()
        os.utime(touched, ns=(st.st_atime_ns, st.st_mtime_ns + 5_000_000_000))  # same size, newer mtime
        with patch.object(R, "_describe", wraps=R._describe) as describe:
            R.load_sources()
        self.assertEqual([call.args[0] for call in describe.call_args_list], [touched])


if __name__ == "__main__":
    unittest.main()
