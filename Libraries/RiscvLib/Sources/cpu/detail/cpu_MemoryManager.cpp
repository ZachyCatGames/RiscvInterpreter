#include <RiscvEmu/cpu/cpu_Result.h>
#include <RiscvEmu/cpu/detail/cpu_MemoryManager.h>
#include <RiscvEmu/diag.h>
#include <RiscvEmu/util/util_Bitfields.h>

namespace riscv {
namespace cpu {
namespace detail {

namespace {

using MemCtlrT = mem::MemoryController;

class PTEBase {
public:
    constexpr PTEBase() noexcept : m_Value(0) {}
    constexpr PTEBase(NativeWord val) noexcept : m_Value(val) {}

    constexpr NativeWord GetValue() const noexcept { return m_Value; }

    constexpr bool GetValid() const noexcept { return this->GetBit(0); }
    constexpr void SetValid(bool val) noexcept { this->SetBit(0, val); }

    constexpr bool GetReadable() const noexcept { return this->GetBit(1); }
    constexpr void SetReadable(bool val) noexcept { this->SetBit(1, val); }

    constexpr bool GetWriteable() const noexcept { return this->GetBit(2); }
    constexpr void SetWriteable(bool val) noexcept { this->SetBit(2, val); }

    constexpr bool GetExecutable() const noexcept { return this->GetBit(3); }
    constexpr void SetExecutable(bool val) noexcept { this->SetBit(3, val); }

    constexpr bool GetForUser() const noexcept { return this->GetBit(4); }
    constexpr void SetForUser(bool val) noexcept { this->SetBit(4, val); }

    constexpr bool GetGlobal() const noexcept { return this->GetBit(5); }
    constexpr void SetGlobal(bool val) noexcept { this->SetBit(5, val); }

    constexpr bool GetAccessed() const noexcept { return this->GetBit(6); }
    constexpr void SetAccessed(bool val) noexcept { this->SetBit(6, val); }

    constexpr bool GetDirty() const noexcept { return this->GetBit(7); }
    constexpr void SetDirty(bool val) noexcept { this->SetBit(7, val); }

    constexpr bool IsLeaf() const noexcept {
        return ((m_Value >> 1) & 0x7) != 0;
    }
protected:
    constexpr bool GetBit(int index) const noexcept { return util::ExtractBitfield(m_Value, index, 1); }
    constexpr void SetBit(int index, bool val) noexcept {
        m_Value = util::AssignBitfield(m_Value, index, 1, static_cast<NativeWord>(val));
    }
protected:
    NativeWord m_Value;
}; // class PTEBase

class PTEFor32 : public PTEBase {
public:
    constexpr PTEFor32() noexcept : PTEBase() {}
    constexpr PTEFor32(NativeWord val) noexcept : PTEBase(val) {}

    constexpr NativeWord GetPPN() const noexcept { return m_Value & (0x3FFFFFu << 10); }
}; // class PTEFor32

class PTEFor64 : public PTEBase {
public:
    constexpr PTEFor64() noexcept : PTEBase() {}
    constexpr PTEFor64(NativeWord val) noexcept : PTEBase(val) {}

    constexpr NativeWord GetPPN() const noexcept { return m_Value & (0xFFFFFFFFFFFull << 10); }

    constexpr NativeWord GetPBMT() const noexcept { return util::ExtractBitfield(m_Value, 61, 2); }

    constexpr bool GetN() const noexcept { return this->GetBit(63); }
}; // class PTEFor64

constexpr int GetMaxPageTableLevelCount(AddrTransMode mode) {
    switch(mode) {
        case AddrTransMode::Sv32: return 2;
        case AddrTransMode::Sv39: return 3;
        case AddrTransMode::Sv48: return 4;
        case AddrTransMode::Sv57: return 5;
        default: diag::UnexpectedDefault("Invalid AddrTransMode\n");
    }
}

constexpr auto VPNPartSize = cfg::cpu::EnableIsaRV64I ? 9 : 10;

constexpr NativeWord GetPTOffset(Address vaddr, int level) {
    return (vaddr >> (VPNPartSize * level + 12)) & (cfg::cpu::EnableIsaRV64I ? 0x1F : 0x3F);
}

constexpr Result GetTranslationResult(Result res, Result accessFault, Result pageFault) {
    if(res.IsFailure()) {
        /* No PTE found & unprivilaged access are page faults. */
        if (ResultNoValidPteFound::Includes(res) ||
            ResultPageFault::Includes(res)) {
            return pageFault;
        }

        /* Anything else is a access fault. */
        return accessFault;
    }

    return ResultSuccess();
}

constexpr bool TranslationModeValid(AddrTransMode mode) {
    if constexpr(cfg::cpu::EnableIsaRV64I) {
        if (mode != AddrTransMode::Bare &&
            mode != AddrTransMode::Sv39 &&
            mode != AddrTransMode::Sv48 &&
            mode != AddrTransMode::Sv57) {
            return false;
        }
    }
    else {
        if (mode != AddrTransMode::Bare &&
            mode != AddrTransMode::Sv32) {
            return false;
        }
    }

    return true;
}

} // namespace

class MemoryManager::PTE : public std::conditional_t<cfg::cpu::EnableIsaRV64I, PTEFor64, PTEFor32> {};

Result MemoryManager::Initialize(mem::MemoryController* pMemCtlr) {
    diag::AssertNotNull(pMemCtlr);
    m_pMemCtlr = pMemCtlr;
    m_Mode = AddrTransMode::Bare;
    return ResultSuccess();
}

void MemoryManager::Finalize() {
    m_pMemCtlr = nullptr;
}

AddrTransMode MemoryManager::GetTransMode() const noexcept { return m_Mode; }

Result MemoryManager::SetTransMode(AddrTransMode mode) noexcept {
    /* Make sure mode is valid. */
    if(!TranslationModeValid(mode)) {
        return ResultInvalidTranslationMode();
    }

    m_Mode = mode;

    /* Update page table level count. */
    m_PTLevelCount = GetMaxPageTableLevelCount(mode);

    return true;
}

Address MemoryManager::GetPTAddr() const noexcept { return m_PTAddr; }
void MemoryManager::SetPTAddr(Address addr) noexcept { m_PTAddr = addr; }

Word MemoryManager::GetASID() const noexcept { return 0; }
void MemoryManager::SetASID(Word) noexcept { }

bool MemoryManager::GetEnabledSUM() const noexcept { return m_EnableSUM; }
void MemoryManager::SetEnabledSUM(bool val) noexcept { m_EnableSUM = val; }

bool MemoryManager::GetEnabledMXR() const noexcept { return m_EnableMXR; }
void MemoryManager::SetEnabledMXR(bool val) noexcept { m_EnableMXR = val; }

template<typename T>
Result MemoryManager::ReadImpl(auto readFunc, T* pOut, Address addr, PrivilageLevel level) {
    Result res;

    /* Assert that output and read func aren't null. */
    diag::AssertNotNull(pOut);
    diag::AssertNotNull(readFunc);

    /* Translate address if in non-machine mode and translation is enabled. */
    if(level != PrivilageLevel::Machine && m_Mode != AddrTransMode::Bare) {
        res = this->TranslateForRead(&addr, addr, level);
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

    /* Assert that write func isn't null. */
    diag::AssertNotNull(writeFunc);

    /* Translate address if in non-machine mode and translation is enabled. */
    if(level != PrivilageLevel::Machine && m_Mode != AddrTransMode::Bare) {
        res = this->TranslateForWrite(&addr, addr, level);
        if(res.IsFailure()) {
            return res;
        }
    }

    /* TODO: PMP: Perform PMP check. */

    /* Perform unmapped write. */
    return (*m_pMemCtlr.*writeFunc)(in, addr);
}

Result MemoryManager::GetPteImpl(PTE* pPte, Address* pPteAddr, int* pLevelFound, Address addr) {
    /* Assert that outputs aren't null. */
    diag::AssertNotNull(pPte);
    diag::AssertNotNull(pPteAddr);
    diag::AssertNotNull(pLevelFound);

    Result res;
    Address curPT = m_PTAddr;
    NativeWord pteVal = 0;
    int curLevel = m_PTLevelCount - 1;
    while(curLevel >= 0) {
        /* Calculate pte offset. */
        auto offset = GetPTOffset(addr, curLevel);
        auto pteAddr = curPT + offset * sizeof(NativeWord);

        /* Read PTE. */
        res = m_pMemCtlr->ReadNativeWord(&pteVal, pteAddr);
        if(res.IsFailure()) {
            return res;
        }

        /* Check valid bit. */
        PTE pte(pteVal);
        if(!pte.GetValid()) {
            return ResultNoValidPteFound();
        }

        /* Check if this is a leaf, return if it is. */
        if(pte.IsLeaf()) {
            *pLevelFound = curLevel;
            *pPte = pte;
            *pPteAddr = pteAddr;
            return ResultSuccess();
        }

        /* The next level of the page table is at pte.PPN. */
        curPT = pte.GetPPN();

        /* Decrement current level. */
        curLevel--;
    }

    return ResultNoValidPteFound();
}

Result MemoryManager::TranslateImpl(Address* pAddrOut, Address addr, PrivilageLevel level, TranslationReason reason) {
    Result res;

    /* Assert that output & check func aren't null. */
    diag::AssertNotNull(pAddrOut);

    /* Get PTE. */
    PTE pte;
    Address pteAddr = 0;
    int levelFound = 0;
    res = this->GetPteImpl(&pte, &pteAddr, &levelFound, addr);
    if(res.IsFailure()) {
        return res;
    }

    /* If we're running in userspace, make sure User bit is set. */
    if(level == PrivilageLevel::User) {
        if(!pte.GetForUser()) {
            return ResultPageFault();
        }
    }

    /* If we're running as supervisor, make sure User bit is unset or SUM is enabled. */
    else if (level == PrivilageLevel::Supervisor) {
        if(pte.GetForUser() && !m_EnableSUM) {
            return ResultPageFault();
        }
    }

    /* Perform check(s) based on reason. */
    switch(reason) {
    case TranslationReason::Load: {
        /* Make sure we can read from this page. */
        if(!pte.GetReadable() && !(this->GetEnabledMXR() && pte.GetExecutable())) {
            return ResultPageFault();
        }
    }
    case TranslationReason::Store: {
        /* Make sure we can write to this page. */
        if(!pte.GetWriteable()) {
            return ResultPageFault();
        }

        /* Set dirty bit. */
        pte.SetDirty(true);
    }
    case TranslationReason::Fetch: {
        /* Make sure we can fetch from this page. */
        if(!pte.GetExecutable()) {
            return ResultPageFault();
        }
    }
    case TranslationReason::Any: break;
    default: diag::UnexpectedDefault();
    }

    /* Set PTE accessed bit. */
    pte.SetAccessed(true);

    /* Write PTE back. */
    res = m_pMemCtlr->WriteNativeWord(pte.GetValue(), pteAddr);
    if(res.IsFailure()) {
        return res;
    }

    /* Calculate new address. */
    Address newAddr = pte.GetPPN() | (addr & 0xFFF); // start with PPN & 4K offset.

    /* If this PTE was at any level other than zero, perform handling for large pages. */
    if(levelFound) {
        auto mask = util::GenerateMaskRight<Address>(levelFound * VPNPartSize) << 12;
        newAddr |= addr & mask;
    }

    /* Return values. */
    *pAddrOut = newAddr;

    return ResultSuccess();
}

Result MemoryManager::TranslateForRead(Address* pOut, Address addr, PrivilageLevel level) {
    /* Assert that output isn't null. */
    diag::AssertNotNull(pOut);

    /* Translate address. */
    Result res = this->TranslateImpl(pOut, addr, level, TranslationReason::Load);

    return GetTranslationResult(res, ResultLoadAccessFault(), ResultLoadPageFault());
}

Result MemoryManager::TranslateForWrite(Address* pOut, Address addr, PrivilageLevel level) {
    /* Assert that output isn't null. */
    diag::AssertNotNull(pOut);

    /* Translate address. */
    Result res = this->TranslateImpl(pOut, addr, level, TranslationReason::Store);

    return GetTranslationResult(res, ResultStoreAccessFault(), ResultStorePageFault());
}

Result MemoryManager::TranslateForFetch(Address* pOut, Address addr, PrivilageLevel level) {
    /* Assert that output isn't null. */
    diag::AssertNotNull(pOut);

    /* Translate address. */
    Result res = this->TranslateImpl(pOut, addr, level, TranslationReason::Fetch);

    return GetTranslationResult(res, ResultFetchAccessFault(), ResultFetchPageFault());
}

Result MemoryManager::TranslateForAny(Address* pOut, Address addr, PrivilageLevel level) {
    return this->TranslateImpl(pOut, addr, level, TranslationReason::Any);
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

    diag::AssertNotNull(pOut);

    /* Translate address if in non-machine mode and translation is enabled. */
    if(level != PrivilageLevel::Machine && m_Mode != AddrTransMode::Bare) {
        res = this->TranslateForFetch(&addr, addr, level);
        if(res.IsFailure()) {
            return res;
        }
    }

    /* TODO: PMP: Perform PMP Check. */

    /* Perform unmapped fetch. */
    return m_pMemCtlr->ReadWord(pOut, addr);
}

template<typename T>
Result MemoryManager::MappedReadImpl(auto readFunc, T* pOut, Address addr) {
    diag::AssertNotNull(readFunc);
    diag::AssertNotNull(pOut);

    /* Translate address without permission checks. */
    Result res = this->TranslateForAny(&addr, addr, PrivilageLevel::Machine);
    if(res.IsFailure()) {
        return res;
    }

    /* Perform unmapped read. */
    return (*m_pMemCtlr.*readFunc)(pOut, addr);
}

template<typename T>
Result MemoryManager::MappedWriteImpl(auto writeFunc, T in, Address addr) {
    diag::AssertNotNull(writeFunc);

    /* Translate address without permission checks. */
    Result res = this->TranslateForAny(&addr, addr, PrivilageLevel::Machine);
    if(res.IsFailure()) {
        return res;
    }

    /* Perform unmapped write. */
    return (*m_pMemCtlr.*writeFunc)(in, addr);
}

Result MemoryManager::MappedReadByte(Byte* pOut, Address addr) {
    return this->MappedReadImpl(&MemCtlrT::ReadByte, pOut, addr);
}

Result MemoryManager::MappedReadHWord(HWord* pOut, Address addr) {
    return this->MappedReadImpl(&MemCtlrT::ReadHWord, pOut, addr);
}

Result MemoryManager::MappedReadWord(Word* pOut, Address addr) {
    return this->MappedReadImpl(&MemCtlrT::ReadWord, pOut, addr);
}

Result MemoryManager::MappedReadDWord(DWord* pOut, Address addr) {
    return this->MappedReadImpl(&MemCtlrT::ReadDWord, pOut, addr);
}

Result MemoryManager::MappedWriteByte(Byte in, Address addr) {
    return this->MappedWriteImpl(&MemCtlrT::WriteByte, in, addr);
}

Result MemoryManager::MappedWriteHWord(HWord in, Address addr) {
    return this->MappedWriteImpl(&MemCtlrT::WriteHWord, in, addr);
}

Result MemoryManager::MappedWriteWord(Word in, Address addr) {
    return this->MappedWriteImpl(&MemCtlrT::WriteWord, in, addr);
}

Result MemoryManager::MappedWriteDWord(DWord in, Address addr) {
    return this->MappedWriteImpl(&MemCtlrT::WriteDWord, in, addr);
}

} // namespace detail
} // namespace cpu
} // namespace riscv
