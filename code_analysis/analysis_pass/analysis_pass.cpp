#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include  <iostream>
#include <fstream>

using namespace llvm;

namespace {

struct AnalysisPass : public PassInfoMixin<AnalysisPass> {

    bool isMemoryAccess(unsigned opcode) {
        return opcode == Instruction::Alloca || opcode == Instruction::Load || opcode == Instruction::Store
            || opcode == Instruction::GetElementPtr || opcode == Instruction::Fence
            || opcode == Instruction::AtomicCmpXchg || opcode == Instruction::AtomicRMW;
    }

    bool isBranch(unsigned opcode) {
        return opcode == Instruction::Br || opcode == Instruction::Switch || opcode == Instruction::IndirectBr;
    }
  

    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
        auto &bfi = FAM.getResult<BlockFrequencyAnalysis>(F);
        auto &bpi = FAM.getResult<BranchProbabilityAnalysis>(F);
        auto &LI = FAM.getResult<LoopAnalysis>(F);
        auto &SE = FAM.getResult<ScalarEvolutionAnalysis>(F);

        auto funcName = F.getName();

        // collect function frequency
        BasicBlock *functionEntry = &(F.getEntryBlock());
        uint64_t functionFrequency = bfi.getBlockFreq(functionEntry).getFrequency();

        uint64_t numBBs = 0;
        uint64_t totalBBFrequency = 0;
        uint64_t totalMemAccess = 0;
        uint64_t totalBiasedBranches = 0;
        uint64_t totalUnbiasedBranches = 0;

        for(const BasicBlock &BB: F) {
            // collect # basic blocks and block frequency
            numBBs += 1;
            std::optional< uint64_t > blockFrequencyOp = bfi.getBlockProfileCount(&BB);
            uint64_t blockFrequency = blockFrequencyOp.value_or(0);
            totalBBFrequency += blockFrequency;

            // collect number of memory accesses, biased/unbiased branches
            uint64_t numMemoryAccess = 0; 
            uint64_t numBiasedBranches = 0;
            uint64_t numUnbiasedBranches = 0;
            for (const Instruction &I : BB) {
            unsigned opcode = I.getOpcode();
            if (isMemoryAccess(opcode)) { numMemoryAccess += 1; }
            if (isBranch(opcode)) {
                // branchInstructions.push_back(&I);  
                bool isBiasedBranch = false; 
                for(int i = 0; i < I.getNumSuccessors(); i++) {
                BranchProbability brPr = bpi.getEdgeProbability(I.getParent(), I.getSuccessor(i));
                if(brPr >= BranchProbability(90, 100)) { isBiasedBranch = true; }
                }         
                if(isBiasedBranch) { numBiasedBranches += 1; }
                else { numUnbiasedBranches += 1;}
            }
            }

            totalMemAccess += numMemoryAccess;
            totalBiasedBranches += numBiasedBranches;
            totalUnbiasedBranches += numUnbiasedBranches;
        }

        // count loops and loop profile information
        uint64_t numLoops = 0;
        uint64_t totalLoopExecutions = 0;
        uint64_t totalLoopCycles = 0;
        uint64_t totalCyclesPerIteration = 0;
        for (LoopInfo::iterator loop_iter = LI.begin(); loop_iter != LI.end(); loop_iter++){
            numLoops += 1;
            BasicBlock *Header = (*loop_iter)->getHeader();
            uint64_t HeaderFreq = bfi.getBlockFreq(Header).getFrequency();
            uint64_t CyclesPerIteration = 10;
            uint64_t TotCycles = HeaderFreq * CyclesPerIteration;

            totalLoopExecutions += HeaderFreq;
            totalLoopCycles += TotCycles;
            totalCyclesPerIteration += CyclesPerIteration;
        }

        // Open a file to write the prompt to with this profile data
        std::ofstream outFile("./model/prompts/program/" + (std::string)funcName + ".txt");

        // Check if the file was opened successfully
        if (!outFile) {
            errs() << "Error: Unable to open file for writing\n";
        }
        else {
        outFile << (std::string)funcName + " "; 
        outFile << (int)functionFrequency << " ";
        outFile << numBBs << " ";
        double avgBBFrequency = (double)(totalBBFrequency) / (double)(numBBs);
        outFile << avgBBFrequency << " ";
        double avgBBMemAccess = (double)(totalMemAccess) / (double)(numBBs);
        outFile << avgBBMemAccess << " ";\
        double avgBBBiasedBranches = (double)(totalBiasedBranches) / (double)(numBBs);
        outFile << avgBBBiasedBranches << " ";
        double avgBBUnbiasedBranches = (double)(totalUnbiasedBranches) / (double)(numBBs);
        outFile << avgBBUnbiasedBranches << " ";
        outFile << numLoops << " ";
        double avgExecutionsPerLoop = (double)(totalLoopExecutions) / (double)(numLoops);
        outFile << avgExecutionsPerLoop << " ";
        double avgCyclesPerLoopIteration = (double)(totalCyclesPerIteration) / (double)(numLoops);
        outFile << avgCyclesPerLoopIteration << " ";

        // Close the file
        outFile.close();
        }
        
        return PreservedAnalyses::all();

    }
};

}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "AnalysisPass", "v0.1",
        [](PassBuilder &PB) {
        PB.registerPipelineParsingCallback(
            [](StringRef Name, FunctionPassManager &FPM,
            ArrayRef<PassBuilder::PipelineElement>) {
            if(Name == "analysis"){
                FPM.addPass(AnalysisPass());
                return true;
            }
            return false;
            }
        );
        }
    };
}
