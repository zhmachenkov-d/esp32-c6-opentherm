from __future__ import annotations

import json
import tarfile
from datetime import datetime, timezone
from pathlib import Path

from okf import __version__
from okf.bundle import load_concepts


def build_manifest(bundle_root: Path) -> dict:
    bundle_root = bundle_root.resolve()
    concepts = load_concepts(bundle_root)
    return {
        "okf_version": "0.1",
        "exported_at": datetime.now(timezone.utc).isoformat(),
        "tool_version": __version__,
        "bundle_name": bundle_root.name,
        "concept_count": len(concepts),
        "concepts": [
            {
                "id": concept.concept_id,
                "type": concept.type,
                "title": concept.title,
                "description": concept.description,
                "tags": concept.frontmatter.get("tags", []),
                "path": f"/{concept.concept_id}.md",
            }
            for concept in sorted(concepts, key=lambda c: c.concept_id)
        ],
    }


def export_bundle(bundle_root: Path, output_dir: Path) -> tuple[Path, Path]:
    bundle_root = bundle_root.resolve()
    output_dir = output_dir.resolve()
    output_dir.mkdir(parents=True, exist_ok=True)

    manifest = build_manifest(bundle_root)
    manifest_path = output_dir / f"{bundle_root.name}-manifest.json"
    manifest_path.write_text(
        json.dumps(manifest, indent=2, ensure_ascii=False) + "\n",
        encoding="utf-8",
    )

    archive_path = output_dir / f"{bundle_root.name}.tar.gz"
    with tarfile.open(archive_path, "w:gz") as tar:
        for path in sorted(bundle_root.rglob("*")):
            if path.is_file():
                arcname = path.relative_to(bundle_root.parent).as_posix()
                tar.add(path, arcname=arcname)

    return archive_path, manifest_path
