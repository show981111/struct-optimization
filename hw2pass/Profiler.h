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
#include "llvm/IR/IRBuilder.h"

#include <unordered_map>
#include <unordered_set>

using namespace llvm;

typedef unsigned int MemberIndex;
class Profiler
{
private:
    void profileInst(Instruction &I, uint64_t numExecuted);

public:
    struct Stat
    {
        u_int64_t accessCounts;
        Type *type;
    };
    /**
     * Useful API of Type
     * ty->isStructTy()
     * ty->getStructName().str()
     * ty->getTypeID()
     */
    std::unordered_map<std::string, std::unordered_map<MemberIndex, Stat>> memberAccessCounts;
    std::unordered_map<std::string, std::vector<std::pair<MemberIndex, Stat>>> sortedMemberVariables;
    std::unordered_map<std::string, std::unordered_set<AllocaInst *>> structInstances;
    std::unordered_map<std::string, StructType *> structs;

    Profiler() {}
    void profileFunction(llvm::Function &F, llvm::BlockFrequencyAnalysis::Result &bfi);
    void printResult() const;
    void createSortedMemberVariables();
};