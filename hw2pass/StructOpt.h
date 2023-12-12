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
    struct MemberDetail
    {
        MemberIndex index;
        std::string substructName;
    };
    struct subStructInfo
    {
        int size;
        int mvId;
        Type *type;
        subStructInfo(int size_in, int mvId_in, Type *type_in)
        {
            size = size_in;
            mvId = mvId_in;
            type = type_in;
        }
    };
    struct CompareSubStruct
    {
        bool operator()(const subStructInfo &lhs, const subStructInfo &rhs) const
        {
            if (lhs.size != rhs.size)
            {
                return lhs.size < rhs.size;
            }
            else
            {
                return lhs.mvId < rhs.mvId;
            }
        }
    };
    const double HOTNESS_THRESHOLD = 0.4;
    /**
     * Test -> Test_0 : <int ,long ,int>
     *      -> Test_1 : <int, char,long,long >
     */
    std::unordered_map<std::string, std::unordered_map<std::string, StructInfo>> subStructMap;
    /**
     * Test -> 0 -> 1(new Index in substruct), Test_0 (Name of new substruct)
     *      -> 1 -> 0(new Index in substruct), Test_1 (Name of new substruct)
     *      -> 2
     */
    std::unordered_map<std::string, std::unordered_map<MemberIndex, MemberDetail>> memberToSubstruct;
    std::unordered_map<AllocaInst *, std::unordered_map<std::string, AllocaInst *>> originalInstanceToNewInstances;
    std::unordered_map<AllocaInst *, std::unordered_map<std::string, AllocaInst *>> originalInstanceToNewInstancesArr;

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
    static void SetIndexOffsetOfGEP(unsigned int operandIndex, unsigned int newIndex, GetElementPtrInst *GEP, IRBuilder<> &Builder);

    /**
     * Different ways of peeling
     */
    void peelTop3Elems(DataLayout &dataLayout);
    void peelBasedOnHotnessThreshold(DataLayout &dataLayout);
    void printErrors(GetElementPtrInst *GEP_array, GetElementPtrInst *GEP);

public:
    Profiler profiler;

    StructOpt(){};
    void structReordering(llvm::Function &F);

    /**
     * Effect: Should create subStructMap & memberToSubstruct
     * Struct peeing is based on those two variables
     */
    void createSubStructMap(DataLayout &dataLayout);

    void printSubStructMap();

    void addStructDeclaration(Module &M, LLVMContext &Context);
    void addNewInstanceDeclaration(Module &M, LLVMContext &Context);
    void fixUsagesOfInstance();

    void addNewArrayInstanceDeclaration(Module &M, LLVMContext &Context);
    void fixArrayInstanceUsage();
};