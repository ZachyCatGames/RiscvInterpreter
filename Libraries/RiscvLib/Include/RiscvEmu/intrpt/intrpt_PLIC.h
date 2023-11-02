#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/intrpt/intrpt_ISource.h>
#include <RiscvEmu/intrpt/intrpt_ITarget.h>
#include <RiscvEmu/mem/mem_AlignedMmioDev.h>
#include <RiscvEmu/mem/mem_MemoryController.h>
#include <queue>
#include <vector>
#include <memory>

namespace riscv {
namespace intrpt {

class PLIC {
public:
    using TargetPtrT = std::shared_ptr<ITarget>;
public:
    Result Initialize(int sourceCount, int targetCount);

    Result RegisterTarget(TargetPtrT pTarget, Word id);

    Result RegisterSource(ISource** ppSrc, Word id);

    Word GetPriority(int source) const noexcept;
    void SetPriority(int source, Word val) noexcept;

    bool GetPending(int source) const noexcept;
    void SetPending(int source, bool val) noexcept;

    bool GetEnabled(int source, int target) noexcept;
    void SetEnabled(int source, int target, bool val) noexcept;
private:
    Word ReadRegister(Word index);
    void WriteRegister(Word index, Word val);

    Word ReadPriorityReg(Word index) const noexcept;
    void WritePriorityReg(Word index, Word val) noexcept;

    Word ReadPendingReg(Word index) const noexcept;

    Word ReadContextReg(Word index);
    void WriteContextReg(Word index, Word val);

    Word ReadEnabledReg(Word index) const noexcept;
    void WriteEnabledReg(Word index, Word val) noexcept;

    bool ReadPriorityRegImpl(Word* pOut, Word index) const noexcept;
    bool WritePriorityRegImpl(Word index, Word val) noexcept;

    bool ReadPendingRegImpl(Word* pOut, Word index) const noexcept;
    bool WritePendingRegImpl(Word index, Word val) noexcept;

    bool ReadEnabledRegImpl(Word* pOut, Word index) const noexcept;
    bool WriteEnabledRegImpl(Word index, Word val) noexcept;

    bool ReadContextRegImpl(Word* pOut, Word index) noexcept;
    bool WriteContextRegImpl(Word index, Word val) noexcept;
private:
    struct SourceImpl;

    void AddSourceToQueue(SourceImpl* pSrc);
    void CleanupQueue();
    Word ClaimTopRequest();
    void MarkCompletion(Word sourceId);
    void InterruptTarget(Word targetId);
    void InterruptAllTargets();

    Word GetSourceIndex(const SourceImpl* pSrc) const noexcept;
private:
    class MmioInterface : public mem::IMmioDev {
    public:
        virtual NativeWord GetMappedSize() override;

        virtual Result ReadByte(Byte* pOut, Address addr) override;
        virtual Result ReadHWord(HWord* pOut, Address addr) override;
        virtual Result ReadWord(Word* pOut, Address addr) override;
        virtual Result ReadDWord(DWord* pOut, Address addr) override;
        virtual Result WriteByte(Byte in, Address addr) override;
        virtual Result WriteHWord(HWord in, Address addr) override;
        virtual Result WriteWord(Word in, Address addr) override;
        virtual Result WriteDWord(DWord in, Address addr) override;
    private:
        PLIC* m_pParent;
    }; // class MmioInterface

    class TargetBridgeImpl : public ITargetCtlrBridge {
    public:
        virtual Result EnableInterrupts() override;

        virtual Result DisableInterrupts() override;
    private:
        friend class PLIC;
        void Initialize(PLIC* pParent, Word id) noexcept;
    private:
        PLIC* m_pParent;
        Word m_Id;
    }; // class TargetBridgeImpl

    enum class InterruptState {
        Waiting,
        Pending,
        AwaitCompletion
    }; // enum class InterruptState

    struct SourceImpl : public ISource {
    public:
        void Initialize(PLIC* pParent) noexcept;
        void Finalize() noexcept;

        bool IsInitialized() const noexcept;

        virtual Result SignalInterrupt() override;

        PLIC* pParent;
        InterruptState state;
        Word priority;
        Word pendingCount;
    }; // class SourceImpl

    struct QueueData {
        constexpr QueueData(SourceImpl* p, Word prio) noexcept :
            pImpl(p), priority(prio) {}

        constexpr auto operator<=>(const QueueData& rhs) const noexcept { return this->priority <=> rhs.priority; }

        SourceImpl* pImpl;
        Word priority;
    }; // struct QueueData

    struct TargetContext {
        TargetPtrT pTarget;
        Word priorityThreshold;
        TargetBridgeImpl bridge;
        std::vector<Word> enableBits;
    }; // struct TargetContextRegisters
private:
    bool m_Claimed;

    Word m_SourceCount;
    Word m_TargetCount;

    Word m_PendingCount;

    std::vector<SourceImpl> m_Sources;
    std::vector<TargetContext> m_Targets;

    std::vector<Word> m_PendingBits;
    //std::vector<Word> m_EnabledBits;

    std::priority_queue<QueueData> m_Queue;

    mem::AlignedMmioDev<MmioInterface> m_MmioIterface;
}; // class PLIC

} // namespace intrpt
} // namespace riscv