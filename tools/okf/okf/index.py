from __future__ import annotations

from pathlib import Path

from okf.bundle import RESERVED_FILENAMES, concept_dirs, load_concepts


def _dir_display_name(name: str) -> str:
    aliases = {
        "esp-idf": "ESP-IDF",
        "esp32": "ESP32",
        "opentherm": "OpenTherm",
        "zigbee": "Zigbee",
        "bridge": "Bridge",
    }
    return aliases.get(name, name.replace("-", " ").title())


def _link_for_concept(bundle_root: Path, concept_path: Path) -> str:
    rel = concept_path.relative_to(bundle_root).as_posix()
    return rel


def _subdir_link(bundle_root: Path, directory: Path) -> str:
    rel = directory.relative_to(bundle_root).as_posix()
    return f"{rel}/"


def render_directory_index(bundle_root: Path, directory: Path) -> str:
    concepts = [
        c
        for c in load_concepts(bundle_root)
        if c.path.parent == directory
    ]
    subdirs = sorted(
        {
            child
            for child in directory.iterdir()
            if child.is_dir() and any(child.rglob("*.md"))
        },
        key=lambda p: p.name,
    )

    lines: list[str] = []
    if directory == bundle_root:
        lines.append("# Knowledge Bundle")
        lines.append("")
        lines.append(
            "Compiled Open Knowledge Format concepts for the esp32-c6-opentherm project."
        )
        lines.append("")

    if subdirs:
        heading = "Directories" if directory == bundle_root else directory.name.title()
        lines.append(f"# {heading}")
        lines.append("")
        for subdir in subdirs:
            name = _dir_display_name(subdir.name)
            lines.append(f"* [{name}]({_subdir_link(bundle_root, subdir)})")
        lines.append("")

    if concepts:
        heading = "Concepts" if directory != bundle_root else "Root Concepts"
        if directory != bundle_root:
            heading = _dir_display_name(directory.name)
        lines.append(f"# {heading}")
        lines.append("")
        for concept in sorted(concepts, key=lambda c: c.title.lower()):
            link = _link_for_concept(bundle_root, concept.path)
            desc = concept.description or concept.type
            lines.append(f"* [{concept.title}]({link}) - {desc}")
        lines.append("")

    return "\n".join(lines).rstrip() + "\n"


def regenerate_indexes(bundle_root: Path) -> list[Path]:
    bundle_root = bundle_root.resolve()
    updated: list[Path] = []

    for directory in concept_dirs(bundle_root):
        index_path = directory / "index.md"
        if directory == bundle_root or any(directory.rglob("*.md")):
            content = render_directory_index(bundle_root, directory)
            index_path.write_text(content, encoding="utf-8")
            updated.append(index_path)

    return updated
