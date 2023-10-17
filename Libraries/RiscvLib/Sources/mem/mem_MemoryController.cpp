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
            this->FindIoRegion(curRegion.GetEnd() - 1) != nullptr) {
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

Result MemoryController::AddMmioDev(IMmioDev* pDev, Address addr) {
    /* Find the IO region we'll be placing this device in. */
    detail::IoRegion* pRegion = this->FindIoRegion(addr);
    if(!pRegion) {
        return ResultRegionDoesNotExist();
    }

    /* Make sure this new device doesn't overlap with an existing one. */
    auto len = pDev->GetMappedSize();
    for(const auto& dev : pRegion->GetDevList()) {
        if(dev.Includes(addr) || dev.Includes(addr + len)) {
            return ResultDeviceAlreadyExists();
        }
    }

    /* Add the new device. */
    pRegion->GetDevList().emplace_back(addr, len, pDev);

    return ResultSuccess();
}

Result MemoryController::ReadByte(Byte* pOut, Address addr) {
    return this->ReadWriteImpl<&decltype(m_MemRegion)::ReadByte, &IMmioDev::ReadByte>(pOut, addr);
}

Result MemoryController::ReadHWord(HWord* pOut, Address addr) {
    return this->ReadWriteImpl<&decltype(m_MemRegion)::ReadHWord, &IMmioDev::ReadHWord>(pOut, addr);
}

Result MemoryController::ReadWord(Word* pOut, Address addr) {
    return this->ReadWriteImpl<&decltype(m_MemRegion)::ReadWord, &IMmioDev::ReadWord>(pOut, addr);
}

Result MemoryController::ReadDWord(DWord* pOut, Address addr) {
    return this->ReadWriteImpl<&decltype(m_MemRegion)::ReadDWord, &IMmioDev::ReadDWord>(pOut, addr);
}

Result MemoryController::ReadNativeWord(NativeWord* pOut, Address addr) {
    Result res;
    if constexpr (cfg::cpu::EnableIsaRV64I) {
        DWord val = 0;
        res = this->ReadDWord(&val, addr);
        *pOut = static_cast<NativeWord>(val);
    }
    else {
        Word val = 0;
        res = this->ReadWord(&val, addr);
        *pOut = static_cast<NativeWord>(val);
    }
    return res;
}

Result MemoryController::WriteByte(Byte in, Address addr) {
    return this->ReadWriteImpl<&decltype(m_MemRegion)::WriteByte, &IMmioDev::WriteByte>(in, addr);
}

Result MemoryController::WriteHWord(HWord in, Address addr) {
    return this->ReadWriteImpl<&decltype(m_MemRegion)::WriteHWord, &IMmioDev::WriteHWord>(in, addr);
}

Result MemoryController::WriteWord(Word in, Address addr) {
    return this->ReadWriteImpl<&decltype(m_MemRegion)::WriteWord, &IMmioDev::WriteWord>(in, addr);
}

Result MemoryController::WriteDWord(DWord in, Address addr) {
    return this->ReadWriteImpl<&decltype(m_MemRegion)::WriteDWord, &IMmioDev::WriteDWord>(in, addr);
}

Result MemoryController::WriteNativeWord(NativeWord in, Address addr) {
    if constexpr (cfg::cpu::EnableIsaRV64I) {
        return this->WriteDWord(static_cast<NativeWord>(in), addr);
    }
    else {
        return this->WriteWord(static_cast<NativeWord>(in), addr);
    }
}

template<auto MemRead, auto IoRead, typename T>
Result MemoryController::ReadWriteImpl(T pOut, Address addr) {
    /* First let's check if this address is in main memory. */
    if(m_MemRegion.Includes(addr)) {
        return (m_MemRegion.*MemRead)(pOut, addr - m_MemRegion.GetStart());
    }
    /* Next if that fails let's try reading/writing from/to an IO device. */
    detail::IoDev* pDev = this->FindIoDevice(addr, sizeof(std::remove_pointer_t<T>));
    if(pDev) {
        return (*pDev->GetDevice().*IoRead)(pOut, addr - pDev->GetStart());
    }
    return ResultReadAccessFault();
}

template<typename T>
T* MemoryController::FindRegionImpl(std::vector<T>& regionList, Address addr) {
    for(auto& region : regionList) {
        if(region.Includes(addr)) {
            return &region;
        }
    }
    return nullptr;
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
    if(!pDev->Includes(relAddr + len)) {
        return nullptr;
    }

    return pDev;
}

} // namespace mem
} // namespace riscv
