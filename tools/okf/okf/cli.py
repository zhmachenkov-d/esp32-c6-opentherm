from __future__ import annotations

import argparse
import sys
from pathlib import Path

from okf.export import export_bundle
from okf.index import regenerate_indexes
from okf.validate import validate_bundle


def _default_repo_root(bundle_root: Path) -> Path:
    candidate = bundle_root.resolve()
    for parent in [candidate, *candidate.parents]:
        if (parent / "CMakeLists.txt").exists():
            return parent
    return candidate.parent


def cmd_validate(args: argparse.Namespace) -> int:
    bundle_root = Path(args.bundle)
    repo_root = Path(args.repo_root) if args.repo_root else _default_repo_root(bundle_root)
    result = validate_bundle(bundle_root, repo_root=repo_root)

    for warning in result.warnings:
        print(f"warning: {warning}", file=sys.stderr)
    for error in result.errors:
        print(f"error: {error}", file=sys.stderr)

    if result.ok:
        print(f"ok: {bundle_root} is conformant ({len(result.warnings)} warnings)")
        return 0
    print(f"failed: {len(result.errors)} errors", file=sys.stderr)
    return 1


def cmd_index(args: argparse.Namespace) -> int:
    bundle_root = Path(args.bundle)
    updated = regenerate_indexes(bundle_root)
    for path in updated:
        print(f"updated: {path}")
    return 0


def cmd_export(args: argparse.Namespace) -> int:
    bundle_root = Path(args.bundle)
    output_dir = Path(args.output)
    archive_path, manifest_path = export_bundle(bundle_root, output_dir)
    print(f"archive: {archive_path}")
    print(f"manifest: {manifest_path}")
    return 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Open Knowledge Format bundle tooling")
    subparsers = parser.add_subparsers(dest="command", required=True)

    validate_parser = subparsers.add_parser("validate", help="Check OKF conformance")
    validate_parser.add_argument("bundle", help="Path to knowledge bundle root")
    validate_parser.add_argument(
        "--repo-root",
        help="Repository root for validating raw: provenance paths",
    )
    validate_parser.set_defaults(func=cmd_validate)

    index_parser = subparsers.add_parser("index", help="Regenerate index.md files")
    index_parser.add_argument("bundle", help="Path to knowledge bundle root")
    index_parser.set_defaults(func=cmd_index)

    export_parser = subparsers.add_parser("export", help="Export tarball and manifest")
    export_parser.add_argument("bundle", help="Path to knowledge bundle root")
    export_parser.add_argument(
        "-o",
        "--output",
        default="dist",
        help="Output directory (default: dist)",
    )
    export_parser.set_defaults(func=cmd_export)

    args = parser.parse_args(argv)
    return args.func(args)


if __name__ == "__main__":
    raise SystemExit(main())
