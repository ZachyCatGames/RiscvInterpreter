#include <RiscvEmu/hw/hw_Scheduler.h>

namespace riscv {
namespace hw {

DeviceScheduler::DeviceScheduler() :
    m_Queue(CompareImpl(this)),
    m_Timer(0) {}

DeviceScheduler::SchdResult DeviceScheduler::ProcessCycle() {
    while(this->GetTopCycle() == m_Timer) {
        auto pDev = this->GetTopDevice();

        /* Process the current device. */
        Result res = pDev->ProcessCycle();
        if(res.IsFailure()) {
            return { res, pDev };
        }

        /* Remove the device from the scheduler. */
        m_Queue.pop();

        /* Re-add to scheduler. */
        res = this->AddDevice(pDev);
        if(res.IsFailure()) {
            return { res, pDev };
        }
    }

    /* Increment timer. */
    m_Timer++;

    /* Reset timer if needed. */
    if(m_Timer >= c_TimerFreq) {
        m_Timer = 0;
    }

    return ResultSuccess();
}

Result DeviceScheduler::AddDevice(IDevice* pDev) {
    /* Calculate the first cycle we'll need to process this device. */
    auto nextCycle = this->CalculateNextCycle(pDev->GetFreq());

    /* Add to scheduler. */
    m_Queue.emplace(pDev, nextCycle);

    return ResultSuccess();
}

Result DeviceScheduler::RemoveDevice(IDevice* pDev) {
    /* Remove device. */
    m_Queue.remove({pDev});

    return ResultSuccess();
}

IDevice* DeviceScheduler::GetTopDevice() const { return m_Queue.top().GetDevice(); }

DWordS DeviceScheduler::GetTopCycle() const { return m_Queue.top().GetNextCycle(); }

DWordS DeviceScheduler::CalculateNextCycle(DWordS freq) const noexcept { return m_Timer + (c_TimerFreq / freq); }


} // namespace hw
} // namespace riscv
