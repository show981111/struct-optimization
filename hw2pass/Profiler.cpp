#include "Profiler.h"

void Profiler::profileInst(Instruction &I, uint64_t numExecuted)
{
    if (std::string(I.getOpcodeName()) == "alloca")
    {
        // If loading the struct instance, grab it!
        AllocaInst *allocInst = dyn_cast<AllocaInst>(&I);
        if (allocInst->getAllocatedType()->isStructTy())
        {
            StructType *StructTy = cast<StructType>(allocInst->getAllocatedType());
            structInstances[StructTy->getStructName().str()].insert(allocInst);
            errs() << "Usage of " << *allocInst << "\n";
            for (auto U : allocInst->users())
            {
                errs() << " Heelo \n";
                if (auto usingInst = dyn_cast<Instruction>(U))
                {
                    errs() << *usingInst << "\n";
                }
            }
            errs() << "----\n";
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

    Value *Operand = GEP->getOperand(GEP->getNumOperands() - 1);
    ConstantInt *Index = dyn_cast<ConstantInt>(Operand);
    if (Index)
    {
        // Extract the integer value
        int IndexValue = Index->getSExtValue();
        // StructTy->getTypeAtIndex(IndexValue)->print(errs());
        // Now, IndexValue contains the value (in this case, 3)
        memberAccessCounts[structName][IndexValue].accessCounts += numExecuted;
    }
}

void Profiler::profileFunction(llvm::Function &F, llvm::BlockFrequencyAnalysis::Result &bfi)
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
            profileInst(I, numExecuted);
        }
    }
}

void Profiler::createSortedMemberVariables()
{
    for (auto &e : memberAccessCounts)
    {
        std::string structName = e.first;
        std::vector<std::pair<unsigned, Profiler::Stat>> vec;
        for (auto &s : e.second)
        {
            vec.push_back({s.first, s.second});
        }

        std::sort(vec.begin(), vec.end(), [](const auto &a, const auto &b)
                  { return a.second.accessCounts > b.second.accessCounts; });
        sortedMemberVariables[structName] = std::move(vec);
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
            errs() << "  Member " << MemberEntry.first << ": " << MemberEntry.second.accessCounts << " times, Type: " << *MemberEntry.second.type << "\n";
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