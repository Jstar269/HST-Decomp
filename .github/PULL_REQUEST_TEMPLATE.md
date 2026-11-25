## What this adds or changes

## Matcher output

<!-- For added or changed matching sources in `src/`, paste the status lines from `python tools/hstmatch.py <paths>`.
     If no matching source in `src/` changes, write N/A. -->

```text
```

## Checks (see CONTRIBUTING.md, "Checks before you finish")

- [ ] For added or changed matching sources in `src/`, `python tools/hstmatch.py <paths>` reports EXACT for each source
- [ ] `python tools/hstprcheck.py --base origin/main` reports OK
- [ ] `python tools/hstgate.py --all --wip`, `python tools/hstreuse.py check` and
      `python -m unittest discover -s tools -p "test_*.py"` pass
- [ ] For a completed good first issue function, this PR removes its entry in `data/starter-issues.json`
      and its obsolete `wip/` files
- [ ] Each new source has a true line-2 provenance note, and no other source already covers the function
- [ ] New compiler lessons, if any, are added to `docs/MWCC-NOTES.md`
- [ ] No game data, decrypted modules, disassembly, keys or compiler binaries are included, and nothing from the game
      was posted to a web tool
- [ ] I wrote this change (or name its source above), may submit it under the repository's licences (CC0-1.0 for
      `src/`, `wip/` and `symbols/`, MIT for tools and documentation), and name any AI tool I used

The maintainer re-verifies every source before accepting it and applies accepted changes in the maintainer's tree
(CONTRIBUTING.md, "Pull requests").
