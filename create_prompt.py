import os
import sys

def create_prompt(folder_name):
    data = []
    executions = []
    # Iterate over files in the folder
    for filename in os.listdir("./model/prompts/" + folder_name):
        # Check if it's a text file
        if filename.endswith(".txt"):
            file_path = os.path.join("./model/prompts", folder_name, filename)
            # Open the file
            with open(file_path, 'r') as file:
                # Read file contents into a list
                function_data = file.read().split()
                data.append(function_data)
                executions.append(int(function_data[1]))

    # Determine which functions execute the most
    indices = list(range(len(executions)))
    sorted_indices = sorted(indices, key=lambda i: executions[i], reverse=True)

    # Write prompt to file
    out_file_name = "./model/prompts/" + folder_name + ".txt"
    with open(out_file_name, 'w') as out_file:
        out_file.write("Based on the following profile information presented for a .c file, name ideal real clang llvm optimization compilation flags separated by whitespace and nothing else. ")
        for rank in range(9):
            if rank < len(data):
                fdata = data[sorted_indices[rank]]
                func_name = str(fdata[0])
                func_freq = str(fdata[1])
                num_bb = str(fdata[2])
                avg_bb_exec = str(fdata[3])
                mem_acc = str(fdata[4])
                biased_br = str(fdata[5])
                unbiased_br = str(fdata[6])
                num_loops = str(fdata[7])
                avg_loop_exec = str(fdata[8])
                loop_cycles_per_iter = str(fdata[9])
                out_file.write(f"The function named {func_name} executes {func_freq} times. ")
                out_file.write(f"It has {num_bb} basic blocks, ") 
                out_file.write(f"where on average each basic block executes {avg_bb_exec} times, ") 
                out_file.write(f"has {mem_acc} memory accesses, ")
                out_file.write(f"{biased_br} biased branches, ")        
                out_file.write(f"and {unbiased_br} unbiased branches. ") 
                out_file.write(f"It has {num_loops} loops, ") 
                out_file.write(f"where on average each loop executes {avg_loop_exec} times ") 
                out_file.write(f"and has {loop_cycles_per_iter} cycles per loop iteration.") 


if __name__ == "__main__":
    # Check if the folder name is provided as a command-line argument
    if len(sys.argv) != 2:
        print("Usage: python script.py <folder_name>")
        sys.exit(1)

    folder_name = sys.argv[1]
    create_prompt(folder_name)
