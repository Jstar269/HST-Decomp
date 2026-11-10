#!/usr/bin/env python3
"""Synthetic, game-free checks of the compiler profile set: tools/hstmatch.py's psp-gcc profiles (the unoptimized
gcc131-O0 / gcc171-O0 next to the optimizing ones), the toolchain lock's view of them (tools/hstproof.py
live_profiles) and the independent verifier's recipes (tools/hstxverify.py). No compiler or game file is needed.

    python tools/test_hstmatch_profiles.py [-v]
"""
from __future__ import annotations

import json
import sys
import unittest
from pathlib import Path
from unittest import mock

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
import hstmatch  # noqa: E402
import hstproof  # noqa: E402
import hstxverify  # noqa: E402

HOME = "/synthetic/gcc"
KIT171_B = "psp-gcc-1.7.1/lib/gcc-lib/psp/3.3.3+allegrex-2.2.2-psp-1.7.1/"
UNOPTIMIZED = {"gcc131-O0", "gcc171-O0"}
G4HOME = "/synthetic/lbp-gcc4"
GCC402X = {f"gcc402x-{o}" for o in ("O0", "O1", "O2", "O3", "Os")}
GCC404X = {f"gcc404x-{o}" for o in ("O0", "O1", "O2", "O3", "Os")}


class GccProfileTests(unittest.TestCase):
    def setUp(self) -> None:
        self.addCleanup(setattr, hstmatch, "_gcc_profiles", hstmatch._gcc_profiles)
        hstmatch._gcc_profiles = None
        for name, value in (("gcc_home", HOME), ("gcc4_home", G4HOME)):
            patch = mock.patch.object(hstmatch, name, return_value=value)
            patch.start()
            self.addCleanup(patch.stop)

    def test_unoptimized_sony_gcc_profiles_are_available(self) -> None:
        profiles = hstmatch.gcc_profiles()
        self.assertEqual({n for n in profiles if n.endswith("-O0")}, UNOPTIMIZED | {"gcc402x-O0", "gcc404x-O0"})
        self.assertLessEqual(UNOPTIMIZED, hstmatch.GCC_PROFILE_NAMES)
        for name in UNOPTIMIZED:
            self.assertEqual(profiles[name]["driver"], f"{HOME}/psp-gcc-1.3.1/bin/psp-gcc")
            self.assertEqual(profiles[name]["flags"], "-O0 -G0 -fno-merge-constants")
        self.assertIsNone(profiles["gcc131-O0"]["B"])
        self.assertEqual(profiles["gcc171-O0"]["B"], f"{HOME}/{KIT171_B}")

    def test_optimizing_profiles_are_unchanged(self) -> None:
        profiles = hstmatch.gcc_profiles()
        self.assertEqual(len(profiles), 28)
        self.assertEqual(profiles["gcc171-O2"]["flags"], "-O2 -G0 -fno-merge-constants")
        self.assertEqual(profiles["gcc171x-O2"]["flags"], "-O2 -G0 -fno-merge-constants -mext")
        self.assertEqual(profiles["gcc131-Os"]["flags"], "-Os -G0 -fno-merge-constants")
        self.assertNotIn("gcc171x-O0", profiles)

    def test_the_lock_keeps_every_locked_profile_and_adds_only_the_unoptimized_ones(self) -> None:
        """The toolchain lock records the profile table (data/toolchain-manifest.json core.profiles): every profile
        the accepted proofs were made with must keep its exact record; the only difference is the two new ones."""
        live = hstproof.live_profiles()
        locked = json.loads((TOOLS.parent / "data" / "toolchain-manifest.json").read_text(encoding="utf-8"))
        locked = locked["core"]["profiles"]
        for name, rec in locked.items():
            self.assertEqual(live.get(name), rec, name)
        self.assertLessEqual(set(live) - set(locked), UNOPTIMIZED | GCC402X | GCC404X)
        self.assertEqual(live["gcc171-O0"], {"compiler": "psp-gcc", "driver": "psp-gcc-1.3.1/bin/psp-gcc",
                                             "B": KIT171_B, "flags": "-O0 -G0 -fno-merge-constants"})


    def test_gcc402x_is_a_secondary_profile_with_its_own_kit(self) -> None:
        """Owner decision 2026-10-27: gcc402x-* (GCC 4.0.2 + pspdev patch + Allegrex tuning, the lbp-gcc4 kit) is a
        secondary profile: five optimisation levels, 16-byte stack alignment, the kit directory recorded by name."""
        profiles = hstmatch.gcc_profiles()
        self.assertLessEqual(GCC402X, hstmatch.GCC_PROFILE_NAMES)
        for name in GCC402X:
            o = name.split("-")[1]
            self.assertEqual(profiles[name]["driver"], f"{G4HOME}/psp-gcc-4.0.2x/bin/psp-gcc")
            self.assertIsNone(profiles[name]["B"])
            self.assertEqual(profiles[name]["flags"], f"-{o} -G0 -fno-merge-constants -mpreferred-stack-boundary=4")
        live = hstproof.live_profiles()
        self.assertEqual(live["gcc402x-O2"], {"compiler": "psp-gcc", "kit": "gcc4_home",
                                              "driver": "psp-gcc-4.0.2x/bin/psp-gcc", "B": None,
                                              "flags": "-O2 -G0 -fno-merge-constants -mpreferred-stack-boundary=4"})
        # the default profiles do not change: no record of theirs names a kit
        self.assertTrue(all("kit" not in rec for n, rec in live.items() if n not in GCC402X | GCC404X))

    def test_gcc404x_is_the_sdk_only_profile_on_the_same_kit(self) -> None:
        """Owner decision 2026-11-09: gcc404x-* (psp-gcc-4.0.4x, lbp-decomp's GCC 4.0.4 kit with ins-from-zero) for
        Sony SDK code only; same kit setting and flags as gcc402x-*, its own build directory."""
        profiles = hstmatch.gcc_profiles()
        self.assertLessEqual(GCC404X, hstmatch.GCC_PROFILE_NAMES)
        for name in GCC404X:
            o = name.split("-")[1]
            self.assertEqual(profiles[name]["driver"], f"{G4HOME}/psp-gcc-4.0.4x/bin/psp-gcc")
            self.assertEqual(profiles[name]["flags"], f"-{o} -G0 -fno-merge-constants -mpreferred-stack-boundary=4")
        self.assertEqual(hstproof.live_profiles()["gcc404x-O2"]["kit"], "gcc4_home")

    def test_gcc402x_without_its_setting_is_refused_only_when_used(self) -> None:
        hstmatch._gcc_profiles = None
        with mock.patch.object(hstmatch, "gcc4_home", return_value=None):
            profiles = hstmatch.gcc_profiles()
            self.assertIsNone(profiles["gcc402x-O2"]["driver"])
            self.assertEqual(profiles["gcc171-O2"]["driver"], f"{HOME}/psp-gcc-1.3.1/bin/psp-gcc")
            with self.assertRaises(hstproof.ProofError):
                hstproof.live_profiles()


class IndependentVerifierProfileTests(unittest.TestCase):
    def test_same_profile_set_as_the_matcher(self) -> None:
        with mock.patch.object(hstmatch, "gcc_home", return_value=HOME), \
                mock.patch.object(hstmatch, "gcc4_home", return_value=G4HOME), \
                mock.patch.object(hstmatch, "_gcc_profiles", None):
            matcher = hstmatch.gcc_profiles()
            verifier = hstxverify.gcc_profiles("$HOME/hst-gcc", "$HOME/lbp-gcc4")
            self.assertEqual(set(verifier), set(matcher))
            for name, rec in matcher.items():
                self.assertEqual(verifier[name]["flags"], rec["flags"], name)

    @unittest.skipUnless(sys.platform == "win32", "the recipe maps Windows drive paths to WSL")
    def test_unoptimized_compile_recipe(self) -> None:
        cmd = hstxverify.compile_command(Path("C:/x/sample.c"), Path("C:/x/o.o"), Path("C:/x/log"),
                                         {"profile": "gcc171-O0", "lang": "c99"}, "$HOME/hst-mwcc",
                                         Path("C:/x/include"), "$HOME/hst-gcc")
        self.assertIn("-B $HOME/hst-gcc/psp-gcc-1.7.1/lib/gcc-lib/psp/", cmd)
        self.assertIn("-c -O0 -G0 -fno-merge-constants -x c ", cmd)

    @unittest.skipUnless(sys.platform == "win32", "the recipe maps Windows drive paths to WSL")
    def test_gcc402x_compile_recipe(self) -> None:
        with mock.patch.object(hstxverify.CFG, "get", return_value="$HOME/lbp-gcc4"):
            cmd = hstxverify.compile_command(Path("C:/x/sample.c"), Path("C:/x/o.o"), Path("C:/x/log"),
                                             {"profile": "gcc402x-O2", "lang": "c99"}, "$HOME/hst-mwcc",
                                             Path("C:/x/include"), "$HOME/hst-gcc")
        self.assertIn("$HOME/lbp-gcc4/psp-gcc-4.0.2x/bin/psp-gcc -c -O2 -G0 -fno-merge-constants "
                      "-mpreferred-stack-boundary=4 -x c ", cmd)
        self.assertNotIn("-B ", cmd)


if __name__ == "__main__":
    unittest.main()
