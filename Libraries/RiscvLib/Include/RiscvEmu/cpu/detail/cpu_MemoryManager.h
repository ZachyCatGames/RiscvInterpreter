#pragma once
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

    bool SetTransMode(AddrTransMode mode) noexcept;

    void SetEnabledSUM(bool val) noexcept;

    Result ReadByte(Byte* pOut, Address addr, PrivilageLevel level);
    Result ReadHWord(HWord* pOut, Address addr, PrivilageLevel level);
    Result ReadWord(Word* pOut, Address addr, PrivilageLevel level);
    Result ReadDWord(DWord* pOut, Address addr, PrivilageLevel level);

    Result WriteByte(Byte in, Address addr, PrivilageLevel level);
    Result WriteHWord(HWord in, Address addr, PrivilageLevel level);
    Result WriteWord(Word in, Address addr, PrivilageLevel level);
    Result WriteDWord(DWord in, Address addr, PrivilageLevel level);

    Result InstFetch(Word* pOut, Address addr, PrivilageLevel level);

    Result MappedReadByte(Byte* pOut, Address addr);
    Result MappedReadHWord(HWord* pOut, Address addr);
    Result MappedReadWord(Word* pOut, Address addr);
    Result MappedReadDWord(DWord* pOut, Address addr);

    Result MappedWriteByte(Byte in, Address addr);
    Result MappedWriteHWord(HWord in, Address addr);
    Result MappedWriteWord(Word in, Address addr);
    Result MappedWriteDWord(DWord in, Address addr);
private:
    template<typename T>
    Result ReadImpl(auto readFunc, T* pOut, Address addr, PrivilageLevel level);

    template<typename T>
    Result WriteImpl(auto writeFunc, T in, Address addr, PrivilageLevel level);

    template<typename T>
    Result MappedReadImpl(auto readFunc, T* pOut, Address addr);

    template<typename T>
    Result MappedWriteImpl(auto readFunc, T in, Address addr);

    Result GetPteImpl(NativeWord* pPte, Address* pPteAddr, int* pLevelFound, Address addr);

    Result TranslateImpl(Address* pAddrOut, Address addr, PrivilageLevel level, auto chkFunc);

    Result TranslateForRead(Address* pOut, Address addr, PrivilageLevel level);
    Result TranslateForWrite(Address* pOut, Address addr, PrivilageLevel level);
    Result TranslateForFetch(Address* pOut, Address addr, PrivilageLevel level);
    Result TranslateForAny(Address* pOut, Address addr, PrivilageLevel level);
private:
    mem::MemoryController* m_pMemCtlr;
    Address m_PageTableAddr;

    AddrTransMode m_Mode;
    int m_PTLevelCount;

    bool m_SUMEnabled;
}; // class MemoryManager

} // namespace detail
} // namespace cpu
} // namespace detail
