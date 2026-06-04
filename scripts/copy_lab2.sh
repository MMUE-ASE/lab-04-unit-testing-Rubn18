#!/usr/bin/env bash
# copy_lab2.sh — Copy your Lab 2 driver files into this Lab 4 workspace.
#
# Usage: bash scripts/copy_lab2.sh <path-to-your-lab2-repo>
set -euo pipefail

LAB2_PATH="${1:-}"

if [[ -z "$LAB2_PATH" ]]; then
    echo "Usage: bash scripts/copy_lab2.sh <path-to-lab2-repo>"
    echo ""
    echo "  Example: bash scripts/copy_lab2.sh ../lab2-make-build-system-myuser"
    exit 1
fi

if [[ ! -d "$LAB2_PATH" ]]; then
    echo "ERROR: '$LAB2_PATH' is not a directory."
    exit 1
fi

if [[ ! -f "$LAB2_PATH/src/gpio.c" || ! -f "$LAB2_PATH/inc/gpio.h" ]]; then
    echo "ERROR: '$LAB2_PATH' does not look like a Lab 2 repo (missing src/gpio.c or inc/gpio.h)."
    exit 1
fi

echo "Copying from $LAB2_PATH ..."
cp "$LAB2_PATH/src/gpio.c"  src/gpio.c
cp "$LAB2_PATH/inc/gpio.h"  inc/gpio.h
cp "$LAB2_PATH/inc/board.h" inc/board.h
cp "$LAB2_PATH/inc/rcc.h"   inc/rcc.h

echo "Done. Files copied:"
echo "  src/gpio.c"
echo "  inc/gpio.h  inc/board.h  inc/rcc.h"
echo ""
echo "Commit the driver, then run 'make test' to run all tests."
