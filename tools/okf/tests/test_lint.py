from pathlib import Path

from okf.lint import lint_bundle


def _write_concept(path: Path, body: str, *, raw: list[str] | None = None) -> None:
    raw_block = ""
    if raw:
        raw_lines = "\n".join(f"  - {item}" for item in raw)
        raw_block = f"raw:\n{raw_lines}\n"
    path.write_text(
        f"""---
type: Reference
title: Test Concept
description: Test.
tags: [test]
timestamp: 2026-07-02T00:00:00Z
{raw_block}---

{body}
""",
        encoding="utf-8",
    )


def test_lint_clean_knowledge_bundle() -> None:
    repo_root = Path(__file__).resolve().parents[3]
    bundle_root = repo_root / "knowledge"
    result = lint_bundle(bundle_root, repo_root=repo_root)
    assert result.ok, result.errors
    assert not result.fixable


def test_lint_detects_stale_index(tmp_path: Path) -> None:
    bundle_root = tmp_path / "bundle"
    topic = bundle_root / "topic"
    topic.mkdir(parents=True)
    _write_concept(topic / "alpha.md", "Alpha body.\n")
    (bundle_root / "index.md").write_text("# Wrong\n", encoding="utf-8")
    (topic / "index.md").write_text("# Wrong\n", encoding="utf-8")

    result = lint_bundle(bundle_root)

    assert any("stale index" in warning for warning in result.warnings)
    assert result.fixable
    fixed = lint_bundle(bundle_root, fix=True)
    assert fixed.ok
    assert any("regenerated index" in item for item in fixed.fixes_applied)
    assert not any("stale index" in warning for warning in fixed.warnings)


def test_lint_fixes_unique_basename_link(tmp_path: Path) -> None:
    bundle_root = tmp_path / "bundle"
    topic = bundle_root / "topic"
    topic.mkdir(parents=True)
    _write_concept(topic / "target.md", "Target body.\n")
    _write_concept(
        topic / "source.md",
        "See [target](/topic/wrong-path/target.md).\n",
    )
    (bundle_root / "index.md").write_text("# Bundle\n", encoding="utf-8")
    (topic / "index.md").write_text("# Topic\n", encoding="utf-8")

    result = lint_bundle(bundle_root)
    assert any("broken link" in warning for warning in result.warnings)
    assert result.fixable

    fixed = lint_bundle(bundle_root, fix=True)
    source_text = (topic / "source.md").read_text(encoding="utf-8")
    assert "/topic/target.md" in source_text
    assert any("link /topic/wrong-path/target.md" in item for item in fixed.fixes_applied)


def test_lint_warns_missing_citation(tmp_path: Path) -> None:
    bundle_root = tmp_path / "bundle"
    topic = bundle_root / "topic"
    topic.mkdir(parents=True)
    _write_concept(
        topic / "alpha.md",
        "No citations section.\n",
        raw=["wiki/raw/topic/source.md"],
    )
    (bundle_root / "index.md").write_text("# Bundle\n", encoding="utf-8")

    result = lint_bundle(bundle_root, repo_root=tmp_path)

    assert any("raw source not cited" in warning for warning in result.warnings)
