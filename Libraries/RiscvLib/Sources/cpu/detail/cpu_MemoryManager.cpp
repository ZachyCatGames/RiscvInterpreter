#include <RiscvEmu/cpu/cpu_Result.h>
#include <RiscvEmu/cpu/detail/cpu_MemoryManager.h>

namespace riscv {
namespace cpu {
namespace detail {

namespace {

using MemCtlrT = mem::MemoryController;

} // namespace

Result MemoryManager::Initialize(mem::MemoryController* pMemCtlr) {
    m_pMemCtlr = pMemCtlr;
    m_Mode = AddrTransMode::Bare;
    return ResultSuccess();
}

void MemoryManager::Finalize() {
    m_pMemCtlr = nullptr;
}

template<typename T>
Result MemoryManager::ReadImpl(auto readFunc, T* pOut, Address addr, PrivilageLevel level) {
    Result res;

    /* Translate address if in non-machine mode and translation is enabled. */
    if(level != PrivilageLevel::Machine && m_Mode != AddrTransMode::Bare) {
        res = this->TranslateAddress(&addr, addr, level, MemAccessReason::Read);
        if(res.IsFailure()) {
            return res;
        }
    }

    /* TODO: PMP: Perform PMP check. */

    /* Perform an unmapped read. */
    return (*m_pMemCtlr.*readFunc)(pOut, addr);
}

template<typename T>
Result MemoryManager::WriteImpl(auto writeFunc, T in, Address addr, PrivilageLevel level) {
    Result res;

    /* Translate address if in non-machine mode and translation is enabled. */
    if(level != PrivilageLevel::Machine && m_Mode != AddrTransMode::Bare) {
        res = this->TranslateAddress(&addr, addr, level, MemAccessReason::Write);
        if(res.IsFailure()) {
            return res;
        }
    }

    /* TODO: PMP: Perform PMP check. */

    /* Perform unmapped write. */
    return (*m_pMemCtlr.*writeFunc)(in, addr);
}

Result MemoryManager::TranslateAddress([[maybe_unused]] Address* pOut, [[maybe_unused]] Address addr, [[maybe_unused]] PrivilageLevel level, [[maybe_unused]] MemAccessReason reason) {
    /* TODO: Implement translation. */
    return ResultNotImplemented();
}

Result MemoryManager::ReadByte(Byte* pOut, Address addr, PrivilageLevel level) {
    return this->ReadImpl(&MemCtlrT::ReadByte, pOut, addr, level);
}

Result MemoryManager::ReadHWord(HWord* pOut, Address addr, PrivilageLevel level) {
    return this->ReadImpl(&MemCtlrT::ReadHWord, pOut, addr, level);
}

Result MemoryManager::ReadWord(Word* pOut, Address addr, PrivilageLevel level) {
    return this->ReadImpl(&MemCtlrT::ReadWord, pOut, addr, level);
}

Result MemoryManager::ReadDWord(DWord* pOut, Address addr, PrivilageLevel level) {
    return this->ReadImpl(&MemCtlrT::ReadDWord, pOut, addr, level);
}

Result MemoryManager::WriteByte(Byte in, Address addr, PrivilageLevel level) {
    return this->WriteImpl(&MemCtlrT::WriteByte, in, addr, level);
}

Result MemoryManager::WriteHWord(HWord in, Address addr, PrivilageLevel level) {
    return this->WriteImpl(&MemCtlrT::WriteHWord, in, addr, level);
}

Result MemoryManager::WriteWord(Word in, Address addr, PrivilageLevel level) {
    return this->WriteImpl(&MemCtlrT::WriteWord, in, addr, level);
}

Result MemoryManager::WriteDWord(DWord in, Address addr, PrivilageLevel level) {
    return this->WriteImpl(&MemCtlrT::WriteDWord, in, addr, level);
}

Result MemoryManager::InstFetch(Word* pOut, Address addr, PrivilageLevel level) {
    Result res;

    /* Translate address if in non-machine mode and translation is enabled. */
    if(level != PrivilageLevel::Machine && m_Mode != AddrTransMode::Bare) {
        res = this->TranslateAddress(&addr, addr, level, MemAccessReason::Instruction);
        if(res.IsFailure()) {
            return res;
        }
    }

    /* TODO: PMP: Perform PMP Check. */

    /* Perform unmapped fetch. */
    return m_pMemCtlr->ReadWord(pOut, addr);
}

} // namespace detail
} // namespace cpu
} // namespace riscv
