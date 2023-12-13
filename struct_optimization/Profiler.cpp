#include "Profiler.h"

void Profiler::profileInst(Instruction &I, uint64_t numExecuted, DataLayout &dataLayout)
{
    if (std::string(I.getOpcodeName()) == "alloca")
    {
        // If loading the struct instance, grab it!
        AllocaInst *allocInst = dyn_cast<AllocaInst>(&I);
        if (allocInst->getAllocatedType()->isArrayTy())
        {
            ArrayType *ArrayTy = cast<ArrayType>(allocInst->getAllocatedType());
            if (ArrayTy->getElementType()->isStructTy())
            {
                arrayInstances[ArrayTy->getElementType()->getStructName().str()].insert(allocInst);
            }
        }
        if (allocInst->getAllocatedType()->isStructTy())
        {
            StructType *StructTy = cast<StructType>(allocInst->getAllocatedType());
            structInstances[StructTy->getStructName().str()].insert(allocInst);
        }
    }
    if (std::string(I.getOpcodeName()) != "getelementptr")
        return;

    GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(&I);
    if (!GEP->getSourceElementType()->isStructTy())
        return;

    std::string structName = GEP->getSourceElementType()->getStructName().str();
    // Get the structure type
    StructType *StructTy = cast<StructType>(GEP->getSourceElementType());
    if (memberAccessCounts.count(structName) == 0)
    {
        structs[structName] = StructTy;
        int numFields = StructTy->getNumElements();
        for (int i = 0; i < numFields; i++)
        {
            Type *ty = StructTy->getTypeAtIndex(i);
            memberAccessCounts[structName][i].type = ty;
            memberAccessCounts[structName][i].accessCounts = 0;
            memberAccessCounts[structName][i].size = dataLayout.getTypeAllocSize(ty);
        }
    }

    Value *Operand = GEP->getOperand(GEP->getNumOperands() - 1);
    ConstantInt *Index = dyn_cast<ConstantInt>(Operand);
    if (Index)
    {
        // Extract the integer value
        int IndexValue = Index->getSExtValue();
        memberAccessCounts[structName][IndexValue].accessCounts += numExecuted;
    }
}

void Profiler::profileFunction(llvm::Function &F, llvm::BlockFrequencyAnalysis::Result &bfi, DataLayout &dataLayout)
{
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
            profileInst(I, numExecuted, dataLayout);
        }
    }
}

void Profiler::createSortedMemberVariables()
{
    for (auto &e : memberAccessCounts)
    {
        std::string structName = e.first;
        std::set<std::pair<unsigned, Stat>, ComparePairs> mySet;

        for (auto &s : e.second)
        {
            mySet.insert({s.first, s.second});
        }

        sortedMemberVariables[structName] = mySet;
    }
}

void Profiler::printResult() const
{
    errs() << "Struct Member Access Counts:\n";
    for (const auto &StructEntry : sortedMemberVariables)
    {
        errs() << "Struct: " << StructEntry.first << "\n";
        for (const auto &MemberEntry : StructEntry.second)
        {
            errs() << "  Member " << MemberEntry.first << ": " << MemberEntry.second.accessCounts << " times, Type: "
                   << *MemberEntry.second.type << " Size " << MemberEntry.second.size
                   << "\n";
        }
    }
    errs() << "Struct Instances\n";
    for (const auto &item : structInstances)
    {
        for (const auto &inst : item.second)
        {
            errs() << "  " << *inst << "\n";
        }
    }
}