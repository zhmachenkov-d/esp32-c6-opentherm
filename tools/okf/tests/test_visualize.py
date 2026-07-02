from pathlib import Path

from okf.visualize import generate_visualization


def test_visualize_knowledge_bundle(tmp_path: Path) -> None:
    repo_root = Path(__file__).resolve().parents[3]
    bundle_root = repo_root / "knowledge"
    out_path = tmp_path / "knowledge-viz.html"

    stats = generate_visualization(bundle_root, out_path, bundle_name="Knowledge")

    assert out_path.is_file()
    html = out_path.read_text(encoding="utf-8")
    assert "cytoscape" in html
    assert "OpenTherm Protocol" in html
    assert stats["concepts"] >= 20
    assert stats["edges"] > 0
