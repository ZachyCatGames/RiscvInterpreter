#include <RiscvEmuTest/test_HartTestSystem.h>

namespace riscv {
namespace test {

Result HartTestSystem::Initialize() {
    /* Initialize the Hart. */
    cpu::Hart::SharedContext hartShared(&m_MemCtlr);
    Result res = m_Hart.Initialize(hartShared);
    if(res.IsFailure()) {
        return res;
    }

    /* Reset the Hart. */
    res = m_Hart.Reset();
    if(res.IsFailure()) {
        return res;
    }

    static constexpr mem::RegionInfo memReg{ MemoryAddress, MemorySize, mem::RegionType::Memory };
    res = m_MemCtlr.Initialize(&memReg, 1);
    if(res.IsFailure()) {
        return res;
    }

    return ResultSuccess();
}

} // namespace test
} // namespace riscv
