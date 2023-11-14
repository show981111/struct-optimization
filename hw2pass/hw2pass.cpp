//===-- Frequent Path Loop Invariant Code Motion Pass --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
//
// EECS583 F23 - This pass can be used as a template for your FPLICM homework
//               assignment.
//               The passes get registered as "fplicm-correctness" and
//               "fplicm-performance".
//
//
////===-------------------------------------------------------------------===//
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopIterator.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Analysis/DominanceFrontier.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <unordered_set>
/* *******Implementation Starts Here******* */
// You can include more Header files here
/* *******Implementation Ends Here******* */

using namespace llvm;

namespace
{
  std::unordered_map<std::string, std::unordered_map<unsigned, unsigned>> MemberAccessCounts;

  void analyzePtr(GetElementPtrInst *GEP)
  {
    if (GEP->getSourceElementType()->isStructTy())
    {
      std::string structName = GEP->getSourceElementType()->getStructName().str();
      // Get the structure type
      StructType *StructTy = cast<StructType>(GEP->getSourceElementType());
      errs() << *StructTy << "\n";

      Value *Operand = GEP->getOperand(GEP->getNumOperands() - 1);
      ConstantInt *Index = dyn_cast<ConstantInt>(Operand);
      if (Index)
      {
        // Extract the integer value
        int IndexValue = Index->getSExtValue();

        // Now, IndexValue contains the value (in this case, 3)
        errs() << "Extracted index value: " << IndexValue << "\n";
        MemberAccessCounts[structName][IndexValue]++;
      }
    }
  }

  // Print the analysis results
  void printResults()
  {
    errs() << "Struct Member Access Counts:\n";
    for (const auto &StructEntry : MemberAccessCounts)
    {
      errs() << "Struct: " << StructEntry.first << "\n";
      for (const auto &MemberEntry : StructEntry.second)
      {
        errs() << "  Member " << MemberEntry.first << ": " << MemberEntry.second << " times\n";
      }
    }
  }
  struct HW2CorrectnessPass : public PassInfoMixin<HW2CorrectnessPass>
  {

    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM)
    {
      llvm::BlockFrequencyAnalysis::Result &bfi = FAM.getResult<BlockFrequencyAnalysis>(F);
      llvm::BranchProbabilityAnalysis::Result &bpi = FAM.getResult<BranchProbabilityAnalysis>(F);
      llvm::LoopAnalysis::Result &li = FAM.getResult<LoopAnalysis>(F);
      DominatorTree &DT = FAM.getResult<DominatorTreeAnalysis>(F);
      DominanceFrontier &DF = FAM.getResult<DominanceFrontierAnalysis>(F);

      /* *******Implementation Starts Here******* */
      // Your core logic should reside here.
      errs() << "Running a Pass\n";
      for (auto &BB : F)
      {
        for (auto &I : BB)
        {
          if (std::string(I.getOpcodeName()) == "getelementptr")
          {
            GetElementPtrInst *ptrInst = dyn_cast<GetElementPtrInst>(&I);
            errs() << I << "\n";
            analyzePtr(ptrInst);
          }
        }
      }

      printResults();
      /* *******Implementation Ends Here******* */
      // Your pass is modifying the source code. Figure out which analyses
      // are preserved and only return those, not all.

      // Indicate that LoopInfo is preserved
      PreservedAnalyses PA;
      PA.preserve<LoopAnalysis>();
      return PA;
    }
  };
  struct HW2PerformancePass : public PassInfoMixin<HW2PerformancePass>
  {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM)
    {
      llvm::BlockFrequencyAnalysis::Result &bfi = FAM.getResult<BlockFrequencyAnalysis>(F);
      llvm::BranchProbabilityAnalysis::Result &bpi = FAM.getResult<BranchProbabilityAnalysis>(F);
      llvm::LoopAnalysis::Result &li = FAM.getResult<LoopAnalysis>(F);
      /* *******Implementation Starts Here******* */
      // This is a bonus. You do not need to attempt this to receive full credit.
      /* *******Implementation Ends Here******* */

      // Your pass is modifying the source code. Figure out which analyses
      // are preserved and only return those, not all.
      return PreservedAnalyses::all();
    }
  };

  extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo()
  {
    return {
        LLVM_PLUGIN_API_VERSION, "HW2Pass", "v0.1",
        [](PassBuilder &PB)
        {
          PB.registerPipelineParsingCallback(
              [](StringRef Name, FunctionPassManager &FPM,
                 ArrayRef<PassBuilder::PipelineElement>)
              {
                if (Name == "fplicm-correctness")
                {
                  FPM.addPass(HW2CorrectnessPass());
                  return true;
                }
                if (Name == "fplicm-performance")
                {
                  FPM.addPass(HW2PerformancePass());
                  return true;
                }
                return false;
              });
        }};
  }
}