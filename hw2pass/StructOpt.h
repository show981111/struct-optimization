#include "llvm/IR/DataLayout.h"

#include "Profiler.h"
#include <vector>

using namespace llvm;

class StructOpt
{
private:
    struct StructInfo
    {
        uint64_t alignment;
        std::vector<Type *> members;
    };
    std::unordered_map<std::string, std::unordered_map<std::string, StructInfo>> subStructMap;
    std::unordered_map<std::string, std::unordered_map<MemberIndex, std::string>> memberToSubstruct;
    std::unordered_map<AllocaInst *, std::vector<AllocaInst *>> originalInstanceToNewInstances;

    /**
     * Create the new ordering of members in struct.
     * Key: Struct name, Vector index: original member var's index, Vector value: new index
     * Ex)
     * Test{            Test{
     *   int a;            int b;
     *   int b;     =>     int a;
     * }                }
     * map["Test"] = [1,0] // a becomes second member(index 1), and b becomes first member(index 0)
     */
    std::unordered_map<std::string, std::vector<int>> getNewOrderOfStruct();
    void fixIndicesBasedOnNewOrder(GetElementPtrInst *GEP, std::unordered_map<std::string, std::vector<int>> &);

public:
    Profiler profiler;

    StructOpt(){};
    void structReordering(llvm::Function &F);

    /**
     * Effect: Should create subStructMap & memberToSubstruct
     */
    void createSubStructMap(DataLayout &dataLayout);

    void printSubStructMap();
    void addStructDeclaration(Module &M, LLVMContext &Context);
    void addNewInstanceDeclaration(Module &M, LLVMContext &Context);

    void fixUsagesOfInstance();
};