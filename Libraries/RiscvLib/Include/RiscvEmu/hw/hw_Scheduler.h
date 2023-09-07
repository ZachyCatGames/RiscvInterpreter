#pragma once
#include <RiscvEmu/result.h>
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/hw/hw_IDevice.h>
#include <RiscvEmu/util/util_RemovablePrioQueue.h>
#include <vector>

namespace riscv {
namespace hw {

/**
 * This is a scheduler for hw::IDevice devices.
 *
 * This will schedule devices relative to eachother and an internal timer, not in realtime.
 * The timer will increment every time ProcessCycle is called.
 *
 * When a device will be processed is calculated by timer + timer_max / dev_feq.
 *
 * i.e. if timer_max is 10, devA runs at 5hz, and devB runs at 10hz
 * devB will run every cycle (10/10 = 1), and devA will run every alternating cycle (10/5 = 2).)
 *
 * NOTE: timer_max is currently non-configurable.
 *
 * TODO: Make timer_max configurable.
*/
class DeviceScheduler {
public:
    class SchdResult : public Result {
    public:
        constexpr SchdResult() noexcept :
            Result(ResultSuccess()),
            m_pDev(nullptr) {}

        constexpr SchdResult(ResultSuccess res) noexcept :
            Result(res.GetValue()),
            m_pDev(nullptr) {}

        constexpr SchdResult(Result res, IDevice* pDev = nullptr) noexcept :
            Result(res.GetValue()),
            m_pDev(pDev) {}

        constexpr IDevice* GetDevice() const noexcept { return m_pDev; }
    private:
        IDevice* m_pDev;
    }; // class SchdResult
public:
    /** Default constructor. */
    DeviceScheduler();

    /**
     * Processes a single cycle.
     *
     * If any device return a non-ResultSuccess result when its ProcessCycle
     * function is called, no further devices will be processed and the internal
     * timer will not be updated, a SchdResult containing the Result and offending
     * device will be returned.
     *
     * @return IDevice::ProcessCycle error if non-Success, otherwise ResultSuccess().
     * */
    SchdResult ProcessCycle();

    /**
     * Add a device to the scheduler.
     *
     * @param[in] pDev  Device to add.
     * @return ResultSuccess()
     */
    Result AddDevice(IDevice* pDev);

    /**
     * Remove a device from the scheduler.
     *
     * @param[in] pDev  Device to remove.
     * @return ResultSuccess()
    */
    Result RemoveDevice(IDevice* pDev);
private:
    class DeviceContainer {
    public:
        constexpr DeviceContainer(IDevice* pDev, DWordS next = 0) noexcept :
            m_pDev(pDev),
            m_NextCycle(next) {}

        constexpr auto GetDevice() const noexcept { return m_pDev; }
        constexpr auto GetNextCycle() const noexcept { return m_NextCycle; }

        constexpr bool operator==(const DeviceContainer& rhs) const noexcept { return this->m_pDev == rhs.m_pDev; }
    private:
        IDevice* m_pDev;
        DWordS m_NextCycle;
    }; // class DeviceContainer

    class CompareImpl {
    public:
        constexpr CompareImpl(DeviceScheduler* pParent) :
            m_pParent(pParent) {}

        constexpr bool operator()(const DeviceContainer& lhs, const DeviceContainer& rhs) const noexcept {
            return this->GetNextCycle(lhs) < this->GetNextCycle(rhs);
        }
    private:
        constexpr DWordS GetNextCycle(const DeviceContainer& pair) const noexcept {
            auto next = pair.GetNextCycle();
            if(next <= m_pParent->m_Timer) {
                return next + DeviceScheduler::c_TimerFreq;
            }
            return next;
        }
    private:
        DeviceScheduler* const m_pParent;
    }; // class CompareImpl
private:
    IDevice* GetTopDevice() const;
    DWordS GetTopCycle() const;
    DWordS CalculateNextCycle(DWordS freq) const noexcept;
private:
    util::RemovablePrioQueue<DeviceContainer, std::vector<DeviceContainer>, CompareImpl> m_Queue;
    static constexpr DWordS c_TimerFreq = 1'000'000;
    DWordS m_Timer;
}; // class DeviceScheduler

} // namespace hw
} // namespace riscv
