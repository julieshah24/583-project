#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include  <iostream>

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
    auto &LI = FAM.getResult<LoopAnalysis>(F);
    auto &SE = FAM.getResult<ScalarEvolutionAnalysis>(F);
    //errs() << "Hello!";

    for (Function::iterator func_iter = F.begin(); func_iter != F.end(); func_iter++){

      // numbering basic blocks, not really helpful
      // std::unordered_map<BasicBlock*, unsigned> blockIDs;
      // unsigned id = 0;
      // for (BasicBlock &BB : F) {
      //     blockIDs[&BB] = id++;
      // }
      //errs() << "going through function";

      // prints frequency of functions
      BasicBlock *functionEntry = (*func_iter)->getEntryBlock();
      uint64_t functionFrequency = bfi.getBlockFreq(functionEntry).getFrequency();

      errs() << "FuncID: " << functionEntry << " ";
      errs() << "Executions: " << functionFrequency << "\n";

      for(const BasicBlock &BB: F) {
        // get the frequence of individual basic blocks
        std::optional< uint64_t > blockFrequencyOp = bfi.getBlockProfileCount(&BB);
        uint64_t blockFrequency = blockFrequencyOp.value_or(0);
        errs() << "BlockID: " << &BB << " ";
        errs() << "Executions: " << blockFrequency << " ";

        // std::vector<Instruction*> branchInstructions;
        uint64_t numMemoryAccess = 0; // memory accesses in a basic block? --> not sure if there is a diff profile metric we want for this
        uint64_t numBiasedBranches = 0;
        uint64_t numUnbiasedBranches = 0;
        for (const Instruction &I : BB) {
          unsigned opcode = I.getOpcode();
          if (isMemoryAccess(opcode)) { numMemoryAccess += 1; }

          // record which instructions are branches 
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
        errs() << "Memory Accesses: " << numMemoryAccess << " ";
        errs() << "Biased Branches: " << numBiasedBranches << " ";
        errs() << "Unbiased Branches: " << numUnbiasedBranches << "\n";
      }

        // use &li to iterate through the loops
        // this way, we don't have to iterate through each basic block and figure out which one has loop
        // loop through all of the loops
        for (LoopInfo::iterator loop_iter = LI.begin(); loop_iter != LI.end(); loop_iter++){
            //errs() << "going through loops";
            BasicBlock *Header = (*loop_iter)->getHeader();
            // errs() << "This is the loop " << "\n";
            // errs() << Header << "\n";
            // unsigned blockID = blockIDs[Header];
            uint64_t HeaderFreq = bfi.getBlockFreq(Header).getFrequency();

            uint64_t CyclesPerIteration = 10;
            uint64_t TotCycles = HeaderFreq * CyclesPerIteration;

            // errs() << "Loop in function " << F.getName() << " with header block " << Header->getName()
            //        << " executes approximately " << HeaderFreq << " times.\n"
            //        << " Estimated total cycles (assuming " << CyclesPerIteration << " cycles/iteration): " << TotCycles << "\n";
            errs() << "LoopID: " << Header << " ";
            //errs() << "LoopID: " << (*loop_iter)->getHeader()->getName() << " ";
            //errs() << "Location: " << File.str() << ":" << LineNo << " ";
            errs() << "Executions: " << HeaderFreq << " ";
            errs() << "Cycles: " << TotCycles << " ";
            errs() << "Cycles per Execution: " << CyclesPerIteration << "\n";
            // Get debug location information
            //if (const DebugLoc &DL = Header->getTerminator()->getDebugLoc()) {
              // errs() << "going through loop info";
              //unsigned LineNo = DL.getLine();
              //StringRef File = DL->getFilename();

            //   // Calculate loop trip count and total execution count
            //   std::optional<unsigned> TripCount = SE.getSmallConstantTripCount(*loop_iter);
            //   errs() << TripCount;
            //   std::optional<long unsigned int> BlockCount = bfi.getBlockProfileCount(Header);
            //   double TotalExecutions = TripCount.has_value() && BlockCount ? TripCount.value() * BlockCount.value() : 0;

            //   // Estimate the total clock cycles (simple estimation: 10 cycles per iteration for now)
            //   double TotalCycles = TotalExecutions * 10;  // Simplified assumption
              

            //   // Print loop information
            //   errs() << "LoopID: " << (*loop_iter)->getHeader()->getName() << " ";
            //   //errs() << "Location: " << File.str() << ":" << LineNo << " ";
            //   errs() << "Executions: " << TotalExecutions << " ";
            //   errs() << "Cycles: " << TotalCycles << "\n";
            // //}
          }
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
