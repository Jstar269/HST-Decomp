#!/usr/bin/env python3
"""Create the campaign directory the tools read, from your own decrypted copies of the game's executables.

    python tools/hstbootstrap.py --out <campaign dir> --eboot <decrypted EBOOT.BIN> [--sharing <decrypted DATA.PSP>]

Then set `campaign_root` in hst-config.json (or HST_CAMPAIGN_ROOT) to <campaign dir>.

The modules this repository reconstructs are described in data/modules.json: where each one comes from on the disc
(Hot Shots Tennis: Get a Grip, UCUS98701, North America, disc version 1.01), the SHA-256 and size of its decrypted
form, its load segment and the list of its reviewed function boundaries (data/functions-<module>.txt).

For each module given, the tool checks the file against that SHA-256. A gzip-compressed image is inflated first,
because some decryption tools write the module before decompressing it. Nothing is decrypted here: decrypting your
copy is your own responsibility. On success the tool copies the checked bytes to <campaign dir>/inputs/ and writes
<campaign dir>/ledger/campaign.json (modules, load segments, functions) and
<campaign dir>/inputs/revision-selection.json (where the inputs are), the two files hstmatch.py and the other tools
read. A module you do not give is left out; the tools then report its sources as unavailable.
"""
from __future__ import annotations

import argparse
import gzip
import hashlib
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
MODULES = ROOT / "data" / "modules.json"


def load_checked(path: Path, want_sha256: str, want_size: int) -> bytes | None:
    """The module bytes when they (or their gzip-inflated form) match the expected digest, else None."""
    data = path.read_bytes()
    candidates = [data]
    if data[:2] == b"\x1f\x8b":
        try:
            candidates.append(gzip.decompress(data))
        except OSError:
            pass
    for c in candidates:
        if len(c) == want_size and hashlib.sha256(c).hexdigest() == want_sha256:
            return c
    return None


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--out", required=True, help="campaign directory to create or update")
    ap.add_argument("--eboot", help="your decrypted EBOOT.BIN (module EBOOT.BIN)")
    ap.add_argument("--sharing", help="your decrypted Sharing Version executable (module hstp_trans_main.prx)")
    args = ap.parse_args()
    given = {"EBOOT.BIN": args.eboot, "hstp_trans_main.prx": args.sharing}
    if not any(given.values()):
        ap.error("give at least one of --eboot and --sharing")
    for option, path in (("--eboot", args.eboot), ("--sharing", args.sharing)):
        if path and not Path(path).is_file():
            ap.error(f"{option} {path}: no such file (give the path of your decrypted module)")

    spec = json.loads(MODULES.read_text(encoding="utf-8"))
    out = Path(args.out).resolve()

    modules, inputs, failed = [], {}, False
    for mod in spec["modules"]:
        path = given.get(mod["name"])
        if not path:
            print(f"{mod['name']}: not given, left out")
            continue
        data = load_checked(Path(path), mod["sha256"], mod["size"])
        if data is None:
            print(f"{mod['name']}: {path} is not the expected decrypted module "
                  f"(want {mod['size']} bytes, SHA-256 {mod['sha256']}; source on the disc: {mod['origin']})")
            failed = True
            continue
        name = f"{mod['name']}.decrypted"
        inputs[name] = data
        functions = []
        for line in (ROOT / mod["functions_file"]).read_text(encoding="utf-8").splitlines():
            if line and not line.startswith("#"):
                start, end = (int(x, 16) for x in line.split())
                functions.append({"start": start, "end": end, "boundary_status": "reviewed"})
        if len(functions) != mod["function_count"]:
            raise SystemExit(f"{mod['functions_file']}: {len(functions)} functions, expected {mod['function_count']}")
        modules.append({"name": mod["name"], "input": {"root": "inputs", "path": name},
                        "input_sha256": mod["sha256"], "load_segments": mod["load_segments"], "functions": functions})
        print(f"{mod['name']}: OK ({len(functions)} functions)")
    if failed:
        return 1   # nothing written: a half-made campaign directory would only fail later, in every tool

    (out / "inputs").mkdir(parents=True, exist_ok=True)
    (out / "ledger").mkdir(parents=True, exist_ok=True)
    for name, data in inputs.items():
        (out / "inputs" / name).write_bytes(data)
    ledger = {"schema": "hst-decomp.bootstrap-campaign.v1", "title_id": spec["title_id"], "modules": modules}
    selection = {"schema": "hst-decomp.bootstrap-selection.v1", "roots": {"inputs": str(out / "inputs")}}
    (out / "ledger" / "campaign.json").write_text(json.dumps(ledger, indent=1) + "\n", encoding="utf-8")
    (out / "inputs" / "revision-selection.json").write_text(json.dumps(selection, indent=1) + "\n", encoding="utf-8")
    print(f"wrote {out / 'ledger' / 'campaign.json'}\nset campaign_root in hst-config.json to: {out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
