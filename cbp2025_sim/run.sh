#!/bin/bash

VARIANT=$1

if [ -z "$VARIANT" ]; then
    echo "Usage: $0 <folder-name>"
    exit 1
fi

CODE_DIR="predictors/$VARIANT"
RESULT_DIR="results/$VARIANT"

# Create results directory
mkdir -p "$RESULT_DIR"

# Copy predictor files into root
cp "$CODE_DIR"/*.cc .
cp "$CODE_DIR"/*.h .

# Rebuild
make clean && make

# Run experiment
python scripts/trace_exec_training_list.py \
    --trace_dir cbp_traces/ \
    --results_dir "$RESULT_DIR"