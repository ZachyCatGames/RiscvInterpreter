#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/mem/mem_IMmioDev.h>
#include <RiscvEmu/mem/detail/mem_MemoryDeviceImpl.h>
#include <memory>

namespace riscv {
namespace mem {

/**
 * This is IO device which provides additional read/write volatile memory.
*/
class MemoryDevice : public IMmioDev {
public:
    /**
     * Construct a MemoryDevice object with a given size.
     * 
     * @param[in] length  Amount of memory to allocate & provide.
    */
    MemoryDevice(NativeWord length) :
        m_Dev(length),
        m_Length(length) {}

    virtual NativeWord GetMappedSize() override { return m_Length; }

    virtual Result LoadByte(Byte* pOut, Address addr) override;
    virtual Result LoadHWord(HWord* pOut, Address addr) override;
    virtual Result LoadWord(Word* pOut, Address addr) override;
    virtual Result LoadDWord(DWord* pOut, Address addr) override;

    virtual Result StoreByte(Byte in, Address addr) override;
    virtual Result StoreHWord(HWord in, Address addr) override;
    virtual Result StoreWord(Word in, Address addr) override;
    virtual Result StoreDWord(DWord in, Address addr) override;
private:
    detail::MemoryDeviceImpl m_Dev;
    NativeWord m_Length;
}; // class MemoryDevice

} // namespace mem
} // namespace riscv
