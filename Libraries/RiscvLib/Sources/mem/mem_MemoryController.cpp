#include <RiscvEmu/mem/mem_MemoryController.h>

namespace riscv {
namespace mem {

Result MemoryController::Initialize(const RegionInfo* pRegions, std::size_t regionCount) {
    for(std::size_t i = 0; i < regionCount; i++) {
        const auto& curRegion = pRegions[i];

        /* Check if this new regions conflicts with any currently existing regions. */
        if (m_MemRegion.Includes(curRegion.GetStart()) ||
            m_MemRegion.Includes(curRegion.GetEnd() - 1) ||
            this->FindIoRegion(curRegion.GetStart()) != nullptr ||
            this->FindIoRegion(curRegion.GetEnd() - 1)) {
            return ResultRegionAlreadyExists();
        }

        /* Add new mem region if the current region is a mem region. */
        if(curRegion.GetType() == RegionType::Memory) {
            /* Setup the new MemRegion. */
            m_MemRegion.Initialize(curRegion);
        }
        else if(curRegion.GetType() == RegionType::IO) {
            /* Setup the new IoRegion. */
            m_IoRegions.emplace_back(curRegion);
        }
        else {
            return ResultInvalidRegionType();
        }
    }

    return ResultSuccess();
}

Result MemoryController::RemoveRegion(Address regionAddr) {
    /* Try removing an IO region. */
    return this->RemoveRegionImpl(m_IoRegions, regionAddr);
}

Result MemoryController::AddMmioDev(std::unique_ptr<IMmioDev>&& dev, Address addr) {
    /* Find the IO region we'll be placing this device in. */
    detail::IoRegion* pRegion = this->FindIoRegion(addr);
    if(!pRegion) {
        return ResultRegionDoesNotExist();
    }

    /* Make sure this new device doesn't overlap with an existing one. */
    auto relAddr = addr - pRegion->GetStart();
    auto len = dev->GetMappedSize();
    for(const auto& dev : pRegion->GetDevList()) {
        if(dev.Includes(relAddr) || dev.Includes(relAddr + len)) {
            return ResultDeviceAlreadyExists();
        }
    }

    /* Add the new device. */
    pRegion->GetDevList().emplace_back(relAddr, len, std::move(dev));

    return ResultSuccess();
}

Result MemoryController::ReadByte(Byte* pOut, Address addr) {
    /* First let's check if this address is in main memory. */
    if(m_MemRegion.Includes(addr)) {
        return m_MemRegion.ReadByte(pOut, addr);
    }

    /* Next if that fails let's try reading from an IO device. */
    detail::IoDev* pDev = this->FindIoDevice(addr, sizeof(*pOut));
    if(pDev) {
        return pDev->GetDevice()->ReadByte(pOut, addr);
    }

    return ResultReadAccessFault();
}

detail::IoRegion* MemoryController::FindIoRegion(Address addr) {
    return this->FindRegionImpl(m_IoRegions, addr);
}

detail::IoDev* MemoryController::FindIoDevice(Address addr, NativeWord len) {
    /* First let's find the region containing this device. */
    detail::IoRegion* pRegion = this->FindIoRegion(addr);
    if(!pRegion) {
        return nullptr;
    }

    /* Next let's find the device within the IO region. */
    auto relAddr = addr - pRegion->GetStart();
    detail::IoDev* pDev = pRegion->FindDevice(relAddr);
    if(!pDev) {
        return nullptr;
    }

    /* Finally let's make sure our access doesn't go out of bounds. */
    if(relAddr + len > pDev->GetEnd()) {
        return nullptr;
    }

    return pDev;
}

} // namespace mem
} // namespace riscv
