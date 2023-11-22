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
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

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
  std::unordered_map<std::string, StructType *> structs;
  std::unordered_map<std::string, std::vector<int>> fieldIndexMapping;
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
      if (memberAccessCounts.count(structName) == 0)
      {
        errs() << *StructTy << "\n";
        structs[structName] = StructTy;
        int numFields = StructTy->getNumElements();
        for (int i = 0; i < numFields; i++)
        {
          Type *ty = StructTy->getTypeAtIndex(i);
          memberAccessCounts[structName][i].type = ty;
          memberAccessCounts[structName][i].accessCounts = 0;
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

  void reOrgStruct()
  {
    for (auto &st : structs)
    {
      if (!st.second->isOpaque())
      {
        std::string structName = st.first;
        std::vector<Type *> sortedFields;
        std::vector<int> fieldMap(sortedMemberVariables[structName].size());
        for (std::pair<unsigned, Stat> &item : sortedMemberVariables[structName])
        {
          fieldMap[item.first] = sortedFields.size();
          sortedFields.push_back(item.second.type);
        }
        fieldIndexMapping[structName] = std::move(fieldMap);
        st.second->setBody(sortedFields);
      }
    }
  }

  void fixUses(GetElementPtrInst *GEP)
  {
    if (GEP->getSourceElementType()->isStructTy())
    {
      std::string structName = GEP->getSourceElementType()->getStructName().str();
      IRBuilder<> Builder(GEP->getContext());
      Value *Operand = GEP->getOperand(GEP->getNumOperands() - 1);
      ConstantInt *Index = dyn_cast<ConstantInt>(Operand);
      errs() << *GEP << " AND " << *Index << " " << fieldIndexMapping[structName].size() << " " << Index->getBitWidth() << "\n";
      int newIndex = fieldIndexMapping[structName][Index->getSExtValue()];
      if (Index->getBitWidth() == 32)
      {
        GEP->setOperand(GEP->getNumOperands() - 1, Builder.getInt32(newIndex));
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

  // std::unordered_map<std::string, std::unordered_map<unsigned, std::string>> subStructMap;

  void printSubstructs()
  {
    for (auto &i : subStructMap)
    {
      errs() << "OG Struct: " << i.first << "\n";
      for (auto &j : i.second)
      {
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
      for (auto &BB : F)
      {
        uint64_t numExecuted = 0;
        if (bfi.getBlockProfileCount(&BB).has_value())
        {
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
            // errs() << I << "\n";
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
      for (auto &e : memberAccessCounts)
      {
        // s -> id and Stat
        //  errs() <<"CHECK: " << e.first << "\n";
        std::string structName = e.first;
        std::vector<std::pair<unsigned, Stat>> vec;

        for (auto &s : e.second)
        {
          // errs() <<"CHECK: " << s.first << "\n";
          vec.push_back({s.first, s.second});
        }

        std::sort(vec.begin(), vec.end(), [](const auto &a, const auto &b)
                  { return a.second.accessCounts > b.second.accessCounts; });

        sortedMemberVariables[structName] = vec;
      }

      // Step 2: We want to split our struct into smaller struct groups
      // Output: Key : id/name of the struct, Value: Map<Key: member variable ID, Value: subStruct name>
      // Into 3 structs

      // A -> a, b, c, d
      // ceil = 3 {ssID = 0,1,2}
      // super struct A -> <{0,0}, {1,1}, c {2}, d {1}, e{0}>
      //{ {(0,A_1),(1,A_2)}, {(1, B_3){3, B_2},  }
      //         A             B

      // Key: id/name of the struct, Value: vector of pairs of Id and type (sorted by hits)

      // A, B, C, D, E

      for (auto &e : sortedMemberVariables)
      {
        std::string ogStructName = e.first;
        errs() << "e.second size: " << std::to_string(e.second.size()) << "\n";
        int mvCount = std::ceil((double)e.second.size() / (double)3);
        errs() << "mvCount: " << std::to_string(mvCount) << "\n";

        std::unordered_map<unsigned, std::string> valueMap;
        int struct_idx = 0;
        int nameIdx = 0;

        while (struct_idx < e.second.size())
        { // e.second = vector representing the super struct A
          int sub_idx = 0;

          while (sub_idx < mvCount)
          {
            // err() << std::to_string(sub_idx);
            std::string newStructName = ogStructName + "_" + std::to_string(nameIdx);
            valueMap[e.second[struct_idx].first] = newStructName;
            ++sub_idx;
            ++struct_idx;
            errs() << "nameIdx: " << std::to_string(nameIdx) << "\n";
            if (struct_idx >= e.second.size())
            {
              break;
            }
          }
          ++nameIdx;
          // ++struct_idx;
        }
        subStructMap[ogStructName] = valueMap;
      }

      printResults();
      printSubstructs();
      reOrgStruct();

      for (auto &BB : F)
      {
        for (auto &I : BB)
        {
          if (std::string(I.getOpcodeName()) == "getelementptr")
          {
            GetElementPtrInst *ptrInst = dyn_cast<GetElementPtrInst>(&I);
            fixUses(ptrInst);
          }
        }
      }

      return PreservedAnalyses::all();
    }
  };

  // // Define a new struct declaration pass
  // class AddStructDeclarationPass : public PassInfoMixin<AddStructDeclarationPass>
  // {
  // public:
  //   // Entry point for the pass
  //   PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM)
  //   {
  //     LLVMContext &Context = M.getContext();
  //     // Create a new struct type (you can customize this based on your needs)
  //     Type *IntType = Type::getInt32Ty(Context);
  //     Type *FloatType = Type::getFloatTy(Context);
  //     Type *DoubleType = Type::getDoubleTy(Context);

  //     std::vector<Type *> StructMembers = {IntType, FloatType, DoubleType};
  //     StructType *MyStructType = StructType::create(Context, StructMembers, "MyStruct");

  //     // Print information about the created struct
  //     errs() << "Created struct type: " << *MyStructType << "\n";

  //     // Create an IRBuilder for inserting instructions
  //     // IRBuilder<> Builder(Context);

  //     // // You can use the struct type to create a variable of that type
  //     // Value *MyStructVariable = Builder.CreateAlloca(MyStructType, nullptr, "myStructVar");

  //     // // Print information about the created variable
  //     // errs() << "Created struct variable: " << *MyStructVariable << "\n";

  //     // // Emit LLVM IR for the module
  //     // M.print(outs(), nullptr);

  //     return PreservedAnalyses::all(); // This pass doesn't modify the module
  //   }
  // };
}

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
              return false;
            });
        // PB.registerModulePass<AddStructDeclarationPass>("my-module-pass", "My Module Pass", false, false);

        // PB.registerPipelineParsingCallback(
        //     [](StringRef Name, ModulePassManager &MPM,
        //        ...)
        //     {
        //       if(Name == "fplicm-correctness")
        //       {
        //         MPM.addPass( AddStructDeclarationPass());
        //         return true;
        //       }
        //       return false; });
      }};
}
