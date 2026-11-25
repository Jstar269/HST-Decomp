#!/usr/bin/env python3
"""Synthetic tests for the scratch compiler tool (tools/hstcc.py)."""
from __future__ import annotations

import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import MagicMock, patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstcc
import hstmatch as M


class HstCcTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.tmp_path = Path(self.tmp.name)
        # hstcc writes its object to ROOT/build/hstcc; keep that inside the temp dir.
        root_patch = patch.object(M, "ROOT", self.tmp_path)
        root_patch.start()
        self.addCleanup(root_patch.stop)

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def compiles(self, stem: str):
        """A stand-in for the WSL step that leaves the object a successful compile writes."""
        def run(*_args, **_kwargs) -> None:
            (self.tmp_path / "build" / "hstcc" / f"{stem}.o").write_bytes(b"\x7fELF")
        return run

    @patch.object(M, "run_wsl")
    @patch.object(M, "pspdev_bin", return_value="/opt/pspdev/bin")
    @patch.object(M, "native_dir", return_value="/tmp/mwcc_native")
    def test_cpp_source_defaults(
        self, mock_native_dir: MagicMock, mock_pspdev_bin: MagicMock, mock_run_wsl: MagicMock
    ) -> None:
        src = self.tmp_path / "probe.cpp"
        src.write_text("int f() { return 1; }\n", encoding="utf-8")
        mock_run_wsl.side_effect = self.compiles("probe")

        with patch.object(sys, "argv", ["hstcc.py", str(src)]):
            res = hstcc.main()

        self.assertEqual(res, 0)
        mock_run_wsl.assert_called_once()
        lines, name, native = mock_run_wsl.call_args[0]
        self.assertEqual(name, "hstcc")
        self.assertEqual(native, "/tmp/mwcc_native")
        self.assertEqual(len(lines), 1)

        cmd = lines[0]
        self.assertIn("-lang c++", cmd)
        self.assertIn(f"$N/builds/{M.PROFILE['build']}/mwccpsp.exe", cmd)
        self.assertIn(M.PROFILE["flags"], cmd)
        self.assertIn("/opt/pspdev/bin/psp-objdump", cmd)
        self.assertIn("probe.o", cmd)
        self.assertIn("probe.cpp", cmd)

    @patch.object(M, "run_wsl")
    @patch.object(M, "pspdev_bin", return_value="/opt/pspdev/bin")
    @patch.object(M, "native_dir", return_value="/tmp/mwcc_native")
    def test_c_source_defaults(
        self, mock_native_dir: MagicMock, mock_pspdev_bin: MagicMock, mock_run_wsl: MagicMock
    ) -> None:
        src = self.tmp_path / "probe.c"
        src.write_text("int f(void) { return 1; }\n", encoding="utf-8")
        mock_run_wsl.side_effect = self.compiles("probe")

        with patch.object(sys, "argv", ["hstcc.py", str(src)]):
            res = hstcc.main()

        self.assertEqual(res, 0)
        mock_run_wsl.assert_called_once()
        lines = mock_run_wsl.call_args[0][0]
        cmd = lines[0]
        self.assertIn("-lang c99", cmd)
        self.assertIn("probe.c", cmd)

    @patch.object(M, "run_wsl")
    @patch.object(M, "pspdev_bin", return_value="/opt/pspdev/bin")
    @patch.object(M, "native_dir", return_value="/tmp/mwcc_native")
    def test_lang_override(
        self, mock_native_dir: MagicMock, mock_pspdev_bin: MagicMock, mock_run_wsl: MagicMock
    ) -> None:
        src_cpp = self.tmp_path / "probe.cpp"
        src_cpp.write_text("int f() { return 1; }\n", encoding="utf-8")
        mock_run_wsl.side_effect = self.compiles("probe")

        with patch.object(sys, "argv", ["hstcc.py", str(src_cpp), "--lang", "c99"]):
            res = hstcc.main()

        self.assertEqual(res, 0)
        cmd = mock_run_wsl.call_args[0][0][0]
        self.assertIn("-lang c99", cmd)

        mock_run_wsl.reset_mock()
        mock_run_wsl.side_effect = self.compiles("probe")

        src_c = self.tmp_path / "probe.c"
        src_c.write_text("int f(void) { return 1; }\n", encoding="utf-8")

        with patch.object(sys, "argv", ["hstcc.py", str(src_c), "--lang", "c++"]):
            res = hstcc.main()

        self.assertEqual(res, 0)
        cmd = mock_run_wsl.call_args[0][0][0]
        self.assertIn("-lang c++", cmd)

    @patch.object(M, "run_wsl")
    @patch.object(M, "pspdev_bin", return_value="/opt/pspdev/bin")
    @patch.object(M, "native_dir", return_value="/tmp/mwcc_native")
    def test_custom_build_and_extra_flags(
        self, mock_native_dir: MagicMock, mock_pspdev_bin: MagicMock, mock_run_wsl: MagicMock
    ) -> None:
        src = self.tmp_path / "probe.cpp"
        src.write_text("int f() { return 1; }\n", encoding="utf-8")
        mock_run_wsl.side_effect = self.compiles("probe")

        with patch.object(
            sys, "argv", ["hstcc.py", str(src), "--build", "220", "--extra", "-opt nopeephole"]
        ):
            res = hstcc.main()

        self.assertEqual(res, 0)
        cmd = mock_run_wsl.call_args[0][0][0]
        self.assertIn("$N/builds/220/mwccpsp.exe", cmd)
        self.assertIn("-opt nopeephole", cmd)

    @patch.object(M, "run_wsl")
    @patch.object(M, "pspdev_bin", return_value="/opt/pspdev/bin")
    @patch.object(M, "native_dir", return_value="/tmp/mwcc_native")
    def test_output_directory_creation(
        self, mock_native_dir: MagicMock, mock_pspdev_bin: MagicMock, mock_run_wsl: MagicMock
    ) -> None:
        src = self.tmp_path / "probe.c"
        src.write_text("int f(void) { return 1; }\n", encoding="utf-8")

        out_dir = self.tmp_path / "build" / "hstcc"
        self.assertFalse(out_dir.exists())
        with patch.object(sys, "argv", ["hstcc.py", str(src)]):
            hstcc.main()

        self.assertTrue(out_dir.exists())
        self.assertTrue(out_dir.is_dir())

    @patch.object(M, "run_wsl")
    @patch.object(M, "pspdev_bin", return_value="/opt/pspdev/bin")
    @patch.object(M, "native_dir", return_value="/tmp/mwcc_native")
    def test_a_failed_compile_exits_1_even_after_an_earlier_probe(
        self, mock_native_dir: MagicMock, mock_pspdev_bin: MagicMock, mock_run_wsl: MagicMock
    ) -> None:
        src = self.tmp_path / "probe.c"
        src.write_text("int f(void) { return }\n", encoding="utf-8")
        out = self.tmp_path / "build" / "hstcc"
        out.mkdir(parents=True)
        (out / "probe.o").write_bytes(b"\x7fELF")          # left by an earlier, successful probe
        with patch.object(sys, "argv", ["hstcc.py", str(src)]), patch("sys.stderr"):
            res = hstcc.main()                             # the stand-in WSL step writes no object
        self.assertEqual(res, 1)
        self.assertFalse((out / "probe.o").exists())

    def test_missing_source_argument(self) -> None:
        with patch.object(sys, "argv", ["hstcc.py"]), patch("sys.stderr"):
            with self.assertRaises(SystemExit):
                hstcc.main()

    def test_invalid_lang_choice(self) -> None:
        src = self.tmp_path / "probe.c"
        src.write_text("int f(void) { return 1; }\n", encoding="utf-8")
        with patch.object(sys, "argv", ["hstcc.py", str(src), "--lang", "fortran"]), patch("sys.stderr"):
            with self.assertRaises(SystemExit):
                hstcc.main()


if __name__ == "__main__":
    unittest.main()
