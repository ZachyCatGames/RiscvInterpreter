#pragma once
#include <RiscvEmu/mem/mem_IMmioDev.h>
#include <RiscvEmu/mem/mem_Result.h>
#include <RiscvEmu/mem/mem_MmioTemplateAdapter.h>
#include <concepts>

namespace riscv {
namespace mem {

/**
 * This wraps around another IMmioDev class, ImplT, and performs alignment checks
 * on all Read/Write functions before calling the appropriate function from ImplT.
 * 
 * This removes the need for duplicating alignment checks.
 * 
 * @param ImplT  IMmioDev class to wrap around.
*/
template<std::derived_from<IMmioDev> ImplT>
class AlignedMmioDev : public IMmioDev {
public:
    virtual NativeWord GetMappedSize() override { return m_Impl.GetMappedSize(); }

    virtual Result LoadByte(Byte* pOut, Address addr) override { return this->LoadImpl(pOut, addr); }
    virtual Result LoadHWord(HWord* pOut, Address addr) override { return this->LoadImpl(pOut, addr); }
    virtual Result LoadWord(Word* pOut, Address addr) override { return this->LoadImpl(pOut, addr); }
    virtual Result LoadDWord(DWord* pOut, Address addr) override { return this->LoadImpl(pOut, addr); }

    virtual Result StoreByte(Byte in, Address addr) override { return this->StoreImpl(in, addr); }
    virtual Result StoreHWord(HWord in, Address addr) override { return this->StoreImpl(in, addr); }
    virtual Result StoreWord(Word in, Address addr) override { return this->StoreImpl(in, addr); }
    virtual Result StoreDWord(DWord in, Address addr) override { return this->StoreImpl(in, addr); }
private:
    template<typename WordType>
    Result LoadImpl(WordType* pOut, Address addr) {
        if(addr % sizeof(WordType)) {
            return ResultBadMisalignedAddress();
        }
        return MmioTemplateAdapter(&m_Impl).template Load<WordType>(pOut, addr);
    }

    template<typename WordType>
    Result StoreImpl(WordType in, Address addr) {
        if(addr % sizeof(WordType)) {
            return ResultBadMisalignedAddress();
        }
        return MmioTemplateAdapter(&m_Impl).template Store<WordType>(in, addr);
    }
private:
    ImplT m_Impl;
}; // class AlignedMmioDev

} // namespace mem
} // namespace riscv
