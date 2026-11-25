#!/usr/bin/env python3
"""Synthetic tests for tools/hstsdkunits.py: the instruction test and the check of the committed units file."""
from __future__ import annotations

import io
import json
import sys
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstsdkunits as U  # noqa: E402

DOC = {"schema": U.SCHEMA, "meaning": "synthetic", "method": "synthetic",
       "modules": {"EBOOT.BIN": [{"start": "0x00001000", "end": "0x00001100", "functions": 2,
                                  "ins_functions": ["0x00001040"]}]}}


class InsFromZeroTests(unittest.TestCase):
    def test_only_ins_with_zero_source_counts(self) -> None:
        ins = (0x1F << 26) | (5 << 16) | (7 << 11) | (3 << 6) | 0x04     # ins a1,$zero,3,5
        self.assertTrue(U.ins_from_zero(ins))
        self.assertFalse(U.ins_from_zero(ins | (4 << 21)))               # ins a1,a0,3,5
        self.assertFalse(U.ins_from_zero((ins & ~0x3F) | 0x00))          # ext


class CheckTests(unittest.TestCase):
    def run_main(self, committed: str | None) -> tuple[int, str]:
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp) / "sdk-gcc4-units.json"
            if committed is not None:
                out.write_text(committed, encoding="utf-8")
            buf = io.StringIO()
            with mock.patch.object(U, "build", return_value=DOC), mock.patch.object(U, "OUT", out), \
                    mock.patch.object(sys, "argv", ["hstsdkunits.py"]), redirect_stdout(buf):
                code = U.main()
        return code, buf.getvalue()

    def test_a_committed_file_that_matches_exits_0(self) -> None:
        code, text = self.run_main(json.dumps(DOC, indent=1) + "\n")
        self.assertIn("committed file matches", text)
        self.assertEqual(code, 0)

    def test_a_committed_file_that_differs_exits_1(self) -> None:
        code, text = self.run_main(json.dumps(dict(DOC, modules={}), indent=1) + "\n")
        self.assertIn("committed file DIFFERS", text)
        self.assertEqual(code, 1)


if __name__ == "__main__":
    unittest.main()
