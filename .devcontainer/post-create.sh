#!/usr/bin/env bash
set -euo pipefail

git config --global --add safe.directory "${containerWorkspaceFolder:-/workspaces/esp32-c6-opentherm}"

source /opt/esp/idf/export.sh

if [[ -f CMakeLists.txt ]]; then
  idf.py set-target esp32c6
fi

echo "ESP-IDF: $(idf.py --version)"
echo "Default target: esp32c6 (set via idf.py set-target when CMakeLists.txt exists)"

if command -v gh >/dev/null 2>&1; then
  echo "GitHub CLI: $(gh --version)"
  if [[ -n "${GH_TOKEN:-}" ]]; then
    gh auth status 2>&1 || true
  else
    echo "GH_TOKEN: not set (copy .env.example to .env and rebuild for gh push)"
  fi
else
  echo "GitHub CLI: not installed (rebuild dev container to apply features)"
fi
