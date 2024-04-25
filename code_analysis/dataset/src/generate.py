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

def main_start(program_name):
    starter = '#include <stdio.h>\n#include <stdbool.h>\n\nint main(void)\n{\n'
    with open(program_name, 'w') as file:
        file.write(starter)
        file.close()
    return

def main_end(program_name):
    ender = '\nreturn 0;\n}\n'
    with open(program_name, 'a') as file:
        file.write(ender)
        file.close()
    return

def endless_ifs(program_name, ifs):
    value = random.choice(range(0, 1000))
    contents = 'int num = ' + str(value) + ';\nbool bool_0 = true;\n'
    for i in range(1, ifs + 1):
        is_enter = random.choice([0,1])
        bool_num = random.choice(range(0, i + 1))
        if is_enter == 1:
            contents = contents + 'bool bool_' + str(i) 
            contents = contents + ' = true;\nif (bool_' + str(i) + ' == bool_' + str(bool_num) + ')\n{\nnum += 1;\n}\n'
        else:
            contents = contents + 'bool bool_' + str(i) 
            contents = contents + ' = false;\nif (bool_' + str(i) + ' == bool_' + str(bool_num) + ')\n{\nnum += 1;\n}\n'
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
        main_start(program_name)
        endless_ifs(program_name, number)
        main_end(program_name)
    
    return

generate_programs()