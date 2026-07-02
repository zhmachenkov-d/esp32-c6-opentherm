---
name: okf-knowledge
description: Build and maintain the project's OKF knowledge bundle under knowledge/ from immutable wiki/raw/ sources. Use for ingest, query, lint, OKF bundle maintenance, or when the user mentions knowledge/, Open Knowledge Format, or OKF concepts.
---

# OKF Knowledge

Compile and maintain the agent-owned **Knowledge bundle** under `knowledge/` from immutable **Raw source** files in `wiki/raw/`. Humans curate sources and ask questions; you compile concepts, update cross-links, and run tooling.

After writes to `knowledge/`, run from repo root:

```bash
pip install -e tools/okf
okf index knowledge/
okf validate knowledge/
```

## Architecture

| Layer | Path | Role |
|-------|------|------|
| Raw source | `wiki/raw/` | Immutable inputs. Read only. |
| Knowledge bundle | `knowledge/` | Compiled OKF concepts, `index.md`, `log.md`. You own this. |
| Tooling | `tools/okf/` | `validate`, `index`, `export`, `visualize`, `lint` (MCP later). |
| Schema | this file | Ingest, query, lint rules. |

### Bundle layout

```text
knowledge/
├── index.md
├── log.md
├── opentherm/
├── esp32/
├── esp-idf/
├── zigbee/
└── bridge/          # project-owned Playbooks
```

### Concept types (T1)

`Protocol`, `Reference`, `Hardware`, `Data ID`, `Attribute`, `Playbook`

### Frontmatter (required + recommended)

```yaml
---
type: Protocol
title: Human-readable title
description: One-line summary for indexes.
tags: [opentherm]
timestamp: 2026-07-02T00:00:00Z
raw:
  - wiki/raw/opentherm/2003-02-07-opentherm-protocol-v2-2.md
resource: https://optional-canonical-uri
---
```

- `type` is required (OKF v0.1).
- `raw:` lists repo-root-relative paths to raw sources (for tooling validation).
- Add `# Citations` at the bottom mirroring `raw:` and external URLs.

### Links

Use bundle-relative absolute links inside `knowledge/`:

```markdown
See [Data ID 1 TSet](/opentherm/data-id-1-tset.md).
```

In conversation, cite with project-root paths: `knowledge/opentherm/data-id-1-tset.md`.

## Initialization

On first ingest, create only what is missing:

- `knowledge/index.md` (empty heading `# Knowledge Bundle` until `okf index` runs)
- `knowledge/log.md` (heading `# Directory Update Log`)
- topic subdirectories as needed

Do not auto-create on query or lint — tell the user to ingest first.

## Ingest

Always both steps: fetch into `wiki/raw/`, then compile into `knowledge/`.

### Fetch (`wiki/raw/`)

1. Get source content; translate to English if needed.
2. Reuse an existing `wiki/raw/<topic>/` subdirectory when close enough.
3. Save as `wiki/raw/<topic>/YYYY-MM-DD-descriptive-slug.md` (or without date if published date unknown).
4. Preserve original text; clean formatting noise only.

### Compile (`knowledge/`)

Granularity: **G2 medium** — split major reference areas into linked concepts (e.g. protocol overview, frame format, individual data IDs) rather than one page per source.

Placement:

- Third-party specs and libraries → topic domain (`opentherm/`, `esp32/`, …)
- Project mappings and procedures → `bridge/` as type `Playbook`

After creating or updating concepts:

1. Update cross-links between affected concepts.
2. Append to `knowledge/log.md` (newest date first, ISO `YYYY-MM-DD` headings).
3. Run `okf index knowledge/` then `okf validate knowledge/`.

Log entry format:

```markdown
## 2026-07-02
* **Creation**: Added [Data ID 1 TSet](/opentherm/data-id-1-tset.md).
* **Update**: Linked [OpenTherm Frame Format](/opentherm/opentherm-frame-format.md) to timing rules.
```

## Query

1. Read `knowledge/index.md` for progressive disclosure.
2. Read relevant concepts; prefer bundle content over training knowledge.
3. Cite with project-root paths in conversation.
4. Do not write files unless the user asks to archive the answer.

### Archive (when asked)

Create a new concept (often type `Reference` or `Playbook`) with the synthesized answer. Update log and run `okf index` + `okf validate`.

## Lint

Run deterministic checks in tooling; the skill handles heuristic review.

```bash
okf lint knowledge/
okf lint knowledge/ --fix
```

**Tooling (`okf lint`)** — validate conformance, raw provenance, citation mirroring, index staleness, broken links; `--fix` regenerates stale indexes and rewrites links when exactly one basename match exists.

**Skill (heuristic, report only)** — contradictions, stale claims vs newer raw sources, orphan concepts, missing G2 splits, concepts mentioned in prose without their own page.

Append to `knowledge/log.md`:

```markdown
## 2026-07-02
* **Lint**: 2 warnings, 1 auto-fixed link.
```

## Export

When the user asks to export or share the bundle:

```bash
okf export knowledge/ -o dist/
```

Produces `dist/knowledge.tar.gz` and `dist/knowledge-manifest.json`.

## Visualize

Generate a self-contained interactive graph viewer (Marie-style OKF viz):

```bash
okf visualize knowledge/
```

Produces `dist/knowledge-viz.html` by default. Open in a browser for force-directed graph navigation, concept detail panel, search, type filter, and backlinks.
