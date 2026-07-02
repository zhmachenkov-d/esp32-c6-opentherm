# OKF knowledge system replaces wiki/articles

This project compiles agent-readable knowledge as an [Open Knowledge Format](https://github.com/GoogleCloudPlatform/knowledge-catalog/blob/main/okf/SPEC.md) (OKF) bundle under `knowledge/`, sourced from immutable inputs in `wiki/raw/`. We replace `wiki/articles/` and the `llm-wiki` skill rather than migrating articles in place, because the bundle is agent-owned compiled output (not hand-edited docs), kept separate from tooling and prompts, and consumed by reusable Python tooling plus a thin external-agent interface.

**Considered options:** migrate `wiki/articles/` in place (rejected — legacy format fights OKF); parallel wiki + OKF trees (rejected — dual maintenance); human-authored `knowledge/` (rejected — bookkeeping belongs to the agent).

**Consequences:**

- `knowledge/` contains only OKF concepts plus `index.md` and `log.md` (topic domains: `opentherm/`, `esp32/`, `esp-idf/`, `zigbee/`, `bridge/`).
- Concepts use medium granularity (G2), T1 types (`Protocol`, `Reference`, `Hardware`, `Data ID`, `Attribute`, `Playbook`), YAML frontmatter with `raw:` provenance and `# Citations` in the body.
- `.cursor/skills/okf-knowledge/` owns ingest, query, and lint; `tools/okf/` (Python) owns validate, index regen, export, then MCP.
- v1 is done when all `wiki/raw/` topics are re-ingested, `bridge/` has synthesis playbooks (setpoint/temperature mapping, control flow), tooling passes, and `wiki/articles/` is removed.
- Initial migration re-compiles from `wiki/raw/` only; old articles are a coverage checklist, not the migration source.
