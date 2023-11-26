#include "StructOpt.h"

std::unordered_map<std::string, std::vector<int>> StructOpt::getNewOrderOfStruct()
{
    std::unordered_map<std::string, std::vector<int>> fieldIndexMapping;

    for (auto &st : profiler.structs)
    {
        if (!st.second->isOpaque())
        {
            std::string structName = st.first;
            std::vector<Type *> sortedFields;
            std::vector<int> fieldMap(profiler.sortedMemberVariables[structName].size());
            for (std::pair<unsigned, Profiler::Stat> &item : profiler.sortedMemberVariables[structName])
            {
                fieldMap[item.first] = sortedFields.size();
                sortedFields.push_back(item.second.type);
            }
            fieldIndexMapping[structName] = std::move(fieldMap);
            st.second->setBody(sortedFields);
        }

        errs() << "After Reordering: " << *st.second << "\n";
    }
    return fieldIndexMapping;
};

void StructOpt::fixIndicesBasedOnNewOrder(GetElementPtrInst *GEP, std::unordered_map<std::string, std::vector<int>> &fieldIndexMapping)
{
    if (GEP->getSourceElementType()->isStructTy())
    {
        std::string structName = GEP->getSourceElementType()->getStructName().str();
        IRBuilder<> Builder(GEP->getContext());
        Value *Operand = GEP->getOperand(GEP->getNumOperands() - 1);
        ConstantInt *Index = dyn_cast<ConstantInt>(Operand);

        int newIndex = fieldIndexMapping[structName][Index->getSExtValue()];
        if (Index->getBitWidth() == 32)
        {
            GEP->setOperand(GEP->getNumOperands() - 1, Builder.getInt32(newIndex));
        }
        else if (Index->getBitWidth() == 8)
        {
            GEP->setOperand(GEP->getNumOperands() - 1, Builder.getInt8(newIndex));
        }
        else if (Index->getBitWidth() == 16)
        {
            GEP->setOperand(GEP->getNumOperands() - 1, Builder.getInt16(newIndex));
        }
        else if (Index->getBitWidth() == 64)
        {
            GEP->setOperand(GEP->getNumOperands() - 1, Builder.getInt64(newIndex));
        }
    }
}

void StructOpt::structReordering(llvm::Function &F)
{
    std::unordered_map<std::string, std::vector<int>> fieldIndexMapping = getNewOrderOfStruct();

    for (auto &BB : F)
    {
        for (auto &I : BB)
        {
            if (std::string(I.getOpcodeName()) == "getelementptr")
            {
                GetElementPtrInst *ptrInst = dyn_cast<GetElementPtrInst>(&I);
                fixIndicesBasedOnNewOrder(ptrInst, fieldIndexMapping);
            }
        }
    }
}

void StructOpt::createSubStructMap(DataLayout &dataLayout)
{
    for (auto &item : profiler.sortedMemberVariables)
    {
        std::string structName = item.first;
        std::vector<Type *> members_0;
        std::vector<Type *> members_1;
        std::string substruct0 = structName + "_0";
        std::string substruct1 = structName + "_1";
        uint64_t maxMemberSizeOfM0 = 0;
        uint64_t maxMemberSizeOfM1 = 0;

        if (item.second.size() < 5)
            break;

        for (auto &member : item.second)
        {
            llvm::Align alignment = dataLayout.getABITypeAlign(member.second.type);

            if (members_0.size() < 3) // top 3 goes to the subStruct
            {
                members_0.push_back(member.second.type);
                maxMemberSizeOfM0 = std::max(maxMemberSizeOfM0, alignment.value());
                memberToSubstruct[structName][member.first] = substruct0;
            }
            else
            {
                members_1.push_back(member.second.type);
                maxMemberSizeOfM1 = std::max(maxMemberSizeOfM1, alignment.value());
                memberToSubstruct[structName][member.first] = substruct1;
            }
        }

        subStructMap[structName][substruct0].members = std::move(members_0);
        subStructMap[structName][substruct0].alignment = maxMemberSizeOfM0;
        subStructMap[structName][substruct1].members = std::move(members_1);
        subStructMap[structName][substruct1].alignment = maxMemberSizeOfM1;
    }
}

void StructOpt::addStructDeclaration(Module &M, LLVMContext &Context)
{
    for (auto &item : subStructMap)
    {
        for (auto &sub : item.second)
        {
            llvm::ArrayRef<llvm::Type *> members(sub.second.members);
            StructType *subStruct = StructType::create(members, sub.first);
            errs() << "Created struct type: " << *subStruct << "\n";
            profiler.structs[sub.first] = subStruct;

            // Temporary: If Struct is not used, it is removed from the IR
            GlobalVariable *MyStructVar =
                new GlobalVariable(M, subStruct, false, GlobalValue::InternalLinkage,
                                   Constant::getNullValue(subStruct), "myStructVar");
        }
    }
}

void StructOpt::addNewInstanceDeclaration(Module &M, LLVMContext &Context)
{
    for (const auto &item : profiler.structInstances)
    {
        if (subStructMap.count(item.first) > 0)
        {
            for (const auto &originalInstance : item.second)
            {
                errs() << *originalInstance << "\n";
                IRBuilder<> builder(originalInstance);
                for (const auto &substruct : subStructMap[item.first])
                {
                    AllocaInst *allocaInst = builder.CreateAlloca(profiler.structs[substruct.first], nullptr);
                    allocaInst->setAlignment(llvm::Align(substruct.second.alignment));
                    originalInstanceToNewInstances[originalInstance].push_back(allocaInst);
                }
            }
        }
    }
}

void StructOpt::fixUsagesOfInstance()
{
    for (auto &item : originalInstanceToNewInstances)
    {
        errs() << "Usage of " << *(item.first) << "\n";
        for (auto U : item.first->users())
        {
            if (auto usingInst = dyn_cast<Instruction>(U))
            {
                if (std::string(usingInst->getOpcodeName()) == "getelementptr")
                {
                    // TODO: Replace ptr based on member it is accessing using memberToSubstruct
                }
                else
                {
                    errs() << *usingInst << "\n";
                }
            }
        }
    }
}

void StructOpt::printSubStructMap()
{
    errs() << "SubStructMap\n";

    for (auto &item : subStructMap)
    {
        errs() << item.first << "\n";

        for (auto &sub : item.second)
        {
            errs() << "  " << sub.first << " : ";
            for (auto &member : sub.second.members)
            {
                errs() << *member << ", ";
            }
            errs() << "\n";
        }
    }
}