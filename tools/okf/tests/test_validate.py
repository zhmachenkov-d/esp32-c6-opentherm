from pathlib import Path

from okf.validate import validate_bundle


def test_validate_opentherm_spike(tmp_path: Path) -> None:
    repo_root = Path(__file__).resolve().parents[3]
    bundle_root = repo_root / "knowledge"
    result = validate_bundle(bundle_root, repo_root=repo_root)
    assert result.ok, result.errors
