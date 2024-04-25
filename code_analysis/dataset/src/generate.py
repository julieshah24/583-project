###################################################################
#                                                                 #
# This script generates 1000 .c files and outputs them in the     #
# ./generated/ folder. These .c files require no input. They are  #
# pointless programs that ultimately do nothing.                  #
#                                                                 #
# To use follow the following input:                              #
#     python generate.py                                          #
#                                                                 #
###################################################################
import os
import random

# start of file (main function)
def sof(program_name):
    starter = '#include <stdio.h>\n#include <stdbool.h>\n\nint main(void)\n{\n'
    with open(program_name, 'w') as file:
        file.write(starter)
        file.close()
    return

# end of file (main function)
def eof(program_name):
    ender = '\nreturn 0;\n}\n'
    with open(program_name, 'a') as file:
        file.write(ender)
        file.close()
    return

def endless_ifs(program_name, ifs):
    
    v1 = random.choice(range(0, 100))
    v2 = random.choice(range(0, 100))
    v3 = random.choice(range(0, 100))
    v4 = random.choice(range(0, 100))
    v5 = random.choice(range(0, 100))

    contents = 'int num1 = ' + str(v1) + ';\nint num2 =  ' + str(v2) + ';\nint num3 =  ' + str(v3) + ';\nint num4 =  ' + str(v4) + ';\nint num5 =  ' + str(v5) + ';\n'
    contents = contents + 'bool bool1 = true;\nbool bool2 = true;\nbool bool3 = true;\nbool bool4 = true;\nbool bool5 = true;\n\n'
    
    for i in range(1, ifs + 1):
        value_num1 = random.choice(range(1, 6))
        value_num2 = random.choice(range(1, 6))
        bool_num1 = random.choice(range(1, 6))
        bool_num2 = random.choice(range(1, 6))
        
        new_bool = random.choice([0,1])
        if new_bool == 1:
            contents = contents + 'bool' + str(bool_num1) + ' = true;\n'
            contents = contents + 'if (bool' + str(bool_num1) + ' == bool' + str(bool_num2) + ')\n{\nnum' + str(value_num1) + ' = num' + str(value_num2) + ' + 1;\n'
        else:
            contents = contents + 'bool' + str(bool_num1) + ' = true;\n'
            contents = contents + 'if (bool' + str(bool_num1) + ' == bool' + str(bool_num2) + ')\n{\nnum' + str(value_num1) + ' = num' + str(value_num2) + ' + 1;\n'
        
        contents = contents + '}\n' #close the if statement
    
    with open(program_name, 'a') as file:
        file.write(contents)
        file.close()
    return

# generates 1000 dummy test .c programs
def generate_programs() :
    
    directory = '.\\generated\\'
    
    if not os.path.exists(directory):
        os.makedirs(directory)

    for number in range(1, 1001):
        program_name = directory + 'test_' + str(number) + '.c' # .\generated\test_#.c
        sof(program_name)
        endless_ifs(program_name, number)
        eof(program_name)
    
    return

generate_programs()