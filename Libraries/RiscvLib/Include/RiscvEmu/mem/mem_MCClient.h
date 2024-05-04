#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/result.h>

namespace riscv {
namespace mem {

class MemoryController;

class MCClient {
public:
    Result RequestWeakReservation(Address addr);

    template<typename WordType>
    Result Load(WordType* pOut, Address addr);

    template<typename WordType>
    Result Store(WordType in, Address addr);
private:
    MCClient();
    MCClient(MemoryController* pCtlr, int deviceId);
private:
    friend class MemoryController;
    MemoryController* m_pCtlr;
}; // class MCClient

} // namespace mem
} // namespace riscv
