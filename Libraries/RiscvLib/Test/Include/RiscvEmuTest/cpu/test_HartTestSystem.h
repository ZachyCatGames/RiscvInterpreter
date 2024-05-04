#pragma once
#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/mem/mem_MemoryController.h>
#include <RiscvEmuTest/test_Result.ext.h>

namespace riscv {
namespace test {

class HartTestSystem {
public:
    static constexpr auto MemoryAddress = 0x100000;
    static constexpr auto MemorySize    = 0x100000;
public:
    Result Initialize();

    auto GetHart() noexcept { return &m_Hart; }

    Result ExecuteInst(cpu::Instruction inst) {
        return m_Hart.ExecuteInst(inst);
    }

    void WritePC(NativeWord val) { m_Hart.WritePC(val); }
    void WriteGPR(int index, NativeWord val) { m_Hart.WriteGPR(index, val); }
    void WriteGPR(std::pair<int, NativeWord> pair) { m_Hart.WriteGPR(std::get<0>(pair), std::get<1>(pair)); }

    NativeWord ReadPC() { return m_Hart.ReadPC(); }
    NativeWord ReadGPR(int index) { return m_Hart.ReadGPR(index); }

    Result CheckGPR(int index, NativeWord val) {
        if(val != m_Hart.ReadGPR(index)) {
            return ResultRegValMismatch();
        }
        return ResultSuccess();
    }
    Result CheckGPR(std::pair<int, NativeWord> pair) { return this->CheckGPR(std::get<0>(pair), std::get<1>(pair)); }

    void ClearGPRs() noexcept;

    /* Access physical memory. */
    template<typename WordType>
    Result MemLoad(WordType* pOut, Address addr) { return m_McClient.Load(pOut, addr); }
    template<typename WordType>
    Result MemStore(WordType in, Address addr) { return m_McClient.Store(in, addr); }

    /* Access memory as the hart. */
    /* TODO: Properly implement to bypass memory restrictions. */
    template<typename WordType>
    Result MemMappedLoad(WordType* pOut, Address addr) { return m_Hart.MemMappedLoadForDebug(pOut, addr); }
    template<typename WordType>
    Result MemMappedStore(WordType in, Address addr) { return m_Hart.MemMappedStoreForDebug(in, addr); }

    Result ClearMem();

    static Result DefaultReset(HartTestSystem* pSys);

private:
    cpu::Hart::SharedState m_HartSharedState;
    cpu::Hart m_Hart;
    mem::MemoryController m_MemCtlr;
    mem::MCClient m_McClient;
}; // class HartTestSystem

} // namespace test
} // namespace riscv
