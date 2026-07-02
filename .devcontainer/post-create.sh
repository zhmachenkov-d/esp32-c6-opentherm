#!/usr/bin/env bash
set -euo pipefail

if [ -d /ssh ]; then
  mkdir -p /root/.ssh
  cp -r /ssh/. /root/.ssh/
  chown -R root:root /root/.ssh
  chmod 700 /root/.ssh
  find /root/.ssh -type f -exec chmod 600 {} \;
fi

git config --global --add safe.directory "${containerWorkspaceFolder:-/workspaces/esp32-c6-opentherm}"

source /opt/esp/idf/export.sh

if [[ -f CMakeLists.txt ]]; then
  idf.py set-target esp32c6
fi

echo "ESP-IDF: $(idf.py --version)"
echo "Default target: esp32c6 (set via idf.py set-target when CMakeLists.txt exists)"
