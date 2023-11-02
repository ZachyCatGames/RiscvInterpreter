#include <RiscvEmu/intrpt/intrpt_PLIC.h>
#include <RiscvEmu/intrpt/intrpt_Result.h>
#include <RiscvEmu/util/util_Bitfields.h>

namespace riscv {
namespace intrpt {

namespace {

[[maybe_unused]] constexpr auto AddrSpaceSize = 0x4000000;
[[maybe_unused]] constexpr auto MinSourceCount = 0;
[[maybe_unused]] constexpr auto MaxSourceCount = 1024;
[[maybe_unused]] constexpr auto MinTargetCount = 0;
[[maybe_unused]] constexpr auto MaxTargetCount = 15872;

[[maybe_unused]] constexpr auto PriorityRegStart = 0x0 / sizeof(Word);
[[maybe_unused]] constexpr auto PriorityRegEnd   = 0x1000 / sizeof(Word);

[[maybe_unused]] constexpr auto PendingRegStart = 0x1000 / sizeof(Word);
[[maybe_unused]] constexpr auto PendingRegEnd   = 0x2000 / sizeof(Word);

[[maybe_unused]] constexpr auto EnabledRegStart = 0x2000 / sizeof(Word);
[[maybe_unused]] constexpr auto EnabledRegEnd   = 0x20000 / sizeof(Word);

[[maybe_unused]] constexpr auto ContextRegStart = 0x200000 / sizeof(Word);
[[maybe_unused]] constexpr auto ContextRegEnd   = AddrSpaceSize / sizeof(Word);

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

void PLIC::TargetBridgeImpl::Initialize(PLIC* pParent, Word id) noexcept {
    m_pParent = pParent;
    m_Id = id;
}

Result PLIC::TargetBridgeImpl::EnableInterrupts() {
    /* The target has reenabled interrupts, attempt to interrupt it. */
    m_pParent->InterruptTarget(m_Id);
    return ResultSuccess();
}

Result PLIC::TargetBridgeImpl::DisableInterrupts() { return ResultSuccess(); }

void PLIC::SourceImpl::Initialize(PLIC* pParent) noexcept {
    this->pParent = pParent;
    this->state = InterruptState::Waiting;
    this->priority = 0;
    this->pendingCount = 0;
}

void PLIC::SourceImpl::Finalize() noexcept { this->pParent = nullptr; }

bool PLIC::SourceImpl::IsInitialized() const noexcept { return this->pParent != nullptr; }

Result PLIC::SourceImpl::SignalInterrupt() {
    /* Increment pending count. */
    this->pendingCount++;

    /* If our pending count is 1, add ourself to our parent's queue. */
    if (this->pendingCount == 1) {
        this->pParent->AddSourceToQueue(this);
    }

    return ResultSuccess();
}

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

Result PLIC::RegisterTarget(TargetPtrT pTarget, Word id) {
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
    this->ReadPriorityRegImpl(&val, static_cast<Word>(source));
    return val;
}

void PLIC::SetPriority(int source, Word val) noexcept {
    this->WritePriorityRegImpl(static_cast<Word>(source), val);
}

bool PLIC::GetPending(int source) const noexcept {
    return util::ExtractBitArray(m_PendingBits.data(), source);
}

void PLIC::SetPending(int source, bool val) noexcept {
    util::AssignBitArray(m_PendingBits.data(), source, val);
}

bool PLIC::GetEnabled(int source, int target) noexcept {
    /* Read enabled register. */
    Word reg = 0;
    this->ReadEnabledRegImpl(&reg, static_cast<Word>(target * EnabledWordsPerContext + source));

    /* Extract bit. */
    return util::ExtractBitfield(reg, static_cast<Word>(source) % WordBitLen, 1);
}

void PLIC::SetEnabled(int source, int target, bool enable) noexcept {
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

    /* Any type of update may cause a change in state, so perform an update. */
    this->PerformUpdate();
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
    this->InterruptAllTargets();
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
        this->InterruptTarget(target);
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
        this->InterruptTarget(target);
    }

    /* Return if reserved word. */
    if (regId != ContextClaimReg) {
        return;
    }

    /* Signal that we've finished handling the interrupt. */
    this->MarkCompletion(val);
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

void PLIC::AddSourceToQueue(SourceImpl* pSrc) {
    /* Set status to pending. */
    pSrc->state = InterruptState::Pending;

    /* Add source to queue. */
    m_Queue.emplace(pSrc, pSrc->priority);
}

void PLIC::CleanupQueue() {
    /* Remove any entries with incorrect priority or non-pending status. */
    const auto* pTop = &m_Queue.top();
    while ((pTop->priority != pTop->pImpl->priority ||
        pTop->pImpl->state != InterruptState::Pending) &&
        !m_Queue.empty()) {
        m_Queue.pop();
        pTop = &m_Queue.top();
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
    auto* pTop = m_Queue.top().pImpl;

    /* Remove from queue. */
    m_Queue.pop();

    /* Decrement pending count. */
    pTop->pendingCount--;

    /* Indicate that this source is waiting to be handled. */
    pTop->state = InterruptState::AwaitCompletion;

    /* Calculate source id. */
    auto id = this->GetSourceIndex(pTop);

    /* Clear pending bit. */
    this->SetPending(static_cast<int>(id), false);

    return id;
}

void PLIC::MarkCompletion(Word sourceId) {
    /* Get the source we're marking as completed. */
    auto* pSrc = &m_Sources[sourceId];

    /* If this source doesn't have any additional pending interrupts, set status to waiting. */
    if (!pSrc->pendingCount) {
        pSrc->state = InterruptState::Waiting;
        return;
    }

    /* If this source does have pending interrupts, add it back to the queue. */
    this->AddSourceToQueue(pSrc);
}

void PLIC::InterruptTarget(Word targetId) {
    auto* pSrc = &m_Queue.top();
    auto* pTargetCtx = &m_Targets[targetId];

    /* Return if the source priority is too low for this target. */
    if(pSrc->priority < pTargetCtx->priorityThreshold) {
        return;
    }

    /* Return if this target doesn't have this interrupt enabled. */
    if(!this->GetEnabled(static_cast<int>(this->GetSourceIndex(pSrc->pImpl)), static_cast<int>(targetId))) {
        return;
    }

    /* Interrupt target. */
    pTargetCtx->pTarget->ReceiveInterruptRequest();
}

void PLIC::InterruptAllTargets() {
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
        this->InterruptTarget(i);
    }

    /* Clear claimed flag. */
    m_Claimed = false;
}

Word PLIC::GetSourceIndex(const SourceImpl* pSrc) const noexcept {
    return static_cast<Word>(pSrc - m_Sources.data());
}

} // namespace intrpt
} // namespace riscv
