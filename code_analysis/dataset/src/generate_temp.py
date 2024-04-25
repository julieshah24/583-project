import os
import random

def sof(program_name):
    """Start of file, writing the main function and includes."""
    starter = '#include <stdio.h>\n#include <stdbool.h>\n\nint main(void)\n{\n'
    with open(program_name, 'w') as file:
        file.write(starter)

def eof(program_name):
    """End of file, writing return statement and closing the main function."""
    ender = '\nreturn 0;\n}\n'
    with open(program_name, 'a') as file:
        file.write(ender)

def write_if_nested(program_name, depth, max_depth):
    """Writes nested if-statements recursively with else blocks including further nesting."""
    if depth > max_depth:
        # print("the depth is: " + str(depth))
        # print("the max depth is: " + str(max_depth))
        return

    # Choosing random variables for conditions and assignments
    v1, v2 = random.sample(range(1, 6), 2)  # Ensure different variables for num
    b1, b2 = random.sample(range(1, 6), 2)  # Ensure different variables for bool

    with open(program_name, 'a') as file:
        file.write(f"bool bool{b1} = true;\n")
        file.write(f"if (bool{b1} == bool{b2})\n{{\n")
        file.write(f"  num{v1} = num{v2} + 1;\n")
        file.write("} else {\n")
        # file.write("HELLOOO")
        write_if_nested(program_name, depth + 1, max_depth)
        file.write(f"  num{v2} = num{v1} + 1;\n")
        if depth + 1 <= max_depth:
            # file.write("HELLLLLLLLLLOOOOOOOOO")
            write_if_nested(program_name, depth + 1, max_depth)  # Nested block within the else
        file.write("}\n")

def endless_ifs(program_name, ifs):
    """Generates a chain of nested if-statements in the given program file."""
    initial_values = '\n'.join(f"int num{i} = {random.randint(0, 100)};" for i in range(1, 6))
    initial_bools = '\n'.join(f"bool bool{i} = true;" for i in range(1, 6))

    with open(program_name, 'a') as file:
        file.write(initial_values + '\n' + initial_bools + '\n\n')

    write_if_nested(program_name, 1, ifs)  # Start recursive nesting
    # print("this is: " + str(ifs))

def generate_programs():
    """Generate 1000 .c files with nested if-statements."""
    directory = './generated/'

    if not os.path.exists(directory):
        os.makedirs(directory)

    for number in range(1, 1001):
        program_name = f"{directory}test_{number}.c"
        sof(program_name)
        endless_ifs(program_name, 10)  # Control the depth of nesting; you can adjust logic here
        eof(program_name)

generate_programs()
