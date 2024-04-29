# 583-project

# Abstract
Large Language Models (LLMs) are seeing increasing use in code-related domains due to the similarities between code and natural language. [Recent work](https://arxiv.org/pdf/2309.07062) has shown that LLMs that have been trained on IR and compiler related data have the ability to suggest meaningful LLVM optimization flags to improve the line count of the compiled code. However, training an LLM is an incredibly costly procedure and requires access to the model architecture, so this process is not widely accessible. This project proposes that pretrained off-the-shelf LLMs may have enough insight into profile data for a program to predict useful compilation flags. We implement an automated pipeline that can collect profile data for a C program, query the LLM Llama 3, and compile and retrieve the line count and runtime with the suggested flags. We evaluate this implementation on 171 C programs taken from prior course projects and open source projects on GitHub, and find that we aren't able to consistently beat our baselines but show a promising start.

---

# Initial Set-up
### Building the pass
Navigate to the code_analysis directory and run the following:
``` bash
mkdir build && cd build	
cmake ..
make
```

To recompile after making changes to AnalysisPass, cd to the build folder and run 
``` bash
make 
```

### Authenticating and using Llama 3
1. In your terminal, run 
``` bash
pip install replicate
```
2. Go to https://replicate.com/account/api-tokens and copy your token
3. In your terminal, run
``` bash
export REPLICATE_API_TOKEN=<paste-your-token-here>
```
Note that this token is unique to your GitHub account and has a limited on the number of times you can run it, so we recommend commenting out any lines in the evaluate scripts that run the model until you are ready to do so.
