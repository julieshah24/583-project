#!/bin/bash
# Run script for Group 2 EECS 583 W24 Final Project
# Usage: sh run_all.sh

PATH2LIB="./code_analysis/build/analysis_pass/AnalysisPass.so"
PASS=analysis

mkdir -p model/output
mkdir -p model/prompts

# Delete any outputs from previous runs
rm -f default.profraw *_prof *_fplicm *.bc *.profdata *_output *.ll *.in *.in.Z

# Evaluation loop runs over each test file
for file in ./code_analysis/dataset/src/*
do
    # filename_dot_c=$(basename "$file")  # Extract filename without path
    # filename="${filename_dot_c%.*}"     # Remove file extension
    # echo "${filename}"
    # echo "${file}"

    # # Creates a link to the input file in the local directory.
    # # Not adding this can sometimes alter results.
    # if [ "${filename}" = "anagram" ]; then 
    # ln -sf code_analysis/dataset/input/input.in
    # ln -sf code_analysis/dataset/input/words
    # elif [ "${filename}" = "compress" ];then
    # ln -sf code_analysis/dataset/input/compress.in 
    # fi

    # mkdir -p model/prompts/program # To store stats for each function

    # # Convert source code to bitcode (IR).
    # clang -emit-llvm -c ${file} -Xclang -disable-O0-optnone -o ${filename}.bc -Wno-deprecated-non-prototype

    # # Instrument profiler passes. Generates profile data.
    # opt -passes='pgo-instr-gen,instrprof' ${filename}.bc -o ${filename}.prof.bc

    # # Generate binary executable with profiler embedded
    # clang -fprofile-instr-generate ${filename}.prof.bc -o ${filename}_prof

    # # When we run the profiler embedded executable, it generates a default.profraw file that contains the profile data.
    # if [ "${filename}" = "anagram" ]; then 
    # ./${filename}_prof words < input.in > /dev/null 2>&1
    # elif [ "${filename}" = "compress" ];then
    # ./${filename}_prof compress.in > /dev/null 
    # else
    # ./${filename}_prof > /dev/null
    # fi

    # # Converting it to LLVM form. This step can also be used to combine multiple profraw files,
    # # in case you want to include different profile runs together.
    # llvm-profdata merge -o ${filename}.profdata default.profraw

    # # The "Profile Guided Optimization Instrumentation-Use" pass attaches the profile data to the bc file.
    # opt -passes="pgo-instr-use" -o ${filename}.profdata.bc -pgo-test-profile-file=${1}.profdata < ${filename}.bc

    # # Uncomment this and disable the cleanup if you want to "see" the instumented IR.
    # # llvm-dis ${filename}.profdata.bc -o ${filename}.prof.ll

    # # Runs pass on the instrumented code.
    # opt --disable-output -load-pass-plugin="${PATH2LIB}" -passes="${PASS}" ${filename}.profdata.bc

    # # Cleanup: Remove this if you want to retain the created files.
    # rm -f *.in *.in.Z default.profraw *_prof *_fplicm *.bc *.profdata *_output *.ll words

    # mv model/prompts/program model/prompts/${filename} # rename output folder

    # python3 create_prompt.py ${filename} # generate prompt
    # rm -rf model/prompts/${filename}

    # cd model && python3 query.py ${filename}.txt # run model

    # python3 parse_query_output.py ${filename}.txt > final_flags.txt
    # FLAGS=$(cat final_flags.txt)
    # echo "$FLAGS"
    # rm -rf final_flags.txt

    # # Get instruction counts
    echo "O2 baseline: "
    # clang -O2 -o out ../code_analysis/dataset/src/${filename}.c
    clang -O2 -o out ${file} 
    llvm-objdump -d out | grep -cE '^\s+[a-f0-9]+:\s+[0-9a-f]+(\s+[0-9a-f]+)?\s+\w+\s+'
    echo "O3 baseline: "
    # clang -O3 -o out ../code_analysis/dataset/src/${filename}.c
    clang -O3 -o out ${file} 2>/dev/null
    llvm-objdump -d out | grep -cE '^\s+[a-f0-9]+:\s+[0-9a-f]+(\s+[0-9a-f]+)?\s+\w+\s+'
    # echo "Our flag suggestions: "
    # clang ${FLAGS} -o out ../code_analysis/dataset/src/${filename}.c
    # llvm-objdump -d out | grep -cE '^\s+[a-f0-9]+:\s+[0-9a-f]+(\s+[0-9a-f]+)?\s+\w+\s+'
done
