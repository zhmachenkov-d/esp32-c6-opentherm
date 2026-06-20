---
name: git-workflow
description: Git workflow for commits, branches, PRs, and history. Use when the user asks about git workflow, commit messages, PR titles or descriptions, pull request summary, staging, rebasing, squashing, or preparing changes for review.
---

# Git Workflow Rules

## When to use this skill

Apply when helping with **local Git operations** and **commit message wording** together—not only message format.

## Conventional Commits

Follow [Conventional Commits](https://www.conventionalcommits.org/) for **git commit subjects** and **PR titles** (same single-line shape):

```
<type>[optional scope]: <subject>
```

- **Type** (required): lowercase. Common types: `feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `build`, `ci`, `chore`.
- **Scope** (optional): short noun after the type — e.g. `feat(opentherm):` — use when it clarifies the area changed.
- **Subject**: imperative mood ("add" not "added", "fix" not "fixed"), **no trailing period**.
- **First-line length**: target **≤ 72 characters** for commit subjects and PR titles.
- **Breaking change**: `!` after type/scope (e.g. `feat(opentherm)!:`) and/or footer `BREAKING CHANGE: …`.
- **Body** (optional): blank line after the subject; explain *why* or *how*; wrap long lines at ~72 characters.
- **Footer** (optional): issue refs (`Refs #123`, `Closes #45`), `Co-authored-by:`, etc.

**PR body** (separate from the title line): use these sections:

- **Summary** — what changed and why (1–3 sentences or bullets)
- **Test plan** — how the change was verified (checklist is fine)
- **Related issues** (optional) — `Closes #123`, `Refs PROJ-456`, etc., only when the user provides or clearly implies an issue key

Do not actively search issue trackers when no issue key or explicit context is present.

## Workflow: pull request title and description

1. **Inspect**: `git status`, `git diff` (and `git diff --staged` if anything is staged), and/or `git log` against the target branch (e.g. `main`).
2. **Title**: Conventional Commits style; imperative; no trailing period; **≤ 72 characters**.
3. **Description**: Markdown body with **Summary** and **Test plan**; call out breaking changes when applicable.
4. **Related issues**: add only when the user provides or clearly implies an issue key.

## Workflow: propose a commit

1. **Inspect**: `git status`, `git diff` (and `git diff --staged` if anything is staged).
2. **Stage intentionally**: stage coherent units; prefer **one logical change per commit** (split if the diff mixes concerns).
3. **Draft message** from the actual diff—not from intent alone.
4. **Verify** before commit: subject length (≤ 72), imperative mood, appropriate type.
5. **Format output**: subject on the first line, blank-line-separated body only when it adds useful context, no code fences.

## Workflow: fix last commit

- **Message only**: `git commit --amend` (or `--amend --no-edit` after adding forgotten files).
- **Avoid** amending commits already pushed to a shared branch unless the user explicitly wants a force-with-lease rewrite.

## Workflow: history cleanup (feature branch)

- Use **interactive rebase** only on **local or personal feature branches** before opening/updating a PR: reorder, squash fixups, reword messages to match Conventional Commits.
- Do not rewrite published `main`/`develop` history without team agreement.

## Workflow: PR-ready branch

- Rebase or merge from the target branch per **project convention**; resolve conflicts with small, clear commits or a single squashed commit if that is what the team expects.
- Push with `--force-with-lease` only when the user has rewritten history on a remote-backed branch.

## Quick checklist

- [ ] Type matches the change (`fix` vs `feat` vs `docs`, etc.).
- [ ] Scope helps disambiguation (omit if it adds noise).
- [ ] Subject first line ≤ 72 characters.
- [ ] Imperative subject, no trailing period.
- [ ] Body present when "why" is not obvious from the subject.

## Examples

**Feature:**

```
feat(opentherm): add boiler status polling

Poll TBoiler every 1s via OpenTherm data ID 25; log on change only.
```

**Bug fix:**

```
fix(wifi): retry connection after DHCP timeout
```

**Breaking change:**

```
feat(opentherm)!: replace legacy status enum with bit flags

BREAKING CHANGE: status field is now a u16 bitmask; update all callers.
```

**Documentation:**

```
docs: add wiring diagram for OpenTherm adapter
```
