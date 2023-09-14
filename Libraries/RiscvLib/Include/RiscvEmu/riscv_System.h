#pragma once
#include <RiscvEmu/result.h>
#include <RiscvEmu/riscv_Peripheral.h>
#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/hw/hw_Scheduler.h>
#include <RiscvEmu/mem/mem_MemoryController.h>
#include <memory>
#include <vector>

namespace riscv {

class System {
public:
    class MemCtlrAccessor {
    public:
        Result ReadByte(Byte* pOut, Address addr)   { return m_pMemCtlr->ReadByte(pOut, addr); }
        Result ReadHWord(HWord* pOut, Address addr) { return m_pMemCtlr->ReadHWord(pOut, addr); }
        Result ReadWord(Word* pOut, Address addr)   { return m_pMemCtlr->ReadWord(pOut, addr); }
        Result ReadDWord(DWord* pOut, Address addr) { return m_pMemCtlr->ReadDWord(pOut, addr); }
        Result WriteByte(Byte in, Address addr)     { return m_pMemCtlr->WriteByte(in, addr); }
        Result WriteHWord(HWord in, Address addr)   { return m_pMemCtlr->WriteHWord(in, addr); }
        Result WriteWord(Word in, Address addr)     { return m_pMemCtlr->WriteWord(in, addr); }
        Result WriteDWord(DWord in, Address addr)   { return m_pMemCtlr->WriteDWord(in, addr); }
    private:
        friend class System;
        constexpr MemCtlrAccessor(mem::MemoryController* pMemCtlr) noexcept :
            m_pMemCtlr(pMemCtlr) {}
    private:
        mem::MemoryController* m_pMemCtlr;
    }; // class MemCtlrAccessor

    class HartAccessor {
    public:
        void WritePC(NativeWord value) const noexcept { return m_pHart->WritePC(value); }
        void WriteGPR(int index, NativeWord value) const noexcept { m_pHart->WriteGPR(index, value); }

        NativeWord ReadPC() const noexcept { return m_pHart->ReadPC(); }
        NativeWord ReadGPR(int index) const noexcept { return m_pHart->ReadGPR(index); }
    private:
        friend class System;
        constexpr HartAccessor(cpu::Hart* pHart) noexcept :
            m_pHart(pHart) {}
    private:
        cpu::Hart* m_pHart;
    }; // class HartAccessor
public:
    System() = default;

    Result Initialize();

    Result InitializeMemRegions(const mem::RegionInfo* pRegions, std::size_t regionCount) {
        return m_MemCtlr.Initialize(pRegions, regionCount);
    }

    auto GetHartAccessor([[maybe_unused]] int index = 0) noexcept { return HartAccessor(&m_Hart); }

    auto GetMemCtlrAccessor() noexcept { return MemCtlrAccessor(&m_MemCtlr); }

    template<typename T>
    Result AddPeripheral(std::unique_ptr<T>&& pPeripheral) {
        this->AddPeripheralImpl(std::move(pPeripheral));
        return ResultSuccess();
    }

    template<std::derived_from<mem::IMmioDev> T>
    Result AddMmioPeripheral(std::unique_ptr<T>&& pPeripheral) {
        /* Add peripheral. */
        auto& p = this->AddPeripheralImpl(std::move(pPeripheral));

        /* Register peripheral with memory controller. */
        m_MemCtlr.AddMmioDev(p.get());

        return ResultSuccess();
    }    
private:
    template<typename T>
    auto& AddPeripheralImpl(std::unique_ptr<T>&& pPeripheral) {
        /* Move the peripheral to our list. */
        auto& p = m_PeripheralList.emplace_back(std::move(pPeripheral));

        /* Register peripheral with hardware scheduler if it's a hw::IDevice. */
        if constexpr(std::derived_from<T, hw::IDevice>) {
            m_DevScheduler.AddDevice(p.get());
        }

        return p;
    }

private:
    std::vector<std::unique_ptr<Peripheral>> m_PeripheralList;
    cpu::Hart m_Hart;
    mem::MemoryController m_MemCtlr;

    hw::DeviceScheduler m_DevScheduler;
};

} // namespace riscv
