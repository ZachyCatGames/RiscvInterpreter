#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/intrpt/intrpt_ISource.h>
#include <RiscvEmu/intrpt/intrpt_ITarget.h>
#include <RiscvEmu/mem/mem_AlignedMmioDev.h>
#include <RiscvEmu/mem/mem_MemoryController.h>
#include <array>
#include <queue>
#include <source_location>
#include <vector>
#include <memory>

namespace riscv {
namespace intrpt {

class PLIC {
public:
    using TargetPtrT = std::shared_ptr<ITarget>;
public:
    Result Initialize();

    constexpr Word GetTargetCount() const noexcept { return m_TargetCount; }
    constexpr Word GetSourceCount() const noexcept { return m_SourceCount; }

    Result RegisterTarget(TargetPtrT pTarget, Word id);
    Result RegisterSource(ISource** ppSrc, Word id);

    Word GetPriority(int source) const noexcept;
    void SetPriority(int source, Word val) noexcept;

    bool GetPending(int source) const noexcept;
    void SetPending(int source, bool val) noexcept;

    bool GetEnabled(int source, int target) const noexcept;
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
    class Source;
    struct QueueData;

    void CleanupQueue();

    void AddSourceToQueue(Source* pSrc);
    Word ClaimTopRequest();

    void NotifyAllTargetsIRQAvailable();

    const QueueData& GetQueueTop() const;

    bool TargetIdValid(Word id) const noexcept;
    bool SourceIdValid(Word id) const noexcept;

    void AssertTargetIdValid(Word id, const std::source_location& location = std::source_location::current()) const noexcept;
    void AssertSourceIdValid(Word id, const std::source_location& location = std::source_location::current()) const noexcept;
private:
    /* TODO: Config. */
    static constexpr auto m_SourceCount = 128;
    static constexpr auto m_TargetCount = 128;

    static constexpr auto m_PendingWordCount = (m_SourceCount + 31u) / 32u;
private:
    class MmioInterface : public mem::IMmioDev {
    public:
        virtual NativeWord GetMappedSize() override;

        virtual Result LoadByte(Byte* pOut, Address addr) override;
        virtual Result LoadHWord(HWord* pOut, Address addr) override;
        virtual Result LoadWord(Word* pOut, Address addr) override;
        virtual Result LoadDWord(DWord* pOut, Address addr) override;
        virtual Result StoreByte(Byte in, Address addr) override;
        virtual Result StoreHWord(HWord in, Address addr) override;
        virtual Result StoreWord(Word in, Address addr) override;
        virtual Result StoreDWord(DWord in, Address addr) override;
    private:
        PLIC* m_pParent;
    }; // class MmioInterface

    enum class InterruptState {
        Waiting,
        Pending,
        AwaitCompletion
    }; // enum class InterruptState

    class Source : public ISource {
    public:
        Source() noexcept;

        void Initialize(PLIC* pParent) noexcept;

        void Finalize() noexcept;

        bool IsInitialized() const noexcept;

        Word GetId() const noexcept;

        Word ReadPriority() const noexcept;
        void WritePriority(Word val) noexcept;

        Word GetPendingCount() const noexcept;

        InterruptState GetState() const noexcept;

        void NotifyPending() noexcept;
        void NotifyWaiting() noexcept;
        Word NotifyClaimed() noexcept;
        void NotifyComplete() noexcept;

        virtual Result SignalInterrupt() override;
    private:
        PLIC* m_pParent;
        InterruptState m_State;
        Word m_Priority;
        Word m_PendingCount;
    }; // class Source

    class Target : public detail::ITargetForCtrl {
    public:
        virtual bool HasPendingIRQ() override;

        virtual Result EnableInterrupts() override;

        virtual Result DisableInterrupts() override;
    public:
        Target() noexcept;
        Target(Target&&) = default;
        Target(const Target&) = default;
        virtual ~Target();

        void Initialize(PLIC* pParent, TargetPtrT&& pTarget, Word id);

        void Finalize();

        bool IsInitialized() const noexcept;

        Word ReadEnableReg(Word index) const;
        void WriteEnableReg(Word index, Word val);

        Word ReadPrioThreshold() const noexcept;
        void WritePrioThreshold(Word val) noexcept;

        bool HasEnabledIRQ(Word src);

        bool CanTakeTopIRQ();

        Result NotifyAvailableIRQImpl();
    private:
        TargetPtrT m_pTarget;
        std::array<Word, PLIC::m_PendingWordCount> m_EnableBits;
        PLIC* m_pParent;
        Word m_PrioThreshold;
        Word m_Id;
    }; // class Target

    struct QueueData {
        QueueData(Source* p, Word prio) noexcept;

        auto operator<=>(const QueueData& rhs) const noexcept;
        
        Source* pSrc;
        Word priority;
    }; // struct QueueData
private:
    bool m_Claimed;

    std::array<Source, m_SourceCount> m_Sources;
    std::array<Target, m_TargetCount> m_Targets;
    std::array<Word, m_PendingWordCount> m_PendingBits;

    std::priority_queue<QueueData> m_Queue;

    mem::AlignedMmioDev<MmioInterface> m_MmioIterface;
}; // class PLIC

} // namespace intrpt
} // namespace riscv