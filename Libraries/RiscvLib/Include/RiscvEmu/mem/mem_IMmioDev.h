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

    virtual Result LoadByte(Byte* pOut, Address addr);
    virtual Result LoadHWord(HWord* pOut, Address addr);
    virtual Result LoadWord(Word* pOut, Address addr);
    virtual Result LoadDWord(DWord* pOut, Address addr);

    virtual Result StoreByte(Byte in, Address addr);
    virtual Result StoreHWord(HWord in, Address addr);
    virtual Result StoreWord(Word in, Address addr);
    virtual Result StoreDWord(DWord in, Address addr);
}; // class IMmioDev

} // namespace mem
} // namespace riscv
