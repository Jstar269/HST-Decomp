#!/usr/bin/env python3
"""Synthetic unit tests for tools/hstbootstrap.py."""
from __future__ import annotations

import gzip
import io
import hashlib
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstbootstrap as B


class LoadCheckedTests(unittest.TestCase):
    def test_load_checked_raw_bytes_matches(self) -> None:
        data = b"hello world decrypted data"
        sha = hashlib.sha256(data).hexdigest()
        size = len(data)

        with tempfile.TemporaryDirectory() as tmp:
            file_path = Path(tmp) / "eboot.bin"
            file_path.write_bytes(data)

            res = B.load_checked(file_path, sha, size)
            self.assertEqual(res, data)

    def test_load_checked_gzip_bytes_matches(self) -> None:
        data = b"hello compressed world data"
        compressed = gzip.compress(data)
        sha = hashlib.sha256(data).hexdigest()
        size = len(data)

        with tempfile.TemporaryDirectory() as tmp:
            file_path = Path(tmp) / "eboot.bin.gz"
            file_path.write_bytes(compressed)

            res = B.load_checked(file_path, sha, size)
            self.assertEqual(res, data)

    def test_load_checked_hash_mismatch(self) -> None:
        data = b"hello world data"
        wrong_sha = "0" * 64
        size = len(data)

        with tempfile.TemporaryDirectory() as tmp:
            file_path = Path(tmp) / "eboot.bin"
            file_path.write_bytes(data)

            res = B.load_checked(file_path, wrong_sha, size)
            self.assertIsNone(res)

    def test_load_checked_size_mismatch(self) -> None:
        data = b"hello world data"
        sha = hashlib.sha256(data).hexdigest()
        wrong_size = len(data) + 10

        with tempfile.TemporaryDirectory() as tmp:
            file_path = Path(tmp) / "eboot.bin"
            file_path.write_bytes(data)

            res = B.load_checked(file_path, sha, wrong_size)
            self.assertIsNone(res)

    def test_load_checked_corrupted_gzip_header(self) -> None:
        # Magic bytes 1f 8b but invalid gzip stream afterwards
        data = b"\x1f\x8bcorrupted stream bytes"
        wrong_sha = "1" * 64
        size = len(data)

        with tempfile.TemporaryDirectory() as tmp:
            file_path = Path(tmp) / "eboot.bin"
            file_path.write_bytes(data)

            res = B.load_checked(file_path, wrong_sha, size)
            self.assertIsNone(res)


class MainTests(unittest.TestCase):
    def test_main_no_arguments_fails(self) -> None:
        with patch.object(sys, "argv", ["hstbootstrap.py", "--out", "/tmp/campaign"]):
            with self.assertRaises(SystemExit) as cm:
                B.main()
            self.assertNotEqual(cm.exception.code, 0)

    def test_main_missing_file_is_a_usage_error_not_a_traceback(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            out_dir = Path(tmp) / "campaign"
            for missing in (Path(tmp) / "no-such-EBOOT.BIN", Path(tmp)):   # a typo, or a directory
                with patch.object(sys, "argv", ["hstbootstrap.py", "--out", str(out_dir), "--eboot", str(missing)]), \
                        patch("sys.stderr", new_callable=io.StringIO) as err:
                    with self.assertRaises(SystemExit) as cm:
                        B.main()
                self.assertEqual(cm.exception.code, 2)
                self.assertIn("no such file", err.getvalue())
            self.assertFalse(out_dir.exists())

    def test_main_module_hash_check_fails(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            tmp_path = Path(tmp)
            out_dir = tmp_path / "campaign"
            fake_eboot = tmp_path / "eboot.bin"
            fake_eboot.write_bytes(b"invalid data")

            with patch.object(sys, "argv", ["hstbootstrap.py", "--out", str(out_dir), "--eboot", str(fake_eboot)]):
                ret = B.main()
                self.assertEqual(ret, 1)
            self.assertFalse(out_dir.exists(), "a refused module leaves no campaign directory behind")

    def test_main_success_single_and_both_modules(self) -> None:
        eboot_data = b"eboot executable contents"
        eboot_sha = hashlib.sha256(eboot_data).hexdigest()
        eboot_size = len(eboot_data)

        sharing_data = b"sharing version executable contents"
        sharing_sha = hashlib.sha256(sharing_data).hexdigest()
        sharing_size = len(sharing_data)

        spec = {
            "schema": "hst-decomp.modules.v1",
            "title": "Test Title",
            "title_id": "TEST12345",
            "modules": [
                {
                    "name": "EBOOT.BIN",
                    "origin": "disc/eboot",
                    "sha256": eboot_sha,
                    "size": eboot_size,
                    "load_segments": [{"flags": 7, "start": 0}],
                    "functions_file": "data/functions-EBOOT.BIN.txt",
                    "function_count": 2,
                },
                {
                    "name": "hstp_trans_main.prx",
                    "origin": "disc/sharing",
                    "sha256": sharing_sha,
                    "size": sharing_size,
                    "load_segments": [{"flags": 7, "start": 0}],
                    "functions_file": "data/functions-hstp_trans_main.prx",
                    "function_count": 1,
                },
            ],
        }

        func_eboot = "# header comment\n0000 0010\n0010 0020\n"
        func_sharing = "0000 0008\n"

        with tempfile.TemporaryDirectory() as tmp:
            tmp_path = Path(tmp)
            data_dir = tmp_path / "data"
            data_dir.mkdir()
            out_dir = tmp_path / "campaign"

            eboot_file = tmp_path / "EBOOT.BIN"
            eboot_file.write_bytes(eboot_data)

            sharing_file = tmp_path / "DATA.PSP"
            sharing_file.write_bytes(sharing_data)

            func_eboot_file = data_dir / "functions-EBOOT.BIN.txt"
            func_eboot_file.write_text(func_eboot, encoding="utf-8")

            func_sharing_file = data_dir / "functions-hstp_trans_main.prx"
            func_sharing_file.write_text(func_sharing, encoding="utf-8")

            modules_file = data_dir / "modules.json"
            modules_file.write_text(json.dumps(spec), encoding="utf-8")

            with (
                patch.object(B, "ROOT", tmp_path),
                patch.object(B, "MODULES", modules_file),
                patch.object(
                    sys,
                    "argv",
                    [
                        "hstbootstrap.py",
                        "--out",
                        str(out_dir),
                        "--eboot",
                        str(eboot_file),
                        "--sharing",
                        str(sharing_file),
                    ],
                ),
            ):
                ret = B.main()
                self.assertEqual(ret, 0)

            self.assertTrue((out_dir / "inputs" / "EBOOT.BIN.decrypted").exists())
            self.assertEqual((out_dir / "inputs" / "EBOOT.BIN.decrypted").read_bytes(), eboot_data)
            self.assertTrue((out_dir / "inputs" / "hstp_trans_main.prx.decrypted").exists())
            self.assertEqual((out_dir / "inputs" / "hstp_trans_main.prx.decrypted").read_bytes(), sharing_data)

            campaign_json_path = out_dir / "ledger" / "campaign.json"
            self.assertTrue(campaign_json_path.exists())
            campaign_data = json.loads(campaign_json_path.read_text(encoding="utf-8"))
            self.assertEqual(campaign_data["title_id"], "TEST12345")
            self.assertEqual(len(campaign_data["modules"]), 2)
            self.assertEqual(len(campaign_data["modules"][0]["functions"]), 2)
            self.assertEqual(campaign_data["modules"][0]["functions"][0], {"start": 0, "end": 16, "boundary_status": "reviewed"})

            revision_json_path = out_dir / "inputs" / "revision-selection.json"
            self.assertTrue(revision_json_path.exists())
            selection_data = json.loads(revision_json_path.read_text(encoding="utf-8"))
            self.assertEqual(selection_data["roots"]["inputs"], str(out_dir / "inputs"))

    def test_main_function_count_mismatch_raises_system_exit(self) -> None:
        eboot_data = b"eboot executable contents"
        eboot_sha = hashlib.sha256(eboot_data).hexdigest()
        eboot_size = len(eboot_data)

        spec = {
            "schema": "hst-decomp.modules.v1",
            "title": "Test Title",
            "title_id": "TEST12345",
            "modules": [
                {
                    "name": "EBOOT.BIN",
                    "origin": "disc/eboot",
                    "sha256": eboot_sha,
                    "size": eboot_size,
                    "load_segments": [{"flags": 7, "start": 0}],
                    "functions_file": "data/functions-EBOOT.BIN.txt",
                    "function_count": 5,  # Want 5, but file has only 1 function
                },
            ],
        }

        func_eboot = "0000 0010\n"

        with tempfile.TemporaryDirectory() as tmp:
            tmp_path = Path(tmp)
            data_dir = tmp_path / "data"
            data_dir.mkdir()
            out_dir = tmp_path / "campaign"

            eboot_file = tmp_path / "EBOOT.BIN"
            eboot_file.write_bytes(eboot_data)

            func_eboot_file = data_dir / "functions-EBOOT.BIN.txt"
            func_eboot_file.write_text(func_eboot, encoding="utf-8")

            modules_file = data_dir / "modules.json"
            modules_file.write_text(json.dumps(spec), encoding="utf-8")

            with (
                patch.object(B, "ROOT", tmp_path),
                patch.object(B, "MODULES", modules_file),
                patch.object(
                    sys,
                    "argv",
                    [
                        "hstbootstrap.py",
                        "--out",
                        str(out_dir),
                        "--eboot",
                        str(eboot_file),
                    ],
                ),
            ):
                with self.assertRaises(BaseException) as cm:
                    B.main()
                self.assertIn("1 functions, expected 5", str(cm.exception))


if __name__ == "__main__":
    unittest.main()
