#!/bin/bash
# Run script for Group 2 EECS 583 W24 Final Project
# Usage: sh evaluate_all.sh

PATH2LIB="./code_analysis/build/analysis_pass/AnalysisPass.so"
PASS=analysis

counter=0

# Evaluation loop runs over each test file
for file in ./code_analysis/dataset/src/*.c
do
    # if [ $counter -le 1 ]; then
    #     counter=$((counter+1))
    #     continue
    # fi
    filename_dot_c=$(basename "$file")  # Extract filename without path
    filename="${filename_dot_c%.*}"     # Remove file extension
    echo "${filename}"

    sh evaluate_csv.sh $filename
done
