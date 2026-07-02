from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path

import yaml

RESERVED_FILENAMES = frozenset({"index.md", "log.md"})
FRONTMATTER_RE = re.compile(r"\A---\r?\n(.*?)\r?\n---\r?\n", re.DOTALL)


@dataclass
class Concept:
    path: Path
    concept_id: str
    frontmatter: dict
    body: str

    @property
    def type(self) -> str:
        return str(self.frontmatter.get("type", ""))

    @property
    def title(self) -> str:
        title = self.frontmatter.get("title")
        if title:
            return str(title)
        return self.path.stem.replace("-", " ").title()

    @property
    def description(self) -> str:
        return str(self.frontmatter.get("description", ""))


def is_reserved(path: Path) -> bool:
    return path.name in RESERVED_FILENAMES


def parse_markdown(path: Path) -> tuple[dict | None, str]:
    text = path.read_text(encoding="utf-8")
    match = FRONTMATTER_RE.match(text)
    if not match:
        return None, text
    frontmatter = yaml.safe_load(match.group(1)) or {}
    if not isinstance(frontmatter, dict):
        return None, text
    body = text[match.end() :]
    return frontmatter, body


def iter_markdown_files(bundle_root: Path) -> list[Path]:
    return sorted(
        path
        for path in bundle_root.rglob("*.md")
        if path.is_file() and not is_reserved(path)
    )


def load_concepts(bundle_root: Path) -> list[Concept]:
    concepts: list[Concept] = []
    for path in iter_markdown_files(bundle_root):
        frontmatter, body = parse_markdown(path)
        if frontmatter is None:
            continue
        rel = path.relative_to(bundle_root).as_posix()
        concept_id = rel[:-3] if rel.endswith(".md") else rel
        concepts.append(
            Concept(
                path=path,
                concept_id=concept_id,
                frontmatter=frontmatter,
                body=body,
            )
        )
    return concepts


def concept_dirs(bundle_root: Path) -> list[Path]:
    dirs = {bundle_root}
    for path in iter_markdown_files(bundle_root):
        dirs.add(path.parent)
    return sorted(dirs, key=lambda p: (len(p.parts), p.as_posix()))
