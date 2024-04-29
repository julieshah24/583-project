#!/bin/bash
# Evaluate script
# This runs all stages of the project and prints metrics to a running CSV file
# This can run on its own but is built to be called within evaluate_all_csv.sh
# Usage: sh evaluate_csv.sh <benchmark_name>
# E.g:   sh evaluate_csv.sh simple

PATH2LIB="./code_analysis/build/analysis_pass/AnalysisPass.so"
PASS=analysis

BENCH=code_analysis/dataset/src/${1}.c

# Delete outputs from previous runs. Update this if you want to retain some files across runs.
rm -f default.profraw *_prof *_fplicm *.bc *.profdata *_output *.ll *.in *.in.Z

# Creates a link to the input file in the local directory.
# Not adding this can sometimes alter results.
if [ "${1}" = "anagram" ]; then 
ln -sf code_analysis/dataset/input/input.in
ln -sf code_analysis/dataset/input/words
elif [ "${1}" = "compress" ];then
ln -sf code_analysis/dataset/input/compress.in 
fi

mkdir -p model/output
mkdir -p model/prompts
mkdir model/prompts/program # To store stats for each function


# Convert source code to bitcode (IR).
clang -emit-llvm -c ${BENCH} -Xclang -disable-O0-optnone -o ${1}.bc -Wno-deprecated-non-prototype

start=$(date +%s) # Starting LLM compiler analysis

# Instrument profiler passes. Generates profile data.
opt -passes='pgo-instr-gen,instrprof' ${1}.bc -o ${1}.prof.bc

# Generate binary executable with profiler embedded
clang -fprofile-instr-generate ${1}.prof.bc -o ${1}_prof

# When we run the profiler embedded executable, it generates a default.profraw file that contains the profile data.
if [ "${1}" = "anagram" ]; then 
./${1}_prof words < input.in > /dev/null 2>&1
elif [ "${1}" = "compress" ];then
./${1}_prof compress.in > /dev/null 
else
./${1}_prof > /dev/null
fi

# Converting it to LLVM form. This step can also be used to combine multiple profraw files,
# in case you want to include different profile runs together.
llvm-profdata merge -o ${1}.profdata default.profraw

# The "Profile Guided Optimization Instrumentation-Use" pass attaches the profile data to the bc file.
opt -passes="pgo-instr-use" -o ${1}.profdata.bc -pgo-test-profile-file=${1}.profdata < ${1}.bc

# Uncomment this and disable the cleanup if you want to "see" the instumented IR.
# llvm-dis ${1}.profdata.bc -o ${1}.prof.ll

# Runs your pass on the instrumented code.
opt --disable-output -load-pass-plugin="${PATH2LIB}" -passes="${PASS}" ${1}.profdata.bc

# Cleanup: Remove this if you want to retain the created files.
rm -f *.in *.in.Z default.profraw *_prof *_fplicm *.bc *.profdata *_output *.ll words

mv model/prompts/program model/prompts/${1} # rename output folder

python3 create_prompt.py ${1} # generate prompt
rm -rf model/prompts/${1}


cd model && python3 query.py ${1}.txt # run model

python3 parse_query_output.py ${1}.txt > final_flags.txt
# FLAGS=$(<final_flags.txt)
FLAGS=$(cat final_flags.txt)
echo "$FLAGS"
rm -rf final_flags.txt

end=$(date +%s) # ended LLM compiler analysis

# echo "Time taken: $((end-start)) seconds."
output_file="output.csv"

compile_and_time() {
    local opt_level=$1
    local src_file=$2
    local opt_level_escaped=$(echo "$opt_level" | tr ' ' '_')
    local exe_file="out_$opt_level_escaped"

    # Time compilation
    local compile_start=$(date +%s.%N)
    clang $opt_level -o $exe_file $src_file 2> /dev/null
    local compile_end=$(date +%s.%N)
    local compile_time=$(echo "$compile_end - $compile_start" | bc)

    # Count instructions
    local instruction_count=$(llvm-objdump -d $exe_file | grep -cE '^\s+[a-f0-9]+:\s+[0-9a-f]+(\s+[0-9a-f]+)?\s+\w+\s+')

    # Time execution
    local exec_start=$(date +%s.%N)
    # ./$exe_file
    local exec_end=$(date +%s.%N)
    local exec_time=$(echo "$exec_end - $exec_start" | bc)

    # echo "$instruction_count,$compile_time,$exec_time"
    echo "$instruction_count"
}

cd ..

# Compile and execute with O2 optimization
echo "Compiling and executing with O2 optimization..."
o2_results=$(compile_and_time "-O2" $BENCH)

# Compile and execute with O3 optimization
echo "Compiling and executing with O3 optimization..."
o3_results=$(compile_and_time "-O3" $BENCH)

# Compile and execute with custom flags
echo "Compiling and executing with custom flags..."
custom_results=$(compile_and_time "${FLAGS}" $BENCH)

rm out_*

# Write to CSV
echo "${1},${o2_results},${o3_results},${custom_results}, ${FLAGS}" >> "$output_file"