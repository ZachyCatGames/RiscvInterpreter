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

constexpr Word GetEnabledTarget(Word val) noexcept { return val / EnabledWordsPerContext; }
constexpr Word GetEnabledSource(Word val) noexcept { return val % EnabledWordsPerContext; }

constexpr Word GetContextTarget(Word val) noexcept { return val / ContextRegWordCount; }
constexpr Word GetContextRegId (Word val) noexcept { return val % ContextSize; }

} // namespace

NativeWord PLIC::MmioInterface::GetMappedSize() { return AddrSpaceSize; }

/* TODO: What do we want to do here? */
Result PLIC::MmioInterface::ReadByte([[maybe_unused]] Byte*, [[maybe_unused]] Address) { return ResultSuccess(); }
Result PLIC::MmioInterface::ReadHWord([[maybe_unused]] HWord*, [[maybe_unused]] Address) { return ResultSuccess(); }
Result PLIC::MmioInterface::ReadDWord([[maybe_unused]] DWord*, [[maybe_unused]] Address) { return ResultSuccess(); }
Result PLIC::MmioInterface::WriteByte([[maybe_unused]] Byte, [[maybe_unused]] Address) { return ResultSuccess(); }
Result PLIC::MmioInterface::WriteHWord([[maybe_unused]] HWord, [[maybe_unused]] Address) { return ResultSuccess(); }
Result PLIC::MmioInterface::WriteDWord([[maybe_unused]] DWord, [[maybe_unused]] Address) { return ResultSuccess(); }

Result PLIC::MmioInterface::ReadWord(Word* pOut, Address addr) {
    *pOut = m_pParent->ReadRegister(static_cast<Word>(addr));
    return ResultSuccess();
}

Result PLIC::MmioInterface::WriteWord(Word in, Address addr) {
    m_pParent->WriteRegister(static_cast<Word>(addr), in);
    return ResultSuccess();
}

PLIC::TargetBridgeImpl::TargetBridgeImpl() noexcept :
    m_pParent(nullptr), m_Id(0) {}

void PLIC::TargetBridgeImpl::Initialize(PLIC* pParent, Word id) noexcept {
    diag::AssertNotNull(pParent);
    m_pParent = pParent;
    m_Id = id;
}

Result PLIC::TargetBridgeImpl::EnableInterrupts() { return ResultSuccess(); }
Result PLIC::TargetBridgeImpl::DisableInterrupts() { return ResultSuccess(); }

bool PLIC::TargetBridgeImpl::IsPendingInterruptAvailable() { return m_pParent->CanTargetTakeTopIRQ(m_Id); }

PLIC::SourceImpl::SourceImpl() noexcept :
    pParent(nullptr), state(InterruptState::Waiting), priority(0), pendingCount(0) {}

void PLIC::SourceImpl::Initialize(PLIC* pParent) noexcept {
    diag::AssertNotNull(pParent);
    this->pParent = pParent;
    this->state = InterruptState::Waiting;
    this->priority = 0;
    this->pendingCount = 0;
}

void PLIC::SourceImpl::Finalize() noexcept { this->pParent = nullptr; }

bool PLIC::SourceImpl::IsInitialized() const noexcept { return this->pParent != nullptr; }

Word PLIC::SourceImpl::GetId() const noexcept {
    return static_cast<Word>(this - this->pParent->m_Sources.data());
}

Result PLIC::SourceImpl::SignalInterrupt() {
    /* Increment pending count. */
    this->pendingCount++;

    /* If our pending count is 1 and status is waiting, add ourself to our parent's queue. */
    if (this->pendingCount == 1 && this->state == InterruptState::Waiting) {
        this->pParent->AddSourceToQueue(this);
    }

    return ResultSuccess();
}

PLIC::QueueData::QueueData(SourceImpl* p, Word prio) noexcept :
    pSrc(p), priority(prio) {}

auto PLIC::QueueData::operator<=>(const QueueData& rhs) const noexcept { return this->priority <=> rhs.priority; }

PLIC::TargetContext::TargetContext() noexcept :
    pTarget(nullptr), priorityThreshold(0), bridge(), enableBits() {}

Result PLIC::Initialize(int sourceCount, int targetCount) {
    /* Make sure source count is valid. */
    if (sourceCount >= MaxSourceCount || sourceCount < MinSourceCount) {
        return ResultPLICInvalidSourceCount();
    }

    /* Make sure target count is valid. */
    if (targetCount >= MaxTargetCount || targetCount < MinTargetCount) {
        return ResultPLICInvalidTargetCount();
    }

    /* Clear claimed flag. */
    m_Claimed = false;

    /* Copy source/target values. */
    m_SourceCount = static_cast<Word>(sourceCount);
    m_TargetCount = static_cast<Word>(targetCount);

    /* Calculate how many words we'll need for pending bits. */
    m_PendingCount = (m_SourceCount + 31u) / 32u;

    /* Init source and target vectors. */
    m_Sources.resize(m_SourceCount);
    m_Targets.resize(m_TargetCount);

    m_PendingBits.resize(m_PendingCount);

    return ResultSuccess();
}

Word PLIC::GetTargetCount() const noexcept { return m_TargetCount; }
Word PLIC::GetSourceCount() const noexcept { return m_SourceCount; }

Result PLIC::RegisterTarget(TargetPtrT pTarget, Word id) {
    /* Assert that id is valid and we've been given a valid target. */
    this->AssertTargetIdValid(id);
    diag::AssertNotNull(pTarget);

    auto pDest = &m_Targets[id];

    if(pDest->pTarget) {
        return ResultPLICTargetAlreadyTaken();
    }

    /* Initialize target context. */
    pDest->pTarget = std::move(pTarget);
    pDest->priorityThreshold = 0;
    pDest->enableBits.resize(m_PendingCount);
    pDest->bridge.Initialize(this, id);

    /* Initialize target. */
    pTarget->InitializeForController(&pDest->bridge);

    return ResultSuccess();
}

Result PLIC::RegisterSource(ISource** ppSrc, Word id) {
    /* Assert that id is valid and we've been given a valid source ptr. */
    this->AssertSourceIdValid(id);
    diag::AssertNotNull(ppSrc);

    auto pSrc = &m_Sources[id];

    if(pSrc->IsInitialized()) {
        return ResultPLICSourceAlreadyTaken();
    }

    pSrc->Initialize(this);
    *ppSrc = pSrc;
    return ResultSuccess();
}

Word PLIC::GetPriority(int source) const noexcept {
    Word val = 0;
    this->AssertSourceIdValid(static_cast<Word>(source));
    this->ReadPriorityRegImpl(&val, static_cast<Word>(source));
    return val;
}

void PLIC::SetPriority(int source, Word val) noexcept {
    this->AssertSourceIdValid(static_cast<Word>(source));
    this->WritePriorityRegImpl(static_cast<Word>(source), val);
}

bool PLIC::GetPending(int source) const noexcept {
    this->AssertSourceIdValid(static_cast<Word>(source));
    return util::ExtractBitArray(m_PendingBits.data(), source);
}

void PLIC::SetPending(int source, bool val) noexcept {
    this->AssertSourceIdValid(static_cast<Word>(source));
    util::AssignBitArray(m_PendingBits.data(), source, val);
}

bool PLIC::GetEnabled(int source, int target) const noexcept {
    /* Code using this should know better. */
    this->AssertTargetIdValid(static_cast<Word>(target));
    this->AssertSourceIdValid(static_cast<Word>(source));

    /* Read enabled register. */
    Word reg = 0;
    this->ReadEnabledRegImpl(&reg, static_cast<Word>(target * EnabledWordsPerContext + source));

    /* Extract bit. */
    return util::ExtractBitfield(reg, static_cast<Word>(source) % WordBitLen, 1);
}

void PLIC::SetEnabled(int source, int target, bool enable) noexcept {
    /* Code using this should know better. */
    this->AssertTargetIdValid(static_cast<Word>(target));
    this->AssertSourceIdValid(static_cast<Word>(source));

    /* Read enabled register. */
    auto regId = static_cast<Word>(target * EnabledWordsPerContext + source);
    Word reg = 0;
    if (!this->ReadEnabledRegImpl(&reg, regId)) {
        return;
    }

    /* Write bit. */
    reg = util::AssignBitfield(reg, static_cast<Word>(source) % WordBitLen, 1, static_cast<Word>(enable));

    /* Write register back. */
    this->WriteEnabledReg(regId, reg);
}

Word PLIC::ReadRegister(Word index) {
    if (index >= ContextRegStart) {
        return this->ReadContextReg(index - ContextRegStart);
    }
    else if (index >= EnabledRegStart) {
        return this->ReadEnabledReg(index - EnabledRegStart);
    }
    else if (index >= PendingRegStart) {
        return this->ReadPendingReg(index - PendingRegStart);
    }
    else /* if (index >= PriorityRegStart) */ {
        return this->ReadPriorityReg(index - PriorityRegStart);
    }
}

void PLIC::WriteRegister(Word index, Word val) {
    if (index >= ContextRegStart) {
        this->WriteContextReg(index - ContextRegStart, val);
    }
    else if (index >= EnabledRegStart) {
        this->WriteEnabledReg(index - EnabledRegStart, val);
    }
    else if (index >= PendingRegStart) {
        /* Writing to pending does nothing. */
        return;
    }
    else /* if (index >= PriorityRegStart) */ {
        this->WriteRegister(index - PriorityRegStart, val);
    }
}

Word PLIC::ReadPriorityReg(Word index) const noexcept {
    Word val = 0;
    this->ReadPriorityRegImpl(&val, index);
    return val;
}

void PLIC::WritePriorityReg(Word index, Word val) noexcept {
    /* noop if priority is the same or index is bad. */
    Word reg = 0;
    if (!this->ReadPriorityRegImpl(&reg, index) || val == reg) {
        return;
    }

    /* Write priority for the appropriate source. */
    this->WritePriorityRegImpl(index, val);

    /* Readd to queue. */
    this->AddSourceToQueue(m_Sources.data() + index);

    /* Attempt to interrupt all target. */
    this->NotifyAllTargetsIRQAvailable();
}

Word PLIC::ReadPendingReg(Word index) const noexcept {
    Word val = 0;
    this->ReadPendingRegImpl(&val, index);
    return val;
}

Word PLIC::ReadEnabledReg(Word index) const noexcept {
    Word val = 0;
    this->ReadEnabledRegImpl(&val, index);
    return val;
}

void PLIC::WriteEnabledReg(Word index, Word val) noexcept {
    /* Read current value. */
    Word cur = 0;
    this->ReadEnabledRegImpl(&cur, index);

    /* Write new value. */
    this->WriteEnabledRegImpl(index, val);

    /* Interrupt any targets that we just enabled. */
    Word changed = (val ^ cur) & val;
    if(changed) {
        auto target = GetEnabledTarget(index);
        this->NotifyTargetIRQAvailable(target);
    }
}

Word PLIC::ReadContextReg(Word index) {
    auto target = GetContextTarget(index);
    auto regId = GetContextRegId(index);

    /* Handle reading from priority threshold. */
    if (regId == ContextPriorityThresholdReg) {
        return m_Targets[target].priorityThreshold;
    }

    /* Return if reserved word. */
    if (regId != ContextClaimReg) {
        return 0;
    }

    /* Claim top request. */
    return this->ClaimTopRequest();
}

void PLIC::WriteContextReg(Word index, Word val) {
    auto target = GetContextTarget(index);
    auto regId = GetContextRegId(index);

    /* Handle writing to priority threshold. */
    if (regId == ContextPriorityThresholdReg) {
        m_Targets[target].priorityThreshold = val;

        /* Attempt to interrupt this target, as it may now be interruptable. */
        this->NotifyTargetIRQAvailable(target);
    }

    /* Do nothing if this is a reserved word. */
    if (regId != ContextClaimReg) {
        return;
    }

    /* Signal that we've finished handling the interrupt. */
    this->NotifyCompletion(val);
}

bool PLIC::ReadPriorityRegImpl(Word* pOut, Word index) const noexcept {
    /* Make sure index is valid. */
    if (index >= m_Sources.size()) {
        return false;
    }

    *pOut = m_Sources[index].priority;
    return true;
}

bool PLIC::WritePriorityRegImpl(Word index, Word val) noexcept {
    if (index >= m_Sources.size()) {
        return false;
    }
    
    m_Sources[index].priority = val;
    return true;
}

bool PLIC::ReadPendingRegImpl(Word* pOut, Word index) const noexcept {
    if (index >= m_PendingBits.size()) {
        return false;
    }

    *pOut = m_PendingBits[index];
    return true;
}
bool PLIC::WritePendingRegImpl(Word index, Word val) noexcept {
    if (index >= m_PendingBits.size()) {
        return false;
    }

    m_PendingBits[index] = val;
    return true;
}

bool PLIC::ReadEnabledRegImpl(Word* pOut, Word index) const noexcept {
    auto target = GetEnabledTarget(index);
    auto source = GetEnabledSource(index);

    if (target >= m_TargetCount || source >= m_SourceCount) {
        return false;
    }

    *pOut = m_Targets[target].enableBits[source];
    return true;
}

bool PLIC::WriteEnabledRegImpl(Word index, Word val) noexcept {
    auto target = GetEnabledTarget(index);
    auto source = GetEnabledSource(index);

    if (target >= m_TargetCount || source >= m_SourceCount) {
        return false;
    }

    m_Targets[target].enableBits[source] = val;
    return true;
}

void PLIC::CleanupQueue() {
    /* Remove any entries with incorrect priority or non-pending status. */
    const auto* pTop = &m_Queue.top();
    while ((pTop->priority != pTop->pSrc->priority ||
        pTop->pSrc->state != InterruptState::Pending) &&
        !m_Queue.empty()) {
        m_Queue.pop();
        pTop = &m_Queue.top();
    }
}

void PLIC::AddSourceToQueue(SourceImpl* pSrc) {
    /* Assert that source is not null. */
    diag::AssertNotNull(pSrc);

    /* Set status to pending. */
    pSrc->state = InterruptState::Pending;

    /* Add source to queue. */
    m_Queue.emplace(pSrc, pSrc->priority);

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

    /* Decrement pending count. */
    pTop->pendingCount--;

    /* Indicate that this source is waiting to be handled. */
    pTop->state = InterruptState::AwaitCompletion;

    /* Calculate source id. */
    auto id = pTop->GetId();

    /* Clear pending bit. */
    this->SetPending(static_cast<int>(id), false);

    /* Notify targets if any other interrupts are available. */
    if(!m_Queue.empty()) {
        this->NotifyAllTargetsIRQAvailable();
    }

    return id;
}

void PLIC::NotifyCompletion(Word sourceId) {
    /* Get the source we're marking as completed. */
    auto* pSrc = &m_Sources[sourceId];

    /* Do nothing if this isn't an initialized source. */
    if(pSrc->IsInitialized()) {
        return;
    }

    /* Also do nothing if this isn't a source awaiting completion. */
    if(pSrc->state != InterruptState::AwaitCompletion) {
        return;
    }

    /* If this source doesn't have any additional pending interrupts, set status to waiting. */
    if (!pSrc->pendingCount) {
        pSrc->state = InterruptState::Waiting;
        return;
    }

    /* If this source does have pending interrupts, add it back to the queue. */
    this->AddSourceToQueue(pSrc);
}

bool PLIC::CanTargetTakeTopIRQ(Word targetId) const noexcept {
    auto& top = m_Queue.top();
    auto* pTargetCtx = &m_Targets[targetId];

    /* Return false if this target isn't initialized. */
    if(!pTargetCtx->pTarget) {
        return false;
    }

    /* Return false if the source priority is too low for this target. */
    if(top.priority < pTargetCtx->priorityThreshold) {
        return false;
    }

    /* Return false if this target doesn't have this interrupt enabled. */
    if(!this->GetEnabled(static_cast<int>(top.pSrc->GetId()), static_cast<int>(targetId))) {
        return false;
    }

    return true;
}

void PLIC::NotifyTargetIRQAvailable(Word targetId) {
    if(!this->CanTargetTakeTopIRQ(targetId)) {
        return;
    }

    /* Interrupt target. */
    m_Targets[targetId].pTarget->NotifyIRQImpl();
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
    for(Word i = 0; i < m_Targets.size(); i++) {
        /* If a context claimed the interrupt, no need to interrupt more targets. */
        if(m_Claimed) {
            break;
        }

        /* Attempt to interrupt the current target. */
        this->NotifyTargetIRQAvailable(i);
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

} // namespace intrpt
} // namespace riscv
