#!/usr/bin/env python3
"""Local review app for exact-matched sources (the owner's human review).

    python tools/hstreview.py [--port 8765]      then open http://127.0.0.1:8765/

Serves one source at a time, next to the retail disassembly of its function, with its provenance note.
Decisions are appended to reviews/decisions.jsonl, each bound to the sha256 of the exact source text that
was shown: a later edit makes the decision stale. "Accept with clones" also accepts every source whose note
says it is an address-only clone of the shown exemplar (hstclone.py), each with its own sha256. Nothing is
sent anywhere: the server listens on 127.0.0.1 only and the page loads no external resources.

The decisions log is the evidence for the campaign ledger's HUMAN_REVIEWED state (recorded there by the
toolkit, bound to each function's exact-match record); this app never edits a ledger.
"""
from __future__ import annotations

import argparse
import hashlib
import html
import json
import os
import re
import struct
import tempfile
import threading
from datetime import datetime, timezone
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import parse_qs, urlparse

import sys
sys.path.insert(0, str(Path(__file__).resolve().parent))
import hstdiff as D
import hstlint
import hstmatch as M

DECISIONS = M.ROOT / "reviews" / "decisions.jsonl"
LOCK = threading.Lock()
NOTE = re.compile(r"/\*\s*(.*?)\*/", re.S)
CLONE_OF = re.compile(r"Clone of (src/\S+)")


def sha(text: str) -> str:
    return hashlib.sha256(text.encode("utf-8")).hexdigest()


def kind_of(note: str) -> str:
    if CLONE_OF.search(note):
        return "clone"
    if "Library source" in note:
        return "library"
    if "Mechanical translation" in note:
        return "generated"
    if "corrected by hand" in note:
        return "corrected"
    if "hsttmpl.py tag" in note:
        return "template"
    if "Hand-written reconstruction" in note:
        return "hand"
    return "unknown"


_FILE_CACHE: dict[str, tuple] = {}


def load_sources() -> list[dict]:
    """Every source with its current sha256; files are re-read only when their size or mtime changed."""
    items = []
    src_dir = M.ROOT / "src"
    for root, dirs, _ in os.walk(src_dir):
        dirs.sort()
        with os.scandir(root) as it:
            entries = sorted((e for e in it if e.name.endswith(".c") or e.name.endswith(".cpp")), key=lambda e: e.name)
            for entry in entries:
                key = entry.path
                st = entry.stat()
                mtime_size = (st.st_size, st.st_mtime_ns)
                cached = _FILE_CACHE.get(key)
                if cached and cached[0] == mtime_size:
                    items.append(cached[1])
                    continue
                p = Path(key)
                item = _describe(p)
                _FILE_CACHE[key] = (mtime_size, item)
                items.append(item)
    return items


def _describe(p: Path) -> dict:
    if True:
        text = p.read_text(encoding="utf-8", errors="replace")
        h = M.parse_header(p)
        # the note as hstlint.py finds it: after the header and any one-line annotations (a DATA-TABLES pointer)
        found = NOTE.match(hstlint.provenance_note(text)[1])
        note = found.group(1).strip() if found else ""
        clone = CLONE_OF.search(note)
        return {"path": p.relative_to(M.ROOT).as_posix(), "module": h["module"], "start": h["start"],
                "end": h["end"], "symbol": h["symbol"], "cc": h.get("cc", "mwcc219"), "note": note,
                "kind": kind_of(note), "exemplar": clone.group(1) if clone else None, "sha256": sha(text)}


def decisions() -> dict[str, dict]:
    """Latest decision per source path (only those still matching the source's current sha256 count)."""
    out = {}
    if DECISIONS.exists():
        for line in DECISIONS.read_text(encoding="utf-8").splitlines():
            if line.strip():
                d = json.loads(line)
                out[d["source"]] = d
    return out


def retail_disasm(module: str, start: int, end: int) -> list[str]:
    data, seg = M.module_bytes(CACHE, module)
    off = seg["file_offset"] + start - seg["start"]
    with tempfile.TemporaryDirectory(dir=M.ROOT / "build") as tmp:
        f = Path(tmp) / "t.bin"
        f.write_bytes(data[off:off + end - start])
        return D.disasm(f, start)


CACHE: dict = {}
PAGE = """<!doctype html><html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1"><title>HST review</title>
<style>
:root{--bg:#f7f7f5;--fg:#1d1d1b;--mut:#6b6b66;--card:#fff;--line:#e2e2dc;--acc:#2f6f4f;--rej:#a33;--warn:#a86b00}
@media (prefers-color-scheme:dark){:root{--bg:#171716;--fg:#e8e8e3;--mut:#9a9a92;--card:#212120;--line:#33332f;--acc:#5fb388;--rej:#e07070;--warn:#d9a441}}
body{margin:0;background:var(--bg);color:var(--fg);font:14px/1.45 system-ui,sans-serif}
header{display:flex;gap:16px;align-items:center;flex-wrap:wrap;padding:10px 16px;border-bottom:1px solid var(--line)}
header b{font-size:15px} .mut{color:var(--mut)} select,input,button{font:inherit}
main{display:grid;grid-template-columns:1fr 1fr;gap:12px;padding:12px 16px}
@media (max-width:900px){main{grid-template-columns:1fr}}
section{background:var(--card);border:1px solid var(--line);border-radius:8px;overflow:hidden}
section h2{font-size:13px;margin:0;padding:8px 10px;border-bottom:1px solid var(--line);color:var(--mut)}
pre{margin:0;padding:10px;overflow:auto;max-height:70vh;font:12px/1.4 ui-monospace,Consolas,monospace}
#meta{padding:10px 16px} .pill{display:inline-block;padding:1px 8px;border-radius:9px;border:1px solid var(--line);margin-right:6px}
#bar{display:flex;gap:8px;flex-wrap:wrap;padding:0 16px 12px} button{padding:6px 12px;border-radius:6px;border:1px solid var(--line);background:var(--card);color:var(--fg);cursor:pointer}
button.a{border-color:var(--acc);color:var(--acc)} button.r{border-color:var(--rej);color:var(--rej)} button.w{border-color:var(--warn);color:var(--warn)}
#note{width:min(600px,90vw)}
</style></head><body>
<header><b>HST review</b>
<label>Module <select id="mod"><option value="">all</option><option>EBOOT.BIN</option><option>hstp_trans_main.prx</option></select></label>
<label>Kind <select id="kind"><option value="">all</option><option>hand</option><option>corrected</option><option>generated</option><option>library</option><option>template</option><option>clone</option></select></label>
<label><input type="checkbox" id="undecided" checked> undecided only</label>
<label><input type="checkbox" id="hidebp" checked> hide typedef/#define boilerplate</label>
<span id="stats" class="mut"></span></header>
<div id="meta"></div>
<div id="bar"><input id="note" placeholder="note (optional)">
<button class="a" onclick="decide('accepted')">Accept (a)</button>
<button class="a" onclick="decide('accepted',true)">Accept with clones (c)</button>
<button class="w" onclick="decide('needs-work')">Needs work (w)</button>
<button class="r" onclick="decide('rejected')">Reject (r)</button>
<button onclick="move(-1)">Prev (k)</button><button onclick="move(1)">Next (j)</button></div>
<main><section><h2>Source</h2><pre id="src"></pre></section><section><h2>Retail disassembly</h2><pre id="dis"></pre></section></main>
<script>
let list=[],i=0;
async function load(){const q=new URLSearchParams({module:mod.value,kind:kind.value,undecided:undecided.checked?1:0});
 const r=await fetch('/api/list?'+q);const j=await r.json();list=j.items;stats.textContent=j.stats;i=0;show();}
async function show(){if(!list.length){meta.innerHTML='Nothing to review with these filters.';src.textContent='';dis.textContent='';return;}
 const it=list[i];const r=await fetch('/api/item?path='+encodeURIComponent(it.path));const d=await r.json();
 meta.innerHTML=`<span class=pill>${i+1}/${list.length}</span><span class=pill>${d.module}</span><span class=pill>0x${d.start.toString(16)}-0x${d.end.toString(16)} (${d.end-d.start} B)</span><span class=pill>${d.kind}</span><span class=pill>${d.cc}</span>${d.clones?`<span class=pill>${d.clones} clones</span>`:''}${d.decision?`<span class=pill>decided: ${d.decision}</span>`:''}<div class=mut style="margin-top:6px">${d.path}<br>${d.note_html}</div>`;
 src.textContent=hidebp.checked?d.source.split('\\n').filter(l=>!/^(typedef |#define )/.test(l)).join('\\n'):d.source;
 dis.textContent=d.disasm.join('\\n');note.value='';}
function move(k){if(!list.length)return;i=Math.max(0,Math.min(list.length-1,i+k));show();}
async function decide(decision,clones){const it=list[i];if(!it)return;
 const r=await fetch('/api/decide',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({path:it.path,sha256:it.sha256,decision,note:note.value,with_clones:!!clones})});
 const j=await r.json();if(!j.ok){alert(j.error);return;}stats.textContent=j.stats;
 if(undecided.checked){list.splice(i,1);if(i>=list.length)i=list.length-1;}else move(1);show();}
document.addEventListener('keydown',e=>{if(e.target.tagName==='INPUT')return;const m={a:()=>decide('accepted'),c:()=>decide('accepted',true),w:()=>decide('needs-work'),r:()=>decide('rejected'),j:()=>move(1),k:()=>move(-1)};if(m[e.key]){e.preventDefault();m[e.key]();}});
[mod,kind,undecided].forEach(x=>x.addEventListener('change',load));hidebp.addEventListener('change',show);load();
</script></body></html>"""


class Handler(BaseHTTPRequestHandler):
    def _json(self, obj, code=200):
        body = json.dumps(obj).encode("utf-8")
        self.send_response(code)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, *args):
        pass

    def stats(self, items, dec):
        current = {it["path"]: it["sha256"] for it in items}
        acc = sum(1 for p, d in dec.items() if d["decision"] == "accepted" and current.get(p) == d["sha256"])
        acc_b = sum(it["end"] - it["start"] for it in items
                    if dec.get(it["path"], {}).get("decision") == "accepted" and dec[it["path"]]["sha256"] == it["sha256"])
        return f"{acc} of {len(items)} sources accepted ({acc_b} bytes)"

    def do_GET(self):
        u = urlparse(self.path)
        q = {k: v[0] for k, v in parse_qs(u.query).items()}
        if u.path == "/":
            body = PAGE.encode("utf-8")
            self.send_response(200)
            self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            self.wfile.write(body)
            return
        items = load_sources()
        dec = decisions()
        if u.path == "/api/list":
            sel = [it for it in items
                   if (not q.get("module") or it["module"] == q["module"])
                   and (not q.get("kind") or it["kind"] == q["kind"])
                   and not (q.get("undecided") == "1" and it["path"] in dec and dec[it["path"]]["sha256"] == it["sha256"])]
            # exemplars before their clones, larger functions first within a kind
            sel.sort(key=lambda it: (it["kind"] == "clone", it["module"], it["start"]))
            self._json({"items": [{"path": it["path"], "sha256": it["sha256"]} for it in sel],
                        "stats": self.stats(items, dec)})
        elif u.path == "/api/item":
            it = next((x for x in items if x["path"] == q.get("path")), None)
            if it is None:
                self._json({"error": "unknown source"}, 404)
                return
            source = (M.ROOT / it["path"]).read_text(encoding="utf-8", errors="replace")
            try:
                dis = retail_disasm(it["module"], it["start"], it["end"])
            except Exception as exc:  # disassembly is context only
                dis = [f"(disassembly unavailable: {exc})"]
            clones = sum(1 for x in items if x["exemplar"] == it["path"])
            d = dec.get(it["path"])
            self._json({**it, "source": source, "disasm": dis, "clones": clones,
                        "note_html": html.escape(it["note"]),
                        "decision": d["decision"] if d and d["sha256"] == it["sha256"] else None})
        else:
            self._json({"error": "not found"}, 404)

    def do_POST(self):
        if urlparse(self.path).path != "/api/decide":
            self._json({"error": "not found"}, 404)
            return
        req = json.loads(self.rfile.read(int(self.headers.get("Content-Length", 0))) or b"{}")
        if req.get("decision") not in ("accepted", "rejected", "needs-work"):
            self._json({"ok": False, "error": "bad decision"}, 400)
            return
        items = load_sources()
        by_path = {it["path"]: it for it in items}
        it = by_path.get(req.get("path"))
        if it is None or it["sha256"] != req.get("sha256"):
            self._json({"ok": False, "error": "the source changed since it was shown; reload"}, 409)
            return
        targets = [it]
        if req.get("with_clones") and req["decision"] == "accepted":
            targets += [x for x in items if x["exemplar"] == it["path"]]
        now = datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")
        with LOCK:
            DECISIONS.parent.mkdir(parents=True, exist_ok=True)
            with DECISIONS.open("a", encoding="utf-8", newline="\n") as fh:
                for t in targets:
                    fh.write(json.dumps({
                        "schema": "hst-decomp.review-decision.v1", "utc": now, "reviewer": "owner",
                        "source": t["path"], "sha256": t["sha256"], "module": t["module"],
                        "start": f"0x{t['start']:08x}", "end": f"0x{t['end']:08x}", "decision": req["decision"],
                        "note": req.get("note", ""),
                        "via": "clone of reviewed exemplar " + it["path"] if t is not it else "shown",
                    }) + "\n")
        self._json({"ok": True, "recorded": len(targets), "stats": self.stats(items, decisions())})


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--port", type=int, default=8765)
    args = ap.parse_args()
    server = ThreadingHTTPServer(("127.0.0.1", args.port), Handler)
    print(f"HST review on http://127.0.0.1:{args.port}/ (decisions -> {DECISIONS})", flush=True)
    server.serve_forever()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
