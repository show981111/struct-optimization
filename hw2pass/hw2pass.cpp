//===--------------------------- Struct Optimization --------------------===//
//
//
//===---------------------------------------------------------------------===//
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
      // for (Function &F : M)
      // {
      //   if (!F.isDeclaration())
      //   {
      //     opt.structReordering(F);
      //   }
      // }

      // errs() << "*** Field Reordering Done ***\n";

      DataLayout dataLayout(&M);
      errs() << "\nCreating Sub Struct Map\n";
      opt.createSubStructMap(dataLayout);
      errs() << "\nPrinting Sub Struct Map\n";
      opt.printSubStructMap();
      errs() << "\nAdding Struct Delcaration\n";
      opt.addStructDeclaration(M, Context);
      errs() << "\nAdd New Instance Declaration\n";
      opt.addNewInstanceDeclaration(M, Context);
      errs() << "\nAdd New Array Instance Declaration\n";
      opt.addNewArrayInstanceDeclaration(M, Context);
      errs() << "\nFixing Usages of Instance\n";
      opt.fixUsagesOfInstance();
      errs() << "\nFixing Usages of Array\n";
      opt.fixArrayInstanceUsage();

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
