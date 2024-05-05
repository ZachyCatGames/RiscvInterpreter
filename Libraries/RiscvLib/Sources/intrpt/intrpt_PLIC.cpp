#include <RiscvEmu/intrpt/intrpt_PLIC.h>
#include <RiscvEmu/intrpt/intrpt_Result.h>
#include <RiscvEmu/diag.h>
#include <RiscvEmu/util/util_Bitfields.h>

namespace riscv {
namespace intrpt {

namespace {

[[maybe_unused]] constexpr auto AddrSpaceSize = 0x4000000;
[[maybe_unused]] constexpr auto MinSourceCount = 0;
[[maybe_unused]] constexpr auto MaxSourceCount = 1024;
[[maybe_unused]] constexpr auto MinTargetCount = 0;
[[maybe_unused]] constexpr auto MaxTargetCount = 15872;

[[maybe_unused]] constexpr Word PriorityRegStart = 0x0 / sizeof(Word);
[[maybe_unused]] constexpr Word PriorityRegEnd   = 0x1000 / sizeof(Word);

[[maybe_unused]] constexpr Word PendingRegStart = 0x1000 / sizeof(Word);
[[maybe_unused]] constexpr Word PendingRegEnd   = 0x2000 / sizeof(Word);

[[maybe_unused]] constexpr Word EnabledRegStart = 0x2000 / sizeof(Word);
[[maybe_unused]] constexpr Word EnabledRegEnd   = 0x20000 / sizeof(Word);

[[maybe_unused]] constexpr Word ContextRegStart = 0x200000 / sizeof(Word);
[[maybe_unused]] constexpr Word ContextRegEnd   = AddrSpaceSize / sizeof(Word);

[[maybe_unused]] constexpr auto EnabledWordsPerContext = 1024 / 32;

[[maybe_unused]] constexpr auto ContextSize = 0x1000;
[[maybe_unused]] constexpr auto ContextRegWordCount = 0x1000 / sizeof(Word);
[[maybe_unused]] constexpr auto ContextPriorityThresholdReg = 0;
[[maybe_unused]] constexpr auto ContextClaimReg = 1;

} // namespace

NativeWord PLIC::MmioInterface::GetMappedSize() { return AddrSpaceSize; }

/* TODO: What do we want to do here? */
Result PLIC::MmioInterface::LoadByte([[maybe_unused]] Byte*, [[maybe_unused]] Address) { return ResultSuccess(); }
Result PLIC::MmioInterface::LoadHWord([[maybe_unused]] HWord*, [[maybe_unused]] Address) { return ResultSuccess(); }
Result PLIC::MmioInterface::LoadDWord([[maybe_unused]] DWord*, [[maybe_unused]] Address) { return ResultSuccess(); }
Result PLIC::MmioInterface::StoreByte([[maybe_unused]] Byte, [[maybe_unused]] Address) { return ResultSuccess(); }
Result PLIC::MmioInterface::StoreHWord([[maybe_unused]] HWord, [[maybe_unused]] Address) { return ResultSuccess(); }
Result PLIC::MmioInterface::StoreDWord([[maybe_unused]] DWord, [[maybe_unused]] Address) { return ResultSuccess(); }

Result PLIC::MmioInterface::LoadWord(Word* pOut, Address addr) {
    return m_pParent->ReadRegister(pOut, static_cast<Word>(addr / 4));
}

Result PLIC::MmioInterface::StoreWord(Word in, Address addr) {
    return m_pParent->WriteRegister(static_cast<Word>(addr / 4), in);
}

PLIC::Target::Target() noexcept = default;
PLIC::Target::~Target() = default;

void PLIC::Target::Initialize(PLIC* pParent, TargetPtrT&& pTarget, Word id) {
    diag::AssertNotNull(pParent);
    diag::AssertNotNull(pTarget);
    m_pTarget = std::move(pTarget);
    m_pParent = pParent;
    m_PrioThreshold = 0;
    m_Id = id;
}

void PLIC::Target::Finalize() { m_pTarget.reset(); }

bool PLIC::Target::IsInitialized() const noexcept { return m_pTarget != nullptr; }

Word PLIC::Target::ReadEnableReg(Word index) const {
    diag::Assert(m_Id < m_EnableBits.size());
    return m_EnableBits[index];
}

void PLIC::Target::WriteEnableReg(Word index, Word val) {
    diag::Assert(m_Id < m_EnableBits.size());
    m_EnableBits[index] = val;
}

Word PLIC::Target::ReadPrioThreshold() const noexcept { return m_PrioThreshold; }
void PLIC::Target::WritePrioThreshold(Word val) noexcept { m_PrioThreshold = val; }

bool PLIC::Target::HasEnabledIRQ(Word src) {
    auto reg = this->ReadEnableReg(src / sizeof(m_EnableBits[0]) * 8);
    return util::ExtractBitfield(reg, src % 32, 1);
}

bool PLIC::Target::CanTakeTopIRQ() {
    auto& top = m_pParent->GetQueueTop();

    /* Return false if this target isn't initialized. */
    if(!this->IsInitialized()) {
        return false;
    }

    /* Return false if the source priority is too low for this target. */
    if(top.priority < m_PrioThreshold) {
        return false;
    }

    /* Return false if this target doesn't have this interrupt enabled. */
    if(!this->HasEnabledIRQ(top.pSrc->GetId())) {
        return false;
    }

    return true;
}

Result PLIC::Target::NotifyAvailableIRQImpl() {
    /* Check if we can take the top IRQ. */
    if(!this->CanTakeTopIRQ()) {
        return ResultSuccess();
    }

    /* Notify target. */
    return this->NotifyAvailableIRQ();
}

PLIC::Source::Source() noexcept :
    m_pParent(nullptr), m_State(InterruptState::Waiting), m_Priority(0), m_PendingCount(0) {}

void PLIC::Source::Initialize(PLIC* pParent) noexcept {
    diag::AssertNotNull(pParent);
    m_pParent = pParent;
    m_State = InterruptState::Waiting;
    m_Priority = 0;
    m_PendingCount = 0;
}

void PLIC::Source::Finalize() noexcept { m_pParent = nullptr; }

bool PLIC::Source::IsInitialized() const noexcept { return m_pParent != nullptr; }

Word PLIC::Source::ReadPriority() const noexcept { return m_Priority; }
void PLIC::Source::WritePriority(Word val) noexcept { m_Priority = val; }

PLIC::InterruptState PLIC::Source::GetState() const noexcept { return m_State; }

void PLIC::Source::NotifyPending() noexcept { m_State = PLIC::InterruptState::Pending; }
void PLIC::Source::NotifyWaiting() noexcept { m_State = PLIC::InterruptState::Waiting; }

Word PLIC::Source::NotifyClaimed() noexcept {
    /* Set our status to awaiting completion. */
    m_State = PLIC::InterruptState::AwaitCompletion;

    /* Decrement our pending count. */
    m_PendingCount--;

    /* Clear our pending bit if needed. */
    auto id = this->GetId();
    if(!m_PendingCount) {
        m_pParent->SetPending(id, false);
    }

    return id;
}

void PLIC::Source::NotifyComplete() noexcept {
    /* Do nothing if we aren't initialized. */
    if(this->IsInitialized()) {
        return;
    }

    /* Also do nothing if we haven't been claimed. */
    if(m_State != InterruptState::AwaitCompletion) {
        return;
    }

    /* If we don't have any more pending interrupts, set our state to waiting. */
    if (!m_PendingCount) {
        m_State = PLIC::InterruptState::Waiting;
        return;
    }

    /* If we do have more pending interrupts, set our state to pending and add ourself to the queue.. */
    m_State = PLIC::InterruptState::Pending;
    m_pParent->AddSourceToQueue(this);
}

Word PLIC::Source::GetId() const noexcept {
    return static_cast<Word>(this - m_pParent->m_Sources.data());
}

Result PLIC::Source::SignalInterrupt() {
    /* Set our pending bit if needed. */
    auto id = this->GetId();
    if(!m_PendingCount) {
        m_pParent->SetPending(id, 1);
    }

    /* Increment pending count. */
    m_PendingCount++;

    /* If our pending count is 1 and status is waiting, add ourself to our parent's queue. */
    if (m_PendingCount == 1 && m_State == InterruptState::Waiting) {
        m_pParent->AddSourceToQueue(this);
        m_State = PLIC::InterruptState::Pending;
    }

    return ResultSuccess();
}

PLIC::QueueData::QueueData(Source* p, Word prio) noexcept :
    pSrc(p), priority(prio) {}

auto PLIC::QueueData::operator<=>(const QueueData& rhs) const noexcept { return this->priority <=> rhs.priority; }

Result PLIC::Initialize() {
    /* Clear claimed flag. */
    m_Claimed = false;

    return ResultSuccess();
}

Result PLIC::RegisterTarget(TargetPtrT pTarget, Word id) {
    /* Assert that id is valid and we've been given a valid target. */
    this->AssertTargetIdValid(id);
    diag::AssertNotNull(pTarget);

    auto& dst = m_Targets[id];

    if(dst.IsInitialized()) {
        return ResultPLICTargetAlreadyTaken();
    }

    /* Initialize target context. */
    dst.Initialize(this, std::move(pTarget), id);

    return ResultSuccess();
}

Result PLIC::RegisterSource(ISource** ppSrc, Word id) {
    /* Assert that id is valid and we've been given a valid source ptr. */
    this->AssertSourceIdValid(id);
    diag::AssertNotNull(ppSrc);

    auto& src = m_Sources[id];

    if(src.IsInitialized()) {
        return ResultPLICSourceAlreadyTaken();
    }

    src.Initialize(this);
    *ppSrc = &src;
    return ResultSuccess();
}

Word PLIC::GetPriority(std::size_t source) const noexcept {
    this->AssertSourceIdValid(static_cast<Word>(source));
    return m_Sources[source].ReadPriority();
}

void PLIC::SetPriority(std::size_t source, Word val) noexcept {
    this->AssertSourceIdValid(static_cast<Word>(source));
    m_Sources[source].WritePriority(val);
}

bool PLIC::GetPending(std::size_t source) const noexcept {
    this->AssertSourceIdValid(static_cast<Word>(source));
    return util::ExtractBitArray(m_PendingBits.data(), static_cast<int>(source));
}

void PLIC::SetPending(std::size_t source, bool val) noexcept {
    this->AssertSourceIdValid(static_cast<Word>(source));
    util::AssignBitArray(m_PendingBits.data(), static_cast<int>(source), val);
}

bool PLIC::GetEnabled(std::size_t source, std::size_t target) const noexcept {
    /* Code using this should know better. */
    this->AssertTargetIdValid(static_cast<Word>(target));
    this->AssertSourceIdValid(static_cast<Word>(source));

    /* Read enabled register. */
    auto reg = m_Targets[target].ReadEnableReg(static_cast<Word>(source));

    /* Extract bit. */
    return util::ExtractBitfield(reg, static_cast<Word>(source) % m_PendingBitsPerWord, 1);
}

void PLIC::SetEnabled(std::size_t source, std::size_t target, bool enable) noexcept {
    /* Code using this should know better. */
    this->AssertTargetIdValid(static_cast<Word>(target));
    this->AssertSourceIdValid(static_cast<Word>(source));

    /* Read enabled register. */
    auto regId = static_cast<Word>(source) / m_PendingBitsPerWord;
    Word reg = m_Targets[target].ReadEnableReg(regId);

    /* Write bit. */
    reg = util::AssignBitfield(reg, static_cast<Word>(source) % m_PendingBitsPerWord, 1, static_cast<Word>(enable));

    /* Write register back. */
    m_Targets[target].WriteEnableReg(regId, reg);
}

Result PLIC::ReadRegister(Word* pOut, Word index) {
    if (index >= ContextRegStart) {
        return this->ReadContextReg(pOut, index - ContextRegStart);
    }
    else if (index >= EnabledRegStart) {
        return this->ReadEnabledReg(pOut, index - EnabledRegStart);
    }
    else if (index >= PendingRegStart) {
        return this->ReadPendingReg(pOut, index - PendingRegStart);
    }
    else /* if (index >= PriorityRegStart) */ {
        return this->ReadPriorityReg(pOut, index - PriorityRegStart);
    }
}

Result PLIC::WriteRegister(Word index, Word val) {
    if (index >= ContextRegStart) {
        return this->WriteContextReg(index - ContextRegStart, val);
    }
    else if (index >= EnabledRegStart) {
        return this->WriteEnabledReg(index - EnabledRegStart, val);
    }
    else if (index >= PendingRegStart) {
        /* Writing to pending does nothing. */
        return ResultSuccess();
    }
    else /* if (index >= PriorityRegStart) */ {
        return this->WriteRegister(index - PriorityRegStart, val);
    }
}

Result PLIC::ReadPriorityReg(Word* pOut, Word index) const noexcept {
    /* If index > SourceCount, trigger a access fault. */
    if(index >= m_SourceCount) {
        return mem::ResultLoadAccessFault();
    }

    /* Load priority value. */
    *pOut = m_Sources[index].ReadPriority();

    return ResultSuccess();
}

Result PLIC::WritePriorityReg(Word index, Word val) noexcept {
    /* If index > SourceCount, trigger a access fault. */
    if(index >= m_SourceCount) {
        return mem::ResultStoreAccessFault();
    }

    /* No-op if priority is the same. */
    if(m_Sources[index].ReadPriority() == val) {
        return ResultSuccess();
    }

    /* Write priority for the appropriate source. */
    m_Sources[index].WritePriority(val);

    /* Readd to queue. */
    this->AddSourceToQueue(m_Sources.data() + index);

    /* Attempt to interrupt all target. */
    this->NotifyAllTargetsIRQAvailable();

    return ResultSuccess();
}

Result PLIC::ReadPendingReg(Word* pOut, Word index) const noexcept {
    /* If index is greater than the number of pending works, trigger an access fault. */
    if(index >= m_PendingWordCount) {
        return mem::ResultLoadAccessFault();
    }

    /* Perform load. */
    *pOut = m_PendingBits[index];
    
    return ResultSuccess();
}

Result PLIC::ReadEnabledReg(Word* pOut, Word index) const noexcept {
    /* Compute target and source indecies. */
    auto target = index / m_MaxSourceCount;
    auto wordidx = index % m_MaxSourceCount;

    /* If either target is greater than the target count or wordidx is greater than pending word count, trigger an access fault. */
    if(target >= m_TargetCount || wordidx >= m_PendingWordCount) {
        return mem::ResultLoadAccessFault();
    }

    /* Load enabled register. */
    *pOut = m_Targets[target].ReadEnableReg(wordidx);

    return ResultSuccess();
}

Result PLIC::WriteEnabledReg(Word index, Word val) noexcept {
    /* Try to load current value. */
    Word cur = 0;
    Result res = this->ReadEnabledReg(&cur, index);
    if(res.IsFailure()) {
        return mem::ResultStoreAccessFault();
    }

    /* Store new value. */
    auto wordIdx = index % m_MaxSourceCount;
    m_Targets[index].WriteEnableReg(wordIdx, cur);

    /* Interrupt any targets that we just enabled. */
    Word changed = (val ^ cur) & val;
    if(changed) {
        m_Targets[wordIdx].NotifyAvailableIRQImpl();
    }

    return ResultSuccess();
}

Result PLIC::ReadContextReg(Word* pOut, Word index) {
    auto target = this->GetContextTarget(index);
    auto regId = this->GetContextRegId(index);

    /* Handle reading from priority threshold. */
    if (regId == ContextPriorityThresholdReg) {
        *pOut = m_Targets[target].ReadPrioThreshold();
    }

    /* Return if reserved word. */
    if (regId != ContextClaimReg) {
        *pOut = 0u;
        return ResultSuccess();
    }

    /* Claim top request. */
    *pOut = this->ClaimTopRequest();

    return ResultSuccess();
}

Result PLIC::WriteContextReg(Word index, Word val) {
    auto& target = m_Targets[this->GetContextTarget(index)];
    auto regId = this->GetContextRegId(index);

    /* Handle writing to priority threshold. */
    if (regId == ContextPriorityThresholdReg) {
        target.WritePrioThreshold(val);

        /* Attempt to interrupt this target, as it may now be interruptable. */
        target.NotifyAvailableIRQImpl();
    }

    /* Do nothing if this is a reserved word. */
    if (regId != ContextClaimReg) {
        return ResultSuccess();
    }

    /* We're writing the claim reg, signal that we've finished handling the interrupt. */
    m_Sources[val].NotifyComplete();

    return ResultSuccess();
}

void PLIC::CleanupQueue() {
    /* Remove any entries with incorrect priority or non-pending status. */
    const auto* pTop = &m_Queue.top();
    while ((pTop->priority != pTop->pSrc->ReadPriority() ||
        pTop->pSrc->GetState() != InterruptState::Pending) &&
        !m_Queue.empty()) {
        m_Queue.pop();
        pTop = &m_Queue.top();
    }
}

void PLIC::AddSourceToQueue(Source* pSrc) {
    /* Assert that source is not null. */
    diag::AssertNotNull(pSrc);

    /* Add source to queue. */
    m_Queue.emplace(pSrc, pSrc->ReadPriority());

    /* Notify targets if the queue was previously empty. */
    if(m_Queue.size() == 1) {
        this->NotifyAllTargetsIRQAvailable();
    }
}

Word PLIC::ClaimTopRequest() {
    /* Set claimed flag. */
    m_Claimed = true;

    /* Cleanup queue to be safe. */
    this->CleanupQueue();

    /* Return 0 if we have no requests. */
    if (m_Queue.empty()) {
        return 0;
    }

    /* Get top entry. */
    auto* pTop = m_Queue.top().pSrc;

    /* Remove from queue. */
    m_Queue.pop();

    /* Notify source it's been claimed. */
    auto id = pTop->NotifyClaimed();

    /* Notify targets if any other interrupts are available. */
    if(!m_Queue.empty()) {
        this->NotifyAllTargetsIRQAvailable();
    }

    return id;
}

void PLIC::NotifyAllTargetsIRQAvailable() {
    /* Cleanup queue. */
    this->CleanupQueue();

    /* Return if queue is empty. */
    if (m_Queue.empty()) {
        return;
    }

    /* Clear claimed flag. */
    m_Claimed = false;

    /* Interrupt all targets until one handles it. */
    for(auto& target : m_Targets) {
        /* Check if a target claimed the interrupt. */
        if(m_Claimed) {
            break;
        }

        /* Attempt to interrupt target. */
        target.NotifyAvailableIRQImpl();
    }

    /* Clear claimed flag. */
    m_Claimed = false;
}

bool PLIC::TargetIdValid(Word id) const noexcept { return id >= MinTargetCount && id < m_TargetCount; }
bool PLIC::SourceIdValid(Word id) const noexcept { return id >= MinSourceCount && id < m_SourceCount; }

void PLIC::AssertTargetIdValid(Word id, const std::source_location& location) const noexcept {
    diag::Assert(this->TargetIdValid(id), diag::FormatString("Invalid Target ID (max = %u; provided = %u)\n", location), m_TargetCount - 1, id);
}

void PLIC::AssertSourceIdValid(Word id, const std::source_location& location) const noexcept {
    diag::Assert(this->SourceIdValid(id), diag::FormatString("Invalid Source ID (max = %u; provided = %u)\n", location), m_SourceCount - 1, id);
}

const PLIC::QueueData& PLIC::GetQueueTop() const { return m_Queue.top(); }

} // namespace intrpt
} // namespace riscv
