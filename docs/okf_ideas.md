# Understanding OKF for Your Life and Business

OKF (Open Knowledge Format) is Google's formalization of the **Karpathy LLM Wiki** pattern. If Karpathy's gist is the *idea*, OKF is the *contract* that makes that idea portable.

## The Core Shift

Most AI setups work like **RAG**: you upload documents, the model retrieves chunks, and it re-discovers the same facts on every question. Nothing accumulates.

OKF + LLM wiki works differently:

1. **Raw sources** stay immutable (articles, transcripts, contracts, sensor logs).
2. An **LLM-maintained wiki** compiles knowledge into linked markdown concept pages.
3. A **schema** (like your project's `llm-wiki` skill or `AGENTS.md`) tells the agent how to ingest, query, and lint.

The wiki **compounds**: cross-links, contradictions, and synthesis are already done. You curate sources and ask questions; the LLM does the bookkeeping Karpathy describes — "touch 15 files in one pass" without getting bored.

## What OKF Adds on Top

Karpathy's pattern is powerful but bespoke. OKF pins down a small interoperability layer:

| Element | Purpose |
|--------|---------|
| **One concept = one `.md` file** | Tables, people, playbooks, metrics — each gets a page |
| **YAML frontmatter** | `type` (required), plus `title`, `description`, `resource`, `tags`, `timestamp` |
| **Markdown links** | Build a knowledge graph (`/tables/orders.md` → `/metrics/weekly-active-users.md`) |
| **`index.md`** | Progressive disclosure — agent reads the catalog first |
| **`log.md`** | Chronological history of what changed |
| **Git repo** | Version control, diffs, collaboration |

No SDK, no platform lock-in. Ship it as a tarball, Obsidian vault, or repo mount. Any agent can read it; any pipeline can produce it.

## How You'd Actually Use It

**For life:** a personal OKF bundle in git — health decisions, financial rules, family logistics, learning notes — maintained by Gemini/Claude with memory + your ingest workflow.

**For business:** the same pattern, but scoped to customers, products, processes, and metrics. Onboarding a new hire or agent becomes "read the bundle" instead of tribal knowledge in Slack.

**The loop:**

```
Source → Ingest (LLM compiles into concepts) → Query (answer + optionally archive) → Lint (fix stale/broken links)
```

The repo's `llm-wiki` skill implements Karpathy's pattern; OKF would mainly add standardized frontmatter (`type`, `resource`, etc.) so bundles can be shared, visualized, or consumed by other tools (e.g. Google's Knowledge Catalog visualizer).

## References

- [Google Cloud: How the Open Knowledge Format can improve data sharing](https://cloud.google.com/blog/products/data-analytics/how-the-open-knowledge-format-can-improve-data-sharing/)
- [OKF Specification (SPEC.md)](https://github.com/GoogleCloudPlatform/knowledge-catalog/blob/main/okf/SPEC.md)
- [Andrej Karpathy: LLM Wiki](https://gist.github.com/karpathy/442a6bf555914893e9891c11519de94f)

---

## 20 Ideas You Could Build

### Personal Life (1–7)

1. **Personal operating system** — Concepts for goals, values, habits, and weekly reviews. Types: `Goal`, `Habit`, `Decision`, `Reflection`. Links show how a health goal connects to sleep data and meal prep playbooks.

2. **Health & medical wiki** — One page per condition, medication, provider, and lab trend. Type: `Medical Record`, `Symptom Pattern`. Resource URIs point to portal links. Agent answers "what changed since my last A1C?" without re-reading every PDF.

3. **Family logistics hub** — School calendars, pickup rules, appliance manuals, warranty dates, emergency contacts. Type: `Contact`, `Schedule`, `Asset`. Replaces scattered Notes/Drive searches.

4. **Home maintenance runbook** — HVAC filter sizes, breaker map, irrigation schedule, contractor history. Type: `Playbook`, `Vendor`, `Asset`. Links from "water heater" to the incident when it leaked.

5. **Learning companion for a deep topic** — e.g. investing, parenting, or ESP32/OpenTherm. Ingest papers and docs; build concept pages that evolve over months. Type: `Concept`, `Reference`, `Comparison`.

6. **Book / course wiki** — As you read, one concept per character/theme/chapter. By the end you have a personal Tolkien Gateway. Query: "how does theme X evolve?" Archive good answers back into the bundle.

7. **Life decisions log** — Major decisions (job change, move, purchase) as concepts with `# Citations` to pros/cons docs. Type: `Decision`. Future-you asks "why did we choose this school?"

### Business & Work (8–14)

8. **Company context bundle for AI agents** — Products, ICP, pricing, brand voice, objection handling. New chat sessions start with "read `/business/index.md`". Type: `Product`, `Persona`, `Playbook`.

9. **Customer intelligence wiki** — One concept per key account: stakeholders, contract terms, open issues, last call summary. Ingest Gong/Zoom transcripts on a schedule. Type: `Account`, `Stakeholder`, `Meeting`.

10. **Standard operating procedures (SOPs) as code** — Onboarding, invoicing, support tiers, escalation paths. Type: `Playbook`. Links between SOPs mirror real dependencies ("refunds" → "Stripe" → "accounting").

11. **Metrics & definitions catalog** — What "MRR", "active user", and "churn" *mean for your business*, with SQL/examples. Type: `Metric`. Stops every analyst and agent from re-deriving definitions.

12. **Competitive & market intelligence** — Competitors, feature matrices, pricing changes, win/loss themes. Ingest earnings calls and blog posts; lint for stale claims. Type: `Competitor`, `Comparison`.

13. **Project memory for a small team** — Architecture decisions, ADRs, incident postmortems, "why we chose X". Type: `ADR`, `Incident`, `API`. Replaces "ask Sarah — she remembers."

14. **Sales & marketing content engine** — Case studies, proof points, testimonial quotes, compliance boundaries. Agent drafts proposals citing linked concepts instead of hallucinating claims.

### Technical & Hybrid (15–20)

15. **IoT / product knowledge base** — For something like an OpenTherm bridge: device registers, MQTT topics, firmware versions, known boiler quirks. Type: `Device`, `Register Map`, `Playbook`. Agents debugging field issues read the bundle first.

16. **Data catalog in OKF** — Export or enrich your warehouse (BigQuery, Postgres) into table/view concepts with schemas and join paths — Google's reference agent does exactly this. Shareable across tools without a proprietary catalog.

17. **API & integration map** — Endpoints, auth patterns, rate limits, deprecation notices. Type: `API Endpoint`. Links to the services and runbooks that depend on them.

18. **Vendor & subscription registry** — SaaS tools, renewal dates, owners, SSO setup. Type: `Vendor`, `Contract`. Business ops + security audits from one graph.

19. **Due diligence / acquisition dossier** — For buying a business or property: financial concepts, risk flags, open questions, document citations. Type: `Risk`, `Financial Statement`, `Open Question`. Compounds over weeks of research.

20. **Portable "brain export" for any new AI tool** — Maintain one OKF bundle as your canonical context. Switch from ChatGPT to Gemini to Claude Code without re-uploading PDFs — point the new agent at the repo. That's OKF's main strategic value: **knowledge as a durable asset**, not chat history.

---

## Practical Starting Point

Pick **one high-friction domain** where you repeatedly explain the same context (your business model, a technical project, or a personal area like health or home). Structure it roughly like:

```
my-knowledge/
├── index.md
├── log.md
├── business/          # or personal/, project/
│   ├── index.md
│   ├── products/
│   └── playbooks/
└── references/        # optional: external citations
```

Start with 5–10 concept pages, one ingest workflow ("drop source in `raw/`, run ingest"), and weekly lint. Scale when the index stops being enough (~100+ pages → add search like [qmd](https://github.com/tobi/qmd)).
