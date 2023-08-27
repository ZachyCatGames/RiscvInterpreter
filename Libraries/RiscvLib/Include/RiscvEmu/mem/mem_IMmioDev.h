#pragma once
#include <RiscvEmu/riscv_Types.h>

namespace riscv {
namespace mem {

class IMmioDev {
public:
    virtual Result ReadByte(Byte* pOut, Address addr) = 0;
    virtual Result ReadHWord(HWord* pOut, Address addr) = 0;
    virtual Result ReadWord(Word* pOut, Address addr) = 0;
    virtual Result ReadDWord(DWord* pOut, Address addr) = 0;

    virtual Result WriteByte(Byte in, Address addr) = 0;
    virtual Result WriteHWord(HWord in, Address addr) = 0;
    virtual Result WriteWord(Word in, Address addr) = 0;
    virtual Result WriteDWord(DWord in, Address addr) = 0;
}; // class IMmioDev

} // namespace mem
} // namespace riscv
