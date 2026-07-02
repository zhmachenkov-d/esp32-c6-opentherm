from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path

from okf.bundle import FRONTMATTER_RE, concept_dirs, iter_markdown_files, parse_markdown
from okf.index import regenerate_indexes, render_directory_index
from okf.validate import LINK_RE, ValidationResult, _is_bundle_link, _resolve_bundle_link, validate_bundle


@dataclass
class LintResult:
    errors: list[str] = field(default_factory=list)
    warnings: list[str] = field(default_factory=list)
    fixable: list[str] = field(default_factory=list)
    fixes_applied: list[str] = field(default_factory=list)

    @property
    def ok(self) -> bool:
        return not self.errors

    def merge_validation(self, validation: ValidationResult) -> None:
        self.errors.extend(validation.errors)
        self.warnings.extend(validation.warnings)


def _bundle_link_target(bundle_root: Path, concept_path: Path) -> str:
    return "/" + concept_path.relative_to(bundle_root).as_posix()


def _concepts_by_basename(bundle_root: Path) -> dict[str, list[Path]]:
    by_name: dict[str, list[Path]] = {}
    for path in iter_markdown_files(bundle_root):
        by_name.setdefault(path.name, []).append(path)
    return by_name


def _check_citations(bundle_root: Path, result: LintResult) -> None:
    for path in iter_markdown_files(bundle_root):
        rel = path.relative_to(bundle_root).as_posix()
        frontmatter, body = parse_markdown(path)
        if frontmatter is None:
            continue
        raw_paths = frontmatter.get("raw")
        if not raw_paths:
            continue
        if not isinstance(raw_paths, list):
            continue
        for raw_path in raw_paths:
            raw_text = str(raw_path)
            if raw_text not in body:
                result.warnings.append(
                    f"{rel}: raw source not cited in body: {raw_text}"
                )


def _check_index_staleness(bundle_root: Path, result: LintResult) -> None:
    for directory in concept_dirs(bundle_root):
        index_path = directory / "index.md"
        if directory != bundle_root and not any(directory.rglob("*.md")):
            continue
        expected = render_directory_index(bundle_root, directory)
        if not index_path.exists():
            rel = index_path.relative_to(bundle_root).as_posix()
            result.warnings.append(f"{rel}: missing index")
            result.fixable.append(f"{rel}: regenerate index")
            continue
        actual = index_path.read_text(encoding="utf-8")
        if actual != expected:
            rel = index_path.relative_to(bundle_root).as_posix()
            result.warnings.append(f"{rel}: stale index")
            result.fixable.append(f"{rel}: regenerate index")


def _find_fixable_links(bundle_root: Path) -> list[tuple[Path, str, str]]:
    fixes: list[tuple[Path, str, str]] = []
    by_basename = _concepts_by_basename(bundle_root)

    for path in iter_markdown_files(bundle_root):
        _, body = parse_markdown(path)
        if body is None:
            continue
        for match in LINK_RE.finditer(body):
            target = match.group(1).strip()
            if not _is_bundle_link(target):
                continue
            resolved = _resolve_bundle_link(bundle_root, path, target)
            if resolved is not None and resolved.exists():
                continue
            basename = Path(target).name
            matches = by_basename.get(basename, [])
            if len(matches) != 1:
                continue
            correct = _bundle_link_target(bundle_root, matches[0])
            if correct == target:
                continue
            fixes.append((path, target, correct))
    return fixes


def _apply_link_fixes(bundle_root: Path, fixes: list[tuple[Path, str, str]]) -> list[str]:
    applied: list[str] = []
    by_path: dict[Path, list[tuple[str, str]]] = {}
    for path, old_target, new_target in fixes:
        by_path.setdefault(path, []).append((old_target, new_target))

    for path, replacements in by_path.items():
        text = path.read_text(encoding="utf-8")
        match = FRONTMATTER_RE.match(text)
        if not match:
            continue
        prefix = text[: match.end()]
        body = text[match.end() :]
        new_body = body
        rel = path.relative_to(bundle_root).as_posix()
        for old_target, new_target in replacements:
            old_fragment = f"]({old_target})"
            new_fragment = f"]({new_target})"
            if old_fragment not in new_body:
                continue
            new_body = new_body.replace(old_fragment, new_fragment, 1)
            applied.append(f"{rel}: link {old_target} -> {new_target}")
        if new_body != body:
            path.write_text(prefix + new_body, encoding="utf-8")
    return applied


def lint_bundle(
    bundle_root: Path,
    repo_root: Path | None = None,
    *,
    fix: bool = False,
) -> LintResult:
    bundle_root = bundle_root.resolve()
    result = LintResult()

    validation = validate_bundle(bundle_root, repo_root=repo_root)
    result.merge_validation(validation)
    _check_citations(bundle_root, result)
    _check_index_staleness(bundle_root, result)

    link_fixes = _find_fixable_links(bundle_root)
    for path, old_target, new_target in link_fixes:
        rel = path.relative_to(bundle_root).as_posix()
        result.fixable.append(f"{rel}: link {old_target} -> {new_target}")

    if fix:
        if link_fixes:
            result.fixes_applied.extend(_apply_link_fixes(bundle_root, link_fixes))
        stale_indexes = [
            item
            for item in result.fixable
            if item.endswith(": regenerate index")
        ]
        if stale_indexes:
            updated = regenerate_indexes(bundle_root)
            for index_path in updated:
                rel = index_path.relative_to(bundle_root).as_posix()
                result.fixes_applied.append(f"{rel}: regenerated index")
        result.fixable.clear()

        follow_up = validate_bundle(bundle_root, repo_root=repo_root)
        result.errors = follow_up.errors
        result.warnings = follow_up.warnings
        _check_citations(bundle_root, result)
        _check_index_staleness(bundle_root, result)
        link_fixes = _find_fixable_links(bundle_root)
        for path, old_target, new_target in link_fixes:
            rel = path.relative_to(bundle_root).as_posix()
            result.fixable.append(f"{rel}: link {old_target} -> {new_target}")

    return result
