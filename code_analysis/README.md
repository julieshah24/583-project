Initial Set-up:
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
