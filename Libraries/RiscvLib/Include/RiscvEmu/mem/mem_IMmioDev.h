#pragma once
#include <RiscvEmu/result.h>
#include <RiscvEmu/riscv_Peripheral.h>
#include <RiscvEmu/riscv_Types.h>

namespace riscv {
namespace mem {

/**
 * This interface represents an mmio device which may be attached to a MemoryController.
 * 
 * All functions must be implemented. An implementation should return ResultSuccess
 * if they don't wish to implement one or more functions.
 * 
 * Implementations should return ResultBadMisalignedAddress if they doesn't support
 * misaligned addresses when a misaligned address is given to them.
*/
class IMmioDev : public Peripheral {
public:
    virtual ~IMmioDev() = default;

    /** Retrieve the size of a device's address space. */
    virtual NativeWord GetMappedSize() = 0;

    /** Read a single byte. */
    virtual Result ReadByte(Byte* pOut, Address addr) = 0;

    /** Read a single half word. */
    virtual Result ReadHWord(HWord* pOut, Address addr) = 0;

    /** Read a single word. */
    virtual Result ReadWord(Word* pOut, Address addr) = 0;

    /** Read a single double word. */
    virtual Result ReadDWord(DWord* pOut, Address addr) = 0;

    /* Write a single byte. */
    virtual Result WriteByte(Byte in, Address addr) = 0;

    /** Write a single half word. */
    virtual Result WriteHWord(HWord in, Address addr) = 0;

    /** Write a single word. */
    virtual Result WriteWord(Word in, Address addr) = 0;

    /** Write a single double word. */
    virtual Result WriteDWord(DWord in, Address addr) = 0;
}; // class IMmioDev

} // namespace mem
} // namespace riscv
