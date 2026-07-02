from __future__ import annotations

import re
from dataclasses import dataclass, field
from pathlib import Path

from okf.bundle import RESERVED_FILENAMES, iter_markdown_files, parse_markdown

LINK_RE = re.compile(r"\[[^\]]*\]\(([^)]+)\)")


@dataclass
class ValidationResult:
    errors: list[str] = field(default_factory=list)
    warnings: list[str] = field(default_factory=list)

    @property
    def ok(self) -> bool:
        return not self.errors


def _is_bundle_link(target: str) -> bool:
    if target.startswith(("http://", "https://", "#")):
        return False
    if target.startswith("wiki/"):
        return False
    return target.endswith(".md") or target.endswith("/")


def _resolve_bundle_link(bundle_root: Path, source: Path, target: str) -> Path | None:
    if not _is_bundle_link(target):
        return None
    if target.startswith("/"):
        return bundle_root / target.lstrip("/")
    return (source.parent / target).resolve()


def validate_bundle(bundle_root: Path, repo_root: Path | None = None) -> ValidationResult:
    bundle_root = bundle_root.resolve()
    result = ValidationResult()

    if not bundle_root.is_dir():
        result.errors.append(f"Bundle path does not exist: {bundle_root}")
        return result

    for path in bundle_root.rglob("*.md"):
        rel = path.relative_to(bundle_root).as_posix()
        if path.name in RESERVED_FILENAMES:
            continue

        frontmatter, body = parse_markdown(path)
        if frontmatter is None:
            result.errors.append(f"{rel}: missing YAML frontmatter")
            continue

        concept_type = frontmatter.get("type")
        if not concept_type or not str(concept_type).strip():
            result.errors.append(f"{rel}: missing required frontmatter field 'type'")

        raw_paths = frontmatter.get("raw", [])
        if raw_paths and repo_root:
            if not isinstance(raw_paths, list):
                result.errors.append(f"{rel}: 'raw' must be a YAML list")
            else:
                for raw_path in raw_paths:
                    candidate = repo_root / str(raw_path)
                    if not candidate.exists():
                        result.errors.append(
                            f"{rel}: raw source not found: {raw_path}"
                        )

        for match in LINK_RE.finditer(body):
            target = match.group(1).strip()
            resolved = _resolve_bundle_link(bundle_root, path, target)
            if resolved is None:
                continue
            if resolved.suffix == ".md" and not resolved.exists():
                result.warnings.append(
                    f"{rel}: broken link to {target}"
                )

    for reserved in RESERVED_FILENAMES:
        reserved_paths = list(bundle_root.rglob(reserved))
        for path in reserved_paths:
            frontmatter, _ = parse_markdown(path)
            if frontmatter is not None:
                rel = path.relative_to(bundle_root).as_posix()
                result.warnings.append(
                    f"{rel}: reserved file should not have frontmatter"
                )

    return result
