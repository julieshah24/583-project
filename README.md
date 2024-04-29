# 583-project

# Abstract
Large Language Models (LLMs) are seeing increasing use in code-related domains due to the similarities between code and natural language. [Recent work](https://arxiv.org/pdf/2309.07062) has shown that LLMs that have been trained on IR and compiler related data have the ability to suggest meaningful LLVM optimization flags to improve the line count of the compiled code. However, training an LLM is an incredibly costly procedure and requires access to the model architecture, so this process is not widely accessible. This project proposes that pretrained off-the-shelf LLMs may have enough insight into profile data for a program to predict useful compilation flags. We implement an automated pipeline that can collect profile data for a C program, query the LLM Llama 3, and compile and retrieve the line count and runtime with the suggested flags. We evaluate this implementation on 171 C programs taken from prior course projects and open source projects on GitHub, and find that we aren't able to consistently beat our baselines but show a promising start.

---

# Initial Set-up:
1. Navigate to the code_analysis project level directory
2. Run mkdir build && cd build	
3. run cmake ..
4. Run make

After making changes to AnalysisPass, cd to the build folder and run make
to recompile the changes.

To be able to run the model:
in your terminal, run pip install replicate
Go to https://replicate.com/account/api-tokens and get a token
In your terminal, run export REPLICATE_API_TOKEN=<paste-your-token-here>
