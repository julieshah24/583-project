# 583-project

# Capabilities
1. Run the analysis pass to collect the profile data
   The result of this pass is a .txt file in the model/prompts folder

2. Run the model query script to get the suggested optimization flags

3. Run the evaluate script to compile the code with the baselines and the 
   recommended optimizations, storing the instruction counts of each into a 
   csv file?

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
