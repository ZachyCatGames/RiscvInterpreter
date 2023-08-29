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
class MemoryDevice : public IMmioDev, private detail::MemoryDeviceImpl<std::unique_ptr<Byte[]>> {
public:
    /**
     * Construct a MemoryDevice object with a given size.
     * 
     * @param[in] length  Amount of memory to allocate & provide.
    */
    constexpr MemoryDevice(NativeWord length) :
        MemoryDeviceImpl(std::make_unique<Byte[]>(length)),
        m_Length(length) {}

    constexpr virtual NativeWord GetMappedSize() override { return m_Length; }

    constexpr virtual Result ReadByte  (Byte* pOut, Address addr)  override { return this->ReadByteImpl(pOut, addr); }
    constexpr virtual Result ReadHWord (HWord* pOut, Address addr) override { return this->ReadHWordImpl(pOut, addr); }
    constexpr virtual Result ReadWord  (Word* pOut, Address addr)  override { return this->ReadWordImpl(pOut, addr); }
    constexpr virtual Result ReadDWord (DWord* pOut, Address addr) override { return this->ReadDWordImpl(pOut, addr); }
    constexpr virtual Result WriteByte (Byte in, Address addr)     override { return this->WriteByteImpl(in, addr); }
    constexpr virtual Result WriteHWord(HWord in, Address addr)    override { return this->WriteHWordImpl(in, addr); }
    constexpr virtual Result WriteWord (Word in, Address addr)     override { return this->WriteWordImpl(in, addr); }
    constexpr virtual Result WriteDWord(DWord in, Address addr)    override { return this->WriteDWordImpl(in, addr); }
private:
    NativeWord m_Length;
};

} // namespace mem
} // namespace riscv
