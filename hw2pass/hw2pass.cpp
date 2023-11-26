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
// #include "llvm/Analysis/BlockFrequencyInfo.h"
// #include "llvm/Analysis/BranchProbabilityInfo.h"
// #include "llvm/Analysis/LoopInfo.h"
// #include "llvm/Analysis/LoopIterator.h"
// #include "llvm/Analysis/LoopPass.h"
// #include "llvm/IR/CFG.h"
// #include "llvm/IR/Instructions.h"
// #include "llvm/IR/LLVMContext.h"
// #include "llvm/IR/PassManager.h"
// #include "llvm/Passes/PassBuilder.h"
// #include "llvm/Passes/PassPlugin.h"
// #include "llvm/Support/Debug.h"
// #include "llvm/Support/raw_ostream.h"
// #include "llvm/Transforms/Scalar/LoopPassManager.h"
// #include "llvm/Transforms/Utils/BasicBlockUtils.h"
// #include "llvm/Transforms/Utils/LoopUtils.h"
// #include "llvm/Analysis/DominanceFrontier.h"
// #include "llvm/Pass.h"
// #include "llvm/IR/Function.h"
// #include "llvm/IR/Instruction.h"
// #include "llvm/IR/Instructions.h"
// #include "llvm/Support/raw_ostream.h"
// #include "llvm/IR/DerivedTypes.h"
// #include "llvm/IR/Module.h"
// #include "llvm/Support/raw_ostream.h"
// #include "llvm/IR/IRBuilder.h"
#include "StructOpt.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <unordered_set>

using namespace llvm;

/**
 * Useful links:
 * https://llvm.org/docs/GetElementPtr.html
 */
namespace
{
  struct StructOptimization : public PassInfoMixin<StructOptimization>
  {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM)
    {
      LLVMContext &Context = M.getContext();
      IRBuilder<> Builder(Context);

      errs() << "CREATE STRUCT PASS\n";

      FunctionAnalysisManager &FAM = MAM.getResult<FunctionAnalysisManagerModuleProxy>(M).getManager();
      StructOpt opt;

      for (Function &F : M)
      {
        if (!F.isDeclaration())
        {
          llvm::BlockFrequencyAnalysis::Result &bfi = FAM.getResult<BlockFrequencyAnalysis>(F);
          llvm::BranchProbabilityAnalysis::Result &bpi = FAM.getResult<BranchProbabilityAnalysis>(F);
          errs() << "NAME " << F.getName() << "\n";
          opt.profiler.profileFunction(F, bfi);
        }
      }
      opt.profiler.createSortedMemberVariables();
      opt.profiler.printResult();

      // Field Reordering
      for (Function &F : M)
      {
        if (!F.isDeclaration())
        {
          opt.structReordering(F);
        }
      }

      errs() << "*** Field Reordering Done ***\n";

      DataLayout dataLayout(&M);
      opt.createSubStructMap(dataLayout);
      opt.printSubStructMap();
      opt.addStructDeclaration(M, Context);
      opt.addNewInstanceDeclaration(M, Context);
      opt.fixUsagesOfInstance();

      return PreservedAnalyses::all();
    }
  };
}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo()
{
  return {
      LLVM_PLUGIN_API_VERSION, "HW2Pass", "v0.1",
      [](PassBuilder &PB)
      {
        // PB.registerPipelineParsingCallback(
        //     [](StringRef Name, FunctionPassManager &FPM,
        //        ArrayRef<PassBuilder::PipelineElement>)
        //     {
        //       if (Name == "fplicm-correctness")
        //       {
        //         FPM.addPass(HW2CorrectnessPass());
        //         return true;
        //       }
        //       return false;
        //     });
        PB.registerPipelineParsingCallback(
            [](StringRef Name, ModulePassManager &MPM,
               ...)
            {
              if (Name == "struct-optimization")
              {
                MPM.addPass(StructOptimization());
                return true;
              }
              return false;
            });
      }};
}
