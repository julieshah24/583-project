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

def write_nested_ifs(file, depth):
    """Recursively writes nested if-statements into the file."""
    if depth == 0:
        return

    v1 = random.choice(range(1, 6))
    v2 = random.choice(range(1, 6))
    b1 = random.choice(range(1, 6))
    b2 = random.choice(range(1, 6))

    # 10 - depth mod 10 gives the number of indents 
    # depth of 10, want 0 indent
    # depth of 9, want 1 indent 

    indent = (10 - depth) % 10
    # print("The indent is " + str(indent))
    # NOTE: indent is one less than the level of depth
    indent_temp = "  " * (indent - 4) 

    file.write(f"{indent_temp}bool bool{b1} = (rand() % 2 == 0);\n")
    file.write(f"{indent_temp}if (bool{b1}) {{\n")  # Double {{ and }} are used to include literal braces
    file.write(f"{indent_temp}    num{v1} += num{v2};\n")
    write_nested_ifs(file, depth - 1)  # Recursive call for deeper nesting
    file.write(f"{indent_temp}}} else {{\n")  # Correct use of braces for literal and variable
    file.write(f"{indent_temp}    num{v2} -= num{v1};\n")
    file.write(f"{indent_temp}}}\n")

def endless_ifs(program_name, ifs):
    """Generates a chain of nested if-statements in the given program file."""
    with open(program_name, 'a') as file:
        file.write('  int num1 = 0, num2 = 0, num3 = 0, num4 = 0, num5 = 0;\n')
        file.write('  bool bool1 = true, bool2 = false, bool3 = true, bool4 = false, bool5 = true;\n\n')
        # NOTE: change this from 5 to some other number if you want another number of nested if-statements
        write_nested_ifs(file, 5)  # Start writing nested if-statements

def generate_programs():
    """Generate 1000 .c files with nested if-statements."""
    directory = './generated/'
    
    if not os.path.exists(directory):
        os.makedirs(directory)

    for number in range(1, 1001):
        program_name = f"{directory}test_{number}.c"
        sof(program_name)
        endless_ifs(program_name, number % 5 + 1)  # Control depth of nesting, here it varies from 1 to 5
        eof(program_name)

generate_programs()
