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
#include "llvm/IR/DerivedTypes.h"

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
  struct Stat
  {
    u_int64_t accessCounts;
    Type *type;
  };
  // structName -> member field index -> Stat(accessCounts, type)
  /**
   * Useful API of Type
   * ty->isStructTy()
   * ty->getStructName().str()
   * ty->getTypeID()
   */
  std::unordered_map<std::string, std::unordered_map<unsigned, Stat>> memberAccessCounts;
  std::unordered_map<std::string, std::vector<std::pair<unsigned, Stat>>> sortedMemberVariables;
  std::unordered_map<std::string, std::unordered_map<unsigned, std::string>> subStructMap;

  void analyzePtr(GetElementPtrInst *GEP, uint64_t numExecuted)
  {
    if (GEP->getSourceElementType()->isStructTy())
    {
      std::string structName = GEP->getSourceElementType()->getStructName().str();
      // Get the structure type
      StructType *StructTy = cast<StructType>(GEP->getSourceElementType());
      errs() << *StructTy << "\n";
      if (memberAccessCounts.count(structName) == 0)
      {
        int numFields = StructTy->getNumElements();
        for (int i = 0; i < numFields; i++)
        {
          Type *ty = StructTy->getTypeAtIndex(i);
          memberAccessCounts[structName][i].type = ty;
          memberAccessCounts[structName][i].accessCounts = 0;
          // if (ty->isStructTy())
          //   errs() << ty->getStructName().str();
          // else
          //   errs() << ty->getTypeID();
        }
      }

      // StructTy->
      Value *Operand = GEP->getOperand(GEP->getNumOperands() - 1);
      ConstantInt *Index = dyn_cast<ConstantInt>(Operand);
      if (Index)
      {
        // Extract the integer value
        int IndexValue = Index->getSExtValue();
        // memberAccessCounts[structName][IndexValue].typeName =
        StructTy->getTypeAtIndex(IndexValue)->print(errs());
        // Now, IndexValue contains the value (in this case, 3)
        errs() << " Extracted index value: " << IndexValue << "\n";
        memberAccessCounts[structName][IndexValue].accessCounts += numExecuted;
      }
    }
  }

  // Print the analysis results
  void printResults()
  {
    errs() << "Struct Member Access Counts:\n";
    for (const auto &StructEntry : sortedMemberVariables)
    {
      errs() << "Struct: " << StructEntry.first << "\n";
      for (const auto &MemberEntry : StructEntry.second)
      {
        errs() << "  Member " << MemberEntry.first << ": " << MemberEntry.second.accessCounts << " times, Type: " << *MemberEntry.second.type << "\n";
      }
    }
  }

  //std::unordered_map<std::string, std::unordered_map<unsigned, std::string>> subStructMap;

  void printSubstructs(){
    for (auto &i : subStructMap){
      errs() << "OG Struct: " << i.first << "\n";
      for (auto &j : i.second){
        errs() << "member variable ID: " << std::to_string(j.first) << ", SubStruct Name: " << j.second << "\n";
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

      // 1. Generating Profile data
      for (auto &BB : F) {
        uint64_t numExecuted = 0;
        if (bfi.getBlockProfileCount(&BB).has_value()) {
          // https://llvm.org/doxygen/BlockFrequencyInfo_8h_source.html get how many times this BB got executed
          numExecuted = bfi.getBlockProfileCount(&BB).value();
        }

        if (numExecuted == 0)
          continue;

        for (auto &I : BB)
        {
          if (std::string(I.getOpcodeName()) == "getelementptr")
          {
            GetElementPtrInst *ptrInst = dyn_cast<GetElementPtrInst>(&I);
            //errs() << I << "\n";
            analyzePtr(ptrInst, numExecuted);
          }
        }
      }

      // 2. Pre-pass generate super-struct map
      // std::unordered_map<std::string, std::unordered_map<unsigned, Stat>> memberAccessCounts;

      // Step 1: Order map based on hit-count values
      // Create a vector to store the key-value pairs

      // Key: id/name of the struct, Value: vector of pairs of Id and type (sorted by hits)
      

      // e -> a map for each struct
      for (auto &e : memberAccessCounts) {
        //s -> id and Stat
        // errs() <<"CHECK: " << e.first << "\n";
        std::string structName = e.first;
        std::vector<std::pair<unsigned, Stat>> vec;

        for(auto &s : e.second) { 
          // errs() <<"CHECK: " << s.first << "\n";
          vec.push_back({s.first, s.second});
        }

        std::sort(vec.begin(), vec.end(), [](const auto &a, const auto &b) {
          return a.second.accessCounts > b.second.accessCounts;
        });
        
        sortedMemberVariables[structName] = vec;
      }

      // Step 2: We want to split our struct into smaller struct groups
      //Output: Key : id/name of the struct, Value: Map<Key: member variable ID, Value: subStruct name>
      // Into 3 structs

      // A -> a, b, c, d 
      // ceil = 3 {ssID = 0,1,2}
      // super struct A -> <{0,0}, {1,1}, c {2}, d {1}, e{0}>
      //{ {(0,A_1),(1,A_2)}, {(1, B_3){3, B_2},  }
      //         A             B

      // Key: id/name of the struct, Value: vector of pairs of Id and type (sorted by hits)

      // A, B, C, D, E


      

      for (auto &e : sortedMemberVariables) {
        std::string ogStructName = e.first;
        errs() << "e.second size: " << std::to_string(e.second.size()) << "\n";
        int mvCount = std::ceil((double)e.second.size() / (double) 3);
        errs() << "mvCount: " << std::to_string(mvCount) << "\n";

        std::unordered_map<unsigned, std::string> valueMap;
        int struct_idx = 0;
        int nameIdx = 0;

        while(struct_idx < e.second.size() ) { // e.second = vector representing the super struct A
          int sub_idx = 0;
          
          while(sub_idx < mvCount) {
            //err() << std::to_string(sub_idx);
            std::string newStructName = ogStructName + "_" + std::to_string(nameIdx);
            valueMap[e.second[struct_idx].first] = newStructName;
            ++sub_idx;
            ++struct_idx;
            errs() << "nameIdx: " << std::to_string(nameIdx) << "\n";
            if(struct_idx >= e.second.size()) {
              break;
            }
          }
          ++nameIdx;
          // ++struct_idx;
        }
        subStructMap[ogStructName] = valueMap;
      }
      
      // for (auto i = memberAccessCounts.begin(); i != memberAccessCounts.end(); ++i){
      //   std::vector<std::pair<unsigned, Stat>> vec (s->second.begin(), s->second.end());
      //     std::sort(vec.begin(), vec.end(), [](const auto &a, const auto &b) {
      //       return a.second.accessCounts < b.second.accessCounts;
      //     });
      // }

      // Sort the vector based on the values

      // int curr_total_count = 0;
      // int top_heuristic = 0;
      // for (auto i : memberAccessCounts) {
      //   curr_total_count = i.size();
      //   for (auto j : i) {
      //     j.accessCounts
      //   }
      // }

      printResults();
      printSubstructs();
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