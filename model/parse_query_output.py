import sys

flags = ["-O0", "-O1", "-O2", "-O3", "-Os", "-Oz", "-Ofast", "-Og", "-fno-strict-aliasing", "-fstrict-aliasing", "-fno-builtin", "-ffast-math", "-ffinite-math-only", "-fno-signed-zeros", "-fno-trapping-math", "-ftrapping-math", "-funsafe-math-optimizations", "-fassociative-math", "-fno-math-errno", "-fno-rounding-math", "-freciprocal-math", "-fno-semantic-interposition", "-ffast-math", "-finline-functions", "-finline-hint-functions", "-fmerge-all-constants", "-fprofile-sample-use", "-fprofile-use", "-freciprocal-math", "-ftree-slp-vectorize", "-ftree-vectorize", "-funroll-loops", "-fvectorize", "-fno-math-errno", "-fno-trapping-math", "-fassociative-math", "-ffinite-math-only", "-fno-signed-zeros", "-ftrapping-math", "-fwrapv", "-fno-stack-protector", "-fstack-protector", "-fstack-protector-all", "-fstack-protector-strong", "-fno-stack-clash-protection", "-fsanitize=address", "-fsanitize=thread", "-fsanitize=memory", "-fsanitize=undefined", "-fsanitize=leak", "-fno-sanitize=address", "-fno-sanitize=thread", "-fno-sanitize=memory", "-fno-sanitize=undefined", "-fno-sanitize=leak", "-fsanitize-address-use-after-scope", "-fno-sanitize-address-use-after-scope", "-fsanitize-memory-track-origins", "-fno-sanitize-memory-track-origins", "-fsanitize-memory-use-after-dtor", "-fno-sanitize-memory-use-after-dtor", "-fsanitize-undefined-trap-on-error", "-fno-sanitize-undefined-trap-on-error", "-freciprocal-math", "-fno-semantic-interposition", "-fno-signed-zeros", "-fno-trapping-math", "-fwrapv", "-fomit-frame-pointer", "-fno-omit-frame-pointer"]


if len(sys.argv) != 2:
    print("Error: insufficient arguments for parse_query_output.py")
    sys.exit(1)

# open the input file holding the query result (from the LLM)
file_name = sys.argv[1]
output_path = "output/" + file_name
file = open(output_path)
final_flags = []

# iterate through the input and identify valid compiler optimization flags
for line in file.readlines():
    words = line.split()
    for word in words:
        if word[0] == '-' and (word in flags):
            final_flags.append(word)

# return the valid suggested flags
output = ""
for flag in final_flags:
    output += flag + " "
print(output)
