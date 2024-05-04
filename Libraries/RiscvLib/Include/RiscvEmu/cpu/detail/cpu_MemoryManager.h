#pragma once
#include <RiscvEmu/cpu/cpu_Types.h>
#include <RiscvEmu/mem/mem_MCClient.h>

namespace riscv {
namespace cpu {

namespace detail {

class MemoryManager {
public:
    Result Initialize(mem::MCClient&& client);
    void Finalize();

    AddrTransMode GetTransMode() const noexcept;
    Result SetTransMode(AddrTransMode mode) noexcept;

    Address GetPTAddr() const noexcept;
    void SetPTAddr(Address addr) noexcept;

    Word GetASID() const noexcept;
    void SetASID(Word val) noexcept;

    bool GetEnabledSUM() const noexcept;
    void SetEnabledSUM(bool val) noexcept;

    bool GetEnabledMXR() const noexcept;
    void SetEnabledMXR(bool val) noexcept;

    template<typename WordType>
    Result Load(WordType* pOut, Address addr, PrivilageLevel level);

    template<typename WordType>
    Result Store(WordType in, Address addr, PrivilageLevel level);

    Result InstFetch(Word* pOut, Address addr, PrivilageLevel level);

    /* Bypass all permission checks and perform a forced mapped load. */
    template<typename WordType>
    Result MappedLoadForDebug(WordType* pOut, Address addr);

    /* Bypass all permission checks and perform a forced mapped store. */
    template<typename WordType>
    Result MappedStoreForDebug(WordType in, Address addr);
private:
    class PTE;

    enum class TranslationReason : int {
        Load = 0,
        Store,
        Fetch,
        Any
    }; // enum class TranslationReason

    Result GetPteImpl(PTE* pPte, Address* pPteAddr, int* pLevelFound, Address addr);

    Result TranslateImpl(Address* pAddrOut, Address addr, PrivilageLevel level, TranslationReason reason);

    Result GetTranslationResult(Result res, TranslationReason);

    Result Translate(Address* pAddrOut, Address addr, PrivilageLevel level, TranslationReason reason);
private:
    mem::MCClient m_McClient;
    mem::MemoryController* m_pMemCtlr;
    Address m_PTAddr;

    AddrTransMode m_Mode;
    int m_PTLevelCount;

    bool m_EnableSUM;
    bool m_EnableMXR;
}; // class MemoryManager

} // namespace detail
} // namespace cpu
} // namespace detail
