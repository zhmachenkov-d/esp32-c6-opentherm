from __future__ import annotations

import json
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from okf.bundle import load_concepts
from okf.validate import LINK_RE, _resolve_bundle_link

_TYPE_PALETTE = {
    "Protocol": "#8b5cf6",
    "Reference": "#10b981",
    "Hardware": "#f59e0b",
    "Data ID": "#3b82f6",
    "Attribute": "#ec4899",
    "Playbook": "#ef4444",
    "BigQuery Dataset": "#8b5cf6",
    "BigQuery Table": "#3b82f6",
}
_DEFAULT_NODE_COLOR = "#94a3b8"


@dataclass
class GraphConcept:
    id: str
    type: str
    title: str
    description: str
    resource: str
    tags: list[str]
    body: str
    links_to: list[str] = field(default_factory=list)

    def to_node(self) -> dict[str, Any]:
        color = _TYPE_PALETTE.get(self.type, _DEFAULT_NODE_COLOR)
        return {
            "data": {
                "id": self.id,
                "label": self.title or self.id,
                "type": self.type,
                "description": self.description,
                "resource": self.resource,
                "tags": self.tags,
                "color": color,
                "size": 30 + min(60, len(self.body) // 200),
            }
        }


def _concept_id_from_path(bundle_root: Path, path: Path) -> str:
    rel = path.relative_to(bundle_root).with_suffix("")
    return "/".join(rel.parts)


def _extract_links(body: str, bundle_root: Path, source: Path) -> list[str]:
    out: list[str] = []
    seen: set[str] = set()
    for match in LINK_RE.finditer(body):
        target = match.group(1).strip()
        resolved = _resolve_bundle_link(bundle_root, source, target)
        if resolved is None or not resolved.exists() or resolved.suffix != ".md":
            continue
        try:
            concept_id = _concept_id_from_path(bundle_root, resolved)
        except ValueError:
            continue
        if concept_id and concept_id not in seen:
            seen.add(concept_id)
            out.append(concept_id)
    return out


def _walk_concepts(bundle_root: Path) -> list[GraphConcept]:
    concepts: list[GraphConcept] = []
    for concept in load_concepts(bundle_root):
        frontmatter = concept.frontmatter
        tags = frontmatter.get("tags") or []
        if not isinstance(tags, list):
            tags = [str(tags)]
        concepts.append(
            GraphConcept(
                id=concept.concept_id,
                type=concept.type or "Unknown",
                title=concept.title,
                description=concept.description,
                resource=str(frontmatter.get("resource") or ""),
                tags=[str(tag) for tag in tags],
                body=concept.body,
                links_to=_extract_links(concept.body, bundle_root, concept.path),
            )
        )
    return concepts


def _build_graph(concepts: list[GraphConcept]) -> dict[str, Any]:
    ids = {concept.id for concept in concepts}
    nodes = [concept.to_node() for concept in concepts]
    edges: list[dict[str, Any]] = []
    seen_edges: set[tuple[str, str]] = set()
    for concept in concepts:
        for target in concept.links_to:
            if target == concept.id or target not in ids:
                continue
            key = (concept.id, target)
            if key in seen_edges:
                continue
            seen_edges.add(key)
            edges.append(
                {
                    "data": {
                        "id": f"{concept.id}__{target}",
                        "source": concept.id,
                        "target": target,
                    }
                }
            )
    bodies = {concept.id: concept.body for concept in concepts}
    types = sorted({concept.type for concept in concepts})
    return {
        "nodes": nodes,
        "edges": edges,
        "bodies": bodies,
        "types": types,
        "palette": _TYPE_PALETTE,
    }


def _viewer_asset(name: str) -> str:
    asset_path = Path(__file__).parent / "viewer" / name
    return asset_path.read_text(encoding="utf-8")


def generate_visualization(
    bundle_root: Path,
    out_path: Path,
    *,
    bundle_name: str | None = None,
) -> dict[str, int]:
    """Walk a bundle and write a single self-contained HTML visualization."""
    bundle_root = bundle_root.resolve()
    out_path = out_path.resolve()
    if not bundle_root.is_dir():
        raise FileNotFoundError(f"Bundle directory not found: {bundle_root}")

    concepts = _walk_concepts(bundle_root)
    graph = _build_graph(concepts)
    template = _viewer_asset("templates/viz.html")
    css = _viewer_asset("static/viz.css")
    js = _viewer_asset("static/viz.js")
    name = bundle_name or bundle_root.name

    html = (
        template.replace("/*__VIZ_CSS__*/", css)
        .replace("/*__VIZ_JS__*/", js)
        .replace("__BUNDLE_NAME__", json.dumps(name))
        .replace("__BUNDLE_DATA__", json.dumps(graph))
    )
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(html, encoding="utf-8")

    return {
        "concepts": len(concepts),
        "edges": len(graph["edges"]),
        "bytes": len(html.encode("utf-8")),
    }
