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

    Result ExecuteInstruction(cpu::Instruction inst) {
        return m_Hart.ExecuteInstruction(inst);
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

    bool AssertGPR(int index, NativeWord value) const noexcept {
        return m_Hart.ReadGPR(index) == value;
    }

    bool AssertPhysMemByte(Address addr, Byte val);
    bool AssertPhysMemHWord(Address addr, HWord val);
    bool AssertPhysMemWord(Address addr, Word val);
    bool AssertPhysMemDWord(Address addr, Word val);

    bool AssertVirtMemByte(Address addr, Byte val);
    bool AssertVirtMemHWord(Address addr, HWord val);
    bool AssertVirtMemWord(Address addr, Word val);
    bool AssertVirtMemDWord(Address addr, Word val);

    /* Access physical memory. */
    Result MemReadByte(Byte* pOut, Address addr)   { return m_MemCtlr.ReadByte(pOut, addr); }
    Result MemReadHWord(HWord* pOut, Address addr) { return m_MemCtlr.ReadHWord(pOut, addr); }
    Result MemReadWord(Word* pOut, Address addr)   { return m_MemCtlr.ReadWord(pOut, addr); }
    Result MemReadDWord(DWord* pOut, Address addr) { return m_MemCtlr.ReadDWord(pOut, addr); }
    Result MemWriteByte(Byte in, Address addr)     { return m_MemCtlr.WriteByte(in, addr); }
    Result MemWriteHWord(HWord in, Address addr)   { return m_MemCtlr.WriteHWord(in, addr); }
    Result MemWriteWord(Word in, Address addr)     { return m_MemCtlr.WriteWord(in, addr); }
    Result MemWriteDWord(DWord in, Address addr)   { return m_MemCtlr.WriteDWord(in, addr); }

    /* Access memory as the hart. */
    /* TODO: Properly implement to bypass memory restrictions. */
    Result HartReadByte(Byte* pOut, Address addr)   { return m_Hart.MemoryReadByte(pOut, addr); }
    Result HartReadHWord(HWord* pOut, Address addr) { return m_Hart.MemoryReadHWord(pOut, addr); }
    Result HartReadWord(Word* pOut, Address addr)   { return m_Hart.MemoryReadWord(pOut, addr); }
    Result HartReadDWord(DWord* pOut, Address addr) { return m_Hart.MemoryReadDWord(pOut, addr); }
    Result HartWriteByte(Byte in, Address addr)     { return m_Hart.MemoryWriteByte(in, addr); }
    Result HartWriteHWord(HWord in, Address addr)   { return m_Hart.MemoryWriteHWord(in, addr); }
    Result HartWriteWord(Word in, Address addr)     { return m_Hart.MemoryWriteWord(in, addr); }
    Result HartWriteDWord(DWord in, Address addr)   { return m_Hart.MemoryWriteDWord(in, addr); }

    static Result DefaultReset(HartTestSystem* pSys);

private:
    cpu::Hart m_Hart;
    mem::MemoryController m_MemCtlr;
}; // class HartTestSystem

} // namespace test
} // namespace riscv
