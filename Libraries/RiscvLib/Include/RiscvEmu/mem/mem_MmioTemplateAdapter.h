#pragma once
#include <RiscvEmu/mem/mem_IMmioDev.h>

namespace riscv {
namespace mem {

template<std::derived_from<IMmioDev> T>
class MmioTemplateAdapter {
public:
    constexpr MmioTemplateAdapter(T* pDev) : m_pDev(pDev) {}

    template<typename WordType>
    Result Load(WordType* pOut, Address addr);
    template<>
    Result Load<Byte>(Byte* pOut, Address addr) { return m_pDev->LoadByte(pOut, addr); }
    template<>
    Result Load<HWord>(HWord* pOut, Address addr) { return m_pDev->LoadHWord(pOut, addr); }
    template<>
    Result Load<Word>(Word* pOut, Address addr) { return m_pDev->LoadWord(pOut, addr); }
    template<>
    Result Load<DWord>(DWord* pOut, Address addr) { return m_pDev->LoadDWord(pOut, addr); }

    template<typename WordType>
    Result Store(WordType in, Address addr);
    template<>
    Result Store<Byte>(Byte in, Address addr) { return m_pDev->StoreByte(in, addr); }
    template<>
    Result Store<HWord>(HWord in, Address addr) { return m_pDev->StoreHWord(in, addr); }
    template<>
    Result Store<Word>(Word in, Address addr) { return m_pDev->StoreWord(in, addr); }
    template<>
    Result Store<DWord>(DWord in, Address addr) { return m_pDev->StoreDWord(in, addr); }
private:
    T* m_pDev;
}; // class MmioTemplateAdapter

} // namespace mem
} // namespace riscv
