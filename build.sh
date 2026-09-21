#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ZEPHYR_BASE="${ZEPHYR_BASE:-/home/alialameh/zephyrproject/zephyr}"
BOARD_NAME="${1:-our_board}"
APP_DIR="${2:-$ROOT_DIR/blink}"
BUILD_DIR="${3:-$ROOT_DIR/blink/build_our_board}"

if [[ ! -d "$ZEPHYR_BASE" ]]; then
    echo "Zephyr base not found at $ZEPHYR_BASE" >&2
    exit 1
fi

if [[ ! -d "$APP_DIR" ]]; then
    echo "Application directory not found: $APP_DIR" >&2
    exit 1
fi

cd "$ZEPHYR_BASE"
west build "$APP_DIR" -b "$BOARD_NAME" -d "$BUILD_DIR" -DBOARD_ROOT="$ROOT_DIR"
