#!/bin/bash

set -e

if [ -z "$1" ]; then
    echo "Usage: ./run_config.sh <config_folder>"
    echo "Example: ./run_config.sh rv32im_tage_sc"
    exit 1
fi

CONFIG=$1

BASE_DIR=$(pwd)

SRC="$BASE_DIR/branch_predictor_softcore_versions/$CONFIG"
DEST="$BASE_DIR/rtl"

if [ ! -d "$SRC" ]; then
    echo "Error: folder does not exist: $SRC"
    exit 1
fi

echo "Replacing: $CONFIG"

rm -rf "$DEST"
cp -r "$SRC" "$DEST"