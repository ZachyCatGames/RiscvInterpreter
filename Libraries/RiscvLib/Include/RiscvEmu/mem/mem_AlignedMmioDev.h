#pragma once
#include <RiscvEmu/mem/mem_IMmioDev.h>
#include <RiscvEmu/mem/mem_Result.h>
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
    constexpr virtual NativeWord GetMappedSize() override { return m_Impl.GetMappedSize(); } 

    constexpr virtual Result ReadByte(Byte* pOut, Address addr) override {
        return m_Impl.ReadByte(pOut, addr);
    }

    constexpr virtual Result ReadHWord(HWord* pOut, Address addr) override {
        return this->CallReadImpl(&ImplT::ReadHWord, pOut, addr);
    }

    constexpr virtual Result ReadWord(Word* pOut, Address addr) override {
        return this->CallReadImpl(&ImplT::ReadWord, pOut, addr);
    }

    constexpr virtual Result ReadDWord(DWord* pOut, Address addr) override {
        return this->CallReadImpl(&ImplT::ReadDWord, pOut, addr);
    }

    constexpr virtual Result WriteByte(Byte in, Address addr) override {
        return m_Impl.WriteByte(in, addr);
    }

    constexpr virtual Result WriteHWord(HWord in, Address addr) override {
        return this->CallWriteImpl(&ImplT::WriteHWord, in, addr);
    }

    constexpr virtual Result WriteWord(Word in, Address addr) override {
        return this->CallWriteImpl(&ImplT::WriteWord, in, addr);
    }

    constexpr virtual Result WriteDWord(DWord in, Address addr) override {
        return this->CallWriteImpl(&ImplT::WriteDWord, in, addr);
    }
private:
    template<typename WordT>
    constexpr Result CallReadImpl(auto func, WordT* pOut, Address addr) {
        if(addr % sizeof(WordT)) {
            return (m_Impl.*func)(pOut, addr);
        }
        return ResultBadMisalignedAddress();
    }

    template<typename WordT>
    constexpr Result CallWriteImpl(auto func, WordT in, Address addr) {
        if(addr % sizeof(WordT)) {
            return (m_Impl.*func)(in, addr);
        }
        return ResultBadMisalignedAddress();
    }
private:
    ImplT m_Impl;
}; // class AlignedMmioDev

} // namespace mem
} // namespace riscv
