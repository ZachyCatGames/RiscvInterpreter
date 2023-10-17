#pragma once
#include <RiscvEmu/cpu/cpu_AccessReason.h>
#include <RiscvEmu/cpu/cpu_AddrTransMode.h>
#include <RiscvEmu/cpu/cpu_PrivilageLevel.h>
#include <RiscvEmu/mem/mem_MemoryController.h>

namespace riscv {
namespace cpu {
namespace detail {

class MemoryManager {
public:
    MemoryManager() :
        m_pMemCtlr(nullptr) {}

    MemoryManager(mem::MemoryController* pMemCtlr) :
        m_pMemCtlr(pMemCtlr),
        m_Mode(AddrTransMode::Bare) {}

    Result Initialize(mem::MemoryController* pMemCtlr);
    void Finalize();

    bool SetTransMode(AddrTransMode mode);

    Result ReadByte(Byte* pOut, Address addr, PrivilageLevel level);
    Result ReadHWord(HWord* pOut, Address addr, PrivilageLevel level);
    Result ReadWord(Word* pOut, Address addr, PrivilageLevel level);
    Result ReadDWord(DWord* pOut, Address addr, PrivilageLevel level);

    Result WriteByte(Byte in, Address addr, PrivilageLevel level);
    Result WriteHWord(HWord in, Address addr, PrivilageLevel level);
    Result WriteWord(Word in, Address addr, PrivilageLevel level);
    Result WriteDWord(DWord in, Address addr, PrivilageLevel level);

    Result InstFetch(Word* pOut, Address addr, PrivilageLevel level);
private:
    template<typename T>
    Result ReadImpl(auto readFunc, T* pOut, Address addr, PrivilageLevel level);

    template<typename T>
    Result WriteImpl(auto writeFunc, T in, Address addr, PrivilageLevel level);

    Result TranslateAddress(Address* pOut, Address addr, PrivilageLevel level, MemAccessReason reason);
private:
    mem::MemoryController* m_pMemCtlr;
    AddrTransMode m_Mode;
}; // class MemoryManager

} // namespace detail
} // namespace cpu
} // namespace detail
