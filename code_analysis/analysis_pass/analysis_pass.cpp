#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include  <iostream>

using namespace llvm;

namespace {

struct Pass : public PassInfoMixin<Pass> {

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
    // These variables contain the results of some analysis passes.
    llvm::BlockFrequencyAnalysis::Result &bfi = FAM.getResult<BlockFrequencyAnalysis>(F); // BlockFrequencyInfo
    llvm::BranchProbabilityAnalysis::Result &bpi = FAM.getResult<BranchProbabilityAnalysis>(F);

    double intALU =     0; // integer alu operations
    double fpALU =      0; // floating point alu operations
    double memory =     0; // memory operations
    double others =     0; // other operations
    double biased =     0; // biased branch operations
    double unbiased =   0; // unbiased branch operations
    double dynOpCount = 0; // dynamic operation count

    // Iterate through basic blocks in function
    for (Function::iterator func_iter = F.begin(); func_iter != F.end(); func_iter++) {
      BasicBlock *curr_block = &(*func_iter); // pointer to current block

      // Number of times block runs
      std::optional<long unsigned int> init_count = bfi.getBlockProfileCount(curr_block);
      double count = init_count.value_or(0);

      // Iterate through instructions in current basic block
      for (BasicBlock::iterator bb_iter = func_iter->begin(); bb_iter != func_iter->end(); bb_iter++) {
        unsigned opcode = bb_iter->getOpcode(); // instruction opcode
        dynOpCount += count; // dynamic number of times instruction runs

        // Branch - br, switch, indirectbr
        if(opcode == Instruction::Br || 
           opcode == Instruction::Switch || 
           opcode == Instruction::IndirectBr) {
          
          bool is_biased = false; // initial value
          
          // For each successor of current block
          for (BasicBlock *succesor: successors(curr_block)) {
            // Get probability from current block to successor
            BranchProbability edge_prob = bpi.getEdgeProbability(curr_block, succesor);
            if (edge_prob > BranchProbability(80, 100)) {
              is_biased = true;
              break; // found a biased branch
            }
          }

          // Increment appropriate count
          if(is_biased) biased += count;
          else          unbiased += count;
        }

        // Integer alu - add, sub, mul, udiv, sdiv, urem, shl, lshr, ashr, and, or, xor, icmp, srem
        else if(opcode == Instruction::Add || opcode == Instruction::UDiv ||
                opcode == Instruction::Sub || opcode == Instruction::SDiv ||
                opcode == Instruction::Mul || opcode == Instruction::URem ||
                opcode == Instruction::Shl || opcode == Instruction::LShr || 
                opcode == Instruction::And || opcode == Instruction::ICmp ||
                opcode == Instruction::Xor || opcode == Instruction::AShr || 
                opcode == Instruction::Or  || opcode == Instruction::SRem) {
          intALU += count;
        }

        // Floating point alu - fadd, fsub, fmul, fdiv, frem, fcmp
        else if(opcode == Instruction::FAdd || opcode == Instruction::FSub || 
                opcode == Instruction::FMul || opcode == Instruction::FDiv || 
                opcode == Instruction::FRem || opcode == Instruction::FCmp) {
          fpALU += count;
        }

        // Memory - alloca, load, store, getelementptr, fence, atomiccmpxchg, atomicrmw
        else if(opcode == Instruction::Store || opcode == Instruction::GetElementPtr ||
                opcode == Instruction::Fence || opcode == Instruction::AtomicCmpXchg ||
                opcode == Instruction::Load  || opcode == Instruction::Alloca ||
                opcode == Instruction::AtomicRMW) {
          memory += count;
        }

        // Others
        else others += count;
      }
    }

    // Print data
    errs() << F.getName();
    errs() << ", " << static_cast<uint64_t>(dynOpCount);
    // errs() << ", " << dynOpCount;
    for (double instr : {intALU, fpALU, memory, biased, unbiased, others}) { // iterate all categories
      if (dynOpCount == 0) errs() << ", " << format("%.3f", 0.000);
      else errs() << ", " << format("%.3f", instr / dynOpCount);
    }
    errs() << "\n";

    return PreservedAnalyses::all();
  }
};
}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "Pass", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
        ArrayRef<PassBuilder::PipelineElement>) {
          if(Name == "analysis"){
            FPM.addPass(Pass());
            return true;
          }
          return false;
        }
      );
    }
  };
}