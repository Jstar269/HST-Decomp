#!/usr/bin/env python3
"""Tests for hstconfig.py machine-specific settings reader."""
from __future__ import annotations

import io
import json
import os
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstconfig


class ConfigTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmpdir = tempfile.TemporaryDirectory()
        self.tmppath = Path(self.tmpdir.name)
        self.config_file = self.tmppath / "hst-config.json"

        # Patch CONFIG_FILE and clear _file_cache
        self.config_patch = patch.object(hstconfig, "CONFIG_FILE", self.config_file)
        self.config_patch.start()
        hstconfig._file_cache = None

        # Backup environment variables prefixed with HST_
        self.orig_env = {k: v for k, v in os.environ.items() if k.startswith(hstconfig.ENV_PREFIX)}
        for k in self.orig_env:
            del os.environ[k]

    def tearDown(self) -> None:
        self.config_patch.stop()
        hstconfig._file_cache = None

        # Restore environment variables
        for k in list(os.environ.keys()):
            if k.startswith(hstconfig.ENV_PREFIX):
                del os.environ[k]
        for k, v in self.orig_env.items():
            os.environ[k] = v

        self.tmpdir.cleanup()

    def test_env_name(self) -> None:
        self.assertEqual(hstconfig.env_name("campaign_root"), "HST_CAMPAIGN_ROOT")
        self.assertEqual(hstconfig.env_name("pspdev_bin"), "HST_PSPDEV_BIN")

    def test_source_get_require_unset(self) -> None:
        self.assertEqual(hstconfig.source("campaign_root"), "unset")
        self.assertIsNone(hstconfig.get("campaign_root"))
        with self.assertRaises(hstconfig.ConfigError) as ctx:
            hstconfig.require("campaign_root")
        self.assertIn("setting 'campaign_root' is not set", str(ctx.exception))

    def test_source_unknown_key(self) -> None:
        with self.assertRaises(KeyError):
            hstconfig.source("nonexistent_key")

    def test_file_values_and_precedence(self) -> None:
        self.config_file.write_text(json.dumps({
            "_comment": "This is a comment",
            "campaign_root": "/path/from/file",
            "pspdev_bin": "/wsl/from/file"
        }), encoding="utf-8")
        hstconfig._file_cache = None

        self.assertEqual(hstconfig.source("campaign_root"), "file")
        self.assertEqual(hstconfig.get("campaign_root"), "/path/from/file")
        self.assertEqual(hstconfig.require("campaign_root"), "/path/from/file")

        # Environment variable overrides file
        os.environ["HST_CAMPAIGN_ROOT"] = "/path/from/env"
        self.assertEqual(hstconfig.source("campaign_root"), "env")
        self.assertEqual(hstconfig.get("campaign_root"), "/path/from/env")
        self.assertEqual(hstconfig.require("campaign_root"), "/path/from/env")

        # Empty or whitespace env var falls back to file
        os.environ["HST_CAMPAIGN_ROOT"] = "   "
        self.assertEqual(hstconfig.source("campaign_root"), "file")
        self.assertEqual(hstconfig.get("campaign_root"), "/path/from/file")

    def test_file_json_errors(self) -> None:
        # Invalid JSON
        self.config_file.write_text("not json", encoding="utf-8")
        hstconfig._file_cache = None
        with self.assertRaises(hstconfig.ConfigError) as ctx:
            hstconfig.get("campaign_root")
        self.assertIn("is not valid JSON", str(ctx.exception))

        # Non-dictionary JSON
        self.config_file.write_text(json.dumps(["a", "b"]), encoding="utf-8")
        hstconfig._file_cache = None
        with self.assertRaises(hstconfig.ConfigError) as ctx:
            hstconfig.get("campaign_root")
        self.assertIn("must contain a JSON object", str(ctx.exception))

        # Unknown key
        self.config_file.write_text(json.dumps({"unknown_key": "val"}), encoding="utf-8")
        hstconfig._file_cache = None
        with self.assertRaises(hstconfig.ConfigError) as ctx:
            hstconfig.get("campaign_root")
        self.assertIn("unknown key 'unknown_key'", str(ctx.exception))

        # Non-string value
        self.config_file.write_text(json.dumps({"campaign_root": 12345}), encoding="utf-8")
        hstconfig._file_cache = None
        with self.assertRaises(hstconfig.ConfigError) as ctx:
            hstconfig.get("campaign_root")
        self.assertIn("must be a string", str(ctx.exception))

    def test_host_path(self) -> None:
        # Assertion error if not host key
        with self.assertRaises(AssertionError):
            hstconfig.host_path("pspdev_bin")

        # Existing directory
        existing_dir = self.tmppath / "existing"
        existing_dir.mkdir()
        os.environ["HST_CAMPAIGN_ROOT"] = str(existing_dir)
        self.assertEqual(hstconfig.host_path("campaign_root"), existing_dir)

        # Non-existing directory for "host" kind raises ConfigError
        missing_dir = self.tmppath / "missing"
        os.environ["HST_CAMPAIGN_ROOT"] = str(missing_dir)
        with self.assertRaises(hstconfig.ConfigError) as ctx:
            hstconfig.host_path("campaign_root")
        self.assertIn("does not exist", str(ctx.exception))

        # "host-new" kind allows non-existing directory
        os.environ["HST_EXT_ROOT"] = str(missing_dir)
        self.assertEqual(hstconfig.host_path("ext_root"), missing_dir)

        # Tilde expansion
        os.environ["HST_EXT_ROOT"] = "~/test_ext_root"
        expected = Path(os.path.expanduser("~/test_ext_root"))
        self.assertEqual(hstconfig.host_path("ext_root"), expected)

    def test_wsl_path(self) -> None:
        # Assertion error if not wsl key
        with self.assertRaises(AssertionError):
            hstconfig.wsl_path("campaign_root")

        os.environ["HST_PSPDEV_BIN"] = "/opt/pspdev/bin///"
        self.assertEqual(hstconfig.wsl_path("pspdev_bin"), "/opt/pspdev/bin")

        os.environ["HST_PSPDEV_BIN"] = "///"
        self.assertEqual(hstconfig.wsl_path("pspdev_bin"), "/")

    @patch("subprocess.run")
    def test_wsl_exists(self, mock_run) -> None:
        # Path with double quote returns False immediately without running subprocess
        self.assertFalse(hstconfig._wsl_exists('path/with/"quote'))
        mock_run.assert_not_called()

        # Success case
        mock_run.return_value.returncode = 0
        self.assertTrue(hstconfig._wsl_exists("/valid/path"))
        mock_run.assert_called_once_with(["wsl.exe", "-e", "sh", "-c", 'test -e "/valid/path"'], capture_output=True)

        # Failure case
        mock_run.reset_mock()
        mock_run.return_value.returncode = 1
        self.assertFalse(hstconfig._wsl_exists("/invalid/path"))

    @patch("subprocess.run", side_effect=FileNotFoundError(2, "No such file or directory", "wsl.exe"))
    def test_wsl_exists_without_wsl_exe_is_a_config_error(self, _mock_run) -> None:
        # off Windows (or without WSL) `check` used to end in a FileNotFoundError traceback
        with self.assertRaises(hstconfig.ConfigError) as ctx:
            hstconfig._wsl_exists("/opt/pspdev/bin/psp-ld")
        self.assertIn("wsl.exe was not found", str(ctx.exception))

    def test_the_example_config_loads_as_documented(self) -> None:
        # docs/GUIDE.md, Setup step 3: copy hst-config.example.json to hst-config.json, then check
        example = Path(hstconfig.__file__).resolve().parents[1] / "hst-config.example.json"
        self.config_file.write_text(example.read_text(encoding="utf-8"), encoding="utf-8")
        hstconfig._file_cache = None
        data = json.loads(example.read_text(encoding="utf-8"))
        keys = [k for k in data if not k.startswith("_")]
        self.assertEqual(sorted(set(keys) - set(hstconfig.KEYS)), [], "example keys hstconfig.py does not know")
        for key in keys:
            self.assertEqual(hstconfig.get(key), data[key].strip())
            self.assertIn("_" + key, data, f"the example documents no _{key} comment")

    # the 13 settings hstconfig.py accepted before host_clang and android_ndk were added
    MAIN_KEYS = ("campaign_root", "pspdev_bin", "mwcc_native", "gcc_home", "gcc4_home", "newlib_home",
                 "newlib_obj_root", "ext_root", "nakagawa_root", "mwcc_builds_dir", "wibo_exe", "ghidra_headless",
                 "java_home")

    def _check_exit(self) -> int:
        hstconfig._file_cache = None
        with patch.object(sys, "argv", ["hstconfig.py", "check"]), \
             patch("sys.stdout", new_callable=io.StringIO), patch("sys.stderr", new_callable=io.StringIO), \
             patch.object(hstconfig, "_wsl_exists", return_value=True):
            return hstconfig.main()

    def _settings_for(self, keys) -> dict:
        settings = {}
        for key in keys:
            spec = hstconfig.KEYS[key]
            base = self.tmppath / "host" / key
            if spec.kind != "wsl" and spec.probe:
                (base / spec.probe).parent.mkdir(parents=True, exist_ok=True)
                (base / spec.probe).write_text("", encoding="utf-8")
            elif spec.kind != "wsl":
                base.mkdir(parents=True, exist_ok=True)
            settings[key] = str(base) if spec.kind != "wsl" else "/opt/" + key
        return settings

    def test_check_passes_a_config_without_the_optional_compilers(self) -> None:
        # a config that sets every key main accepted (13) must still pass `check`
        self.config_file.write_text(json.dumps(self._settings_for(self.MAIN_KEYS)), encoding="utf-8")
        self.assertEqual(self._check_exit(), 0)

    def test_check_reports_a_configured_optional_compiler_that_is_missing(self) -> None:
        settings = self._settings_for(self.MAIN_KEYS)
        settings["host_clang"] = str(self.tmppath / "absent" / "clang.exe")
        self.config_file.write_text(json.dumps(settings), encoding="utf-8")
        self.assertEqual(self._check_exit(), 1)

    def test_main_get(self) -> None:
        os.environ["HST_CAMPAIGN_ROOT"] = "/test/campaign"
        with patch.object(sys, "argv", ["hstconfig.py", "get", "campaign_root"]), \
             patch("sys.stdout", new_callable=io.StringIO) as mock_out:
            code = hstconfig.main()
            self.assertEqual(code, 0)
            self.assertEqual(mock_out.getvalue().strip(), "/test/campaign")

    def test_main_list(self) -> None:
        os.environ["HST_CAMPAIGN_ROOT"] = "/test/campaign"
        with patch.object(sys, "argv", ["hstconfig.py", "list"]), \
             patch("sys.stdout", new_callable=io.StringIO) as mock_out:
            code = hstconfig.main()
            self.assertEqual(code, 0)
            out = mock_out.getvalue()
            self.assertIn("campaign_root", out)
            self.assertIn("[env  ]", out)

    @patch("hstconfig._wsl_exists")
    def test_main_check(self, mock_wsl_exists) -> None:
        mock_wsl_exists.return_value = True

        # When all keys are unset, check returns 1
        with patch.object(sys, "argv", ["hstconfig.py", "check"]), \
             patch("sys.stdout", new_callable=io.StringIO), \
             patch("sys.stderr", new_callable=io.StringIO) as mock_err:
            code = hstconfig.main()
            self.assertEqual(code, 1)
            self.assertIn("setting(s) unset or missing", mock_err.getvalue())

        # Create dummy directory structure for host probes
        dummy_dir = self.tmppath / "dummy"
        dummy_dir.mkdir()
        (dummy_dir / "ledger").mkdir()
        (dummy_dir / "ledger" / "campaign.json").touch()
        (dummy_dir / "tools").mkdir()
        (dummy_dir / "tools" / "import_audit_baseline.json").touch()
        (dummy_dir / "bin").mkdir()

        for k in hstconfig.KEYS:
            if hstconfig.KEYS[k].kind == "wsl":
                os.environ[hstconfig.env_name(k)] = "/wsl/dummy/path"
            else:
                os.environ[hstconfig.env_name(k)] = str(dummy_dir)

        with patch.object(sys, "argv", ["hstconfig.py", "check"]), \
             patch("sys.stdout", new_callable=io.StringIO) as mock_out:
            code = hstconfig.main()
            self.assertEqual(code, 0)
            self.assertIn("ok", mock_out.getvalue())


if __name__ == "__main__":
    unittest.main()
