###################################################################
#                                                                 #
# This script takes a prompt file and uses its contents to prompt #
# the LLM LLAMA 3. The output produced by LLAMA 3 will then be    #
# saved in the folder "./outputs/".                               #
#                                                                 #
# To use follow the following input:                              #
#     python3 query.py <file containing prompt>.txt                #
#                                                                 #
###################################################################
import replicate
import sys
from datetime import datetime

# reads prompt file given in arguments
def get_prompt(file_name):
    
    file_path = './prompts/' + file_name
    
    try:
        file = open(file_path, 'r')
    except:
        print('\nERROR WITH PROMPT FILE')
        print('This script cannot find the prompt file "' + file_name + '" in "./prompts/"')
        print('Please make sure the prompt file is located in "./prompts/"\n')
        return ''

    try:
        prompt = file.read()
    except:
        print('\nERROR READING PROMPT FILE\n')
        return ''
    
    try:
        file.close()
    except:
        print('\nERROR CLOSING PROMPT FILE\n')
        return ''
    
    return prompt

# queries llama with the given prompt
def query_llama(prompt):
    
    input = {'prompt': prompt, 'prompt_template': '<|begin_of_text|><|start_header_id|>system<|end_header_id|>\n\nYou are a helpful assistant<|eot_id|><|start_header_id|>user<|end_header_id|>\n\n{prompt}<|eot_id|><|start_header_id|>assistant<|end_header_id|>\n\n'}
   
    output = replicate.run('meta/meta-llama-3-8b-instruct', # can replace 8 with 70
            input=input)

    return output

# saves model output to a text file
def save_model_output(output, file_name):
    
    now = datetime.now()
    dt_str = now.strftime('%d%m%Y_%H%M%S')
    file_path = './output/' + file_name 
    
    try:
        file = open(file_path, 'w')
    except:
        print('\nERROR OPENING OUTPUT FILE\n')
        return

    try:
        file.write(output)
    except:
        print('\nERROR WRITING TO OUTPUT FILE\n')
        return ''

    try:
        file.close()
    except:
        print('\nERROR CLOSING OUTPUT FILE\n')
        return
    
    print('Model output saved to "' + file_path + '"')
    return

# runs script
if len(sys.argv) < 2:
    print('\nERROR WITH INPUT')
    print('To run this script, please use the following input structure:')
    print('    python query.py <file containing prompt>\n')
else:
    prompt_str = get_prompt(sys.argv[1])
    if prompt_str != '':
        output = query_llama(prompt_str)
        save_model_output("".join(output), sys.argv[1])
    else:
        print('Prompt was empty. LLAMA was not queried.\n')
