#include <RiscvEmu/mem/mem_MemoryController.h>
#include <RiscvEmu/mem/mem_MCClient.h>
#include <RiscvEmu/mem/mem_MmioTemplateAdapter.h>
#include <span>

namespace riscv {
namespace mem {

MemoryController::MemoryController() = default;

Result MemoryController::Initialize(const RegionInfo* pRegions, std::size_t count) {
    return this->Initialize(std::span<const RegionInfo>(pRegions, count));
}

MCClient MemoryController::GetClient() { return MCClient(this); }

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

Result MemoryController::AddRegionImpl(const RegionInfo* pRegion) {
    /* Check if this new regions conflicts with any currently existing regions. */
    if (m_MemRegion.Includes(pRegion->GetStart()) ||
        m_MemRegion.Includes(pRegion->GetEnd() - 1) ||
        this->FindIoRegion(pRegion->GetStart()) != nullptr ||
        this->FindIoRegion(pRegion->GetEnd() - 1) != nullptr) {
        return ResultRegionAlreadyExists();
    }

    /* Add new mem region if the current region is a mem region. */
    if(pRegion->GetType() == RegionType::Memory) {
        /* Setup the new MemRegion. */
        m_MemRegion.Initialize(*pRegion);
    }
    else if(pRegion->GetType() == RegionType::IO) {
        /* Setup the new IoRegion. */
        m_IoRegions.emplace_back(*pRegion);
    }
    else {
        return ResultInvalidRegionType();
    }

    return ResultSuccess();
}

template<std::unsigned_integral WordType>
Result MemoryController::LoadImpl(WordType* pOut, Address addr) {
    /* First let's check if this address is in main memory. */
    if(m_MemRegion.Includes(addr)) {
        return m_MemRegion.Load<WordType>(pOut, addr - m_MemRegion.GetStart());
    }
    /* Next, if that fails let's try reading/writing from/to an IO device. */
    detail::IoDev* pDev = this->FindIoDevice(addr, sizeof(std::remove_pointer_t<WordType>));
    if(pDev) {
        return MmioTemplateAdapter(pDev->GetDevice()).Load(pOut, addr - pDev->GetStart());
    }
    return ResultReadAccessFault();
}

template Result MemoryController::LoadImpl<Byte>(Byte*, Address);
template Result MemoryController::LoadImpl<HWord>(HWord*, Address);
template Result MemoryController::LoadImpl<Word>(Word*, Address);
template Result MemoryController::LoadImpl<DWord>(DWord*, Address);

template<std::signed_integral WordType>
Result MemoryController::LoadImpl(WordType* pOut, Address addr) {
    using Unsigned = std::make_unsigned_t<WordType>;
    Unsigned tmp;

    /* Read value as unsigned. */
    Result res = this->LoadImpl(&tmp, addr);
    if(res.IsFailure()) {
        return res;
    }

    /* Convert value to signed. */
    *pOut = static_cast<WordType>(tmp);

    return ResultSuccess();
}

template Result MemoryController::LoadImpl<ByteS>(ByteS*, Address);
template Result MemoryController::LoadImpl<HWordS>(HWordS*, Address);
template Result MemoryController::LoadImpl<WordS>(WordS*, Address);
template Result MemoryController::LoadImpl<DWordS>(DWordS*, Address);

template<std::floating_point WordType>
Result MemoryController::LoadImpl(WordType* pOut, Address addr) {
    using TmpType = std::conditional_t<std::is_same_v<WordType, float>, Word, DWord>;

    /* Read word. */
    TmpType tmp;
    Result res = this->LoadImpl<TmpType>(&tmp, addr);

    /* Bitcast value to float/double. */
    *pOut = std::bit_cast<WordType>(tmp);

    return ResultSuccess();
}

template Result MemoryController::LoadImpl<float>(float*, Address);
template Result MemoryController::LoadImpl<double>(double*, Address);

template<std::unsigned_integral WordType>
Result MemoryController::StoreImpl(WordType in, Address addr) {
    /* First let's check if this address is in main memory. */
    if(m_MemRegion.Includes(addr)) {
        /* Acquire lock on reservation info. */
        std::scoped_lock lock(m_ReserveMutex);

        /* Invalidate reservation on addr. */
        this->InvalidateReservation(addr);

        /* Perform store. */
        return m_MemRegion.Store<WordType>(in, addr - m_MemRegion.GetStart());
    }
    /* Next, if that fails let's try reading/writing from/to an IO device. */
    detail::IoDev* pDev = this->FindIoDevice(addr, sizeof(WordType));
    if(pDev) {
        /* Acquire lock on reservation info. */
        std::scoped_lock lock(m_ReserveMutex);

        /* Invalidate reservation on addr. */
        this->InvalidateReservation(addr);

        /* Perform store. */
        return MmioTemplateAdapter(pDev->GetDevice()).Store(in, addr - pDev->GetStart());
    }
    return ResultWriteAccessFault();
}

template Result MemoryController::StoreImpl<Byte>(Byte, Address);
template Result MemoryController::StoreImpl<HWord>(HWord, Address);
template Result MemoryController::StoreImpl<Word>(Word, Address);
template Result MemoryController::StoreImpl<DWord>(DWord, Address);

template<std::signed_integral WordType>
Result MemoryController::StoreImpl(WordType in, Address addr) {
    /* Convert value to unsigned value and store it. */
    using Unsigned = std::make_unsigned_t<WordType>;
    return this->StoreImpl(static_cast<Unsigned>(in), addr);
}

template Result MemoryController::StoreImpl<ByteS>(ByteS, Address);
template Result MemoryController::StoreImpl<HWordS>(HWordS, Address);
template Result MemoryController::StoreImpl<WordS>(WordS, Address);
template Result MemoryController::StoreImpl<DWordS>(DWordS, Address);

template<std::floating_point WordType>
Result MemoryController::StoreImpl(WordType in, Address addr) {
    /* Convert value to word type then store it. */
    using TmpType = std::conditional_t<std::is_same_v<WordType, float>, Word, DWord>;
    return this->StoreImpl(std::bit_cast<TmpType>(in), addr);
}

template Result MemoryController::StoreImpl<float>(float, Address);
template Result MemoryController::StoreImpl<double>(double, Address);

template<typename WordType>
Result MemoryController::LoadReserve(MCClient* pClient, WordType* pOut, Address addr) {
    /* Acquire lock on reservation info. */
    std::scoped_lock lock(m_ReserveMutex);

    /* Add a reservation. */
    this->AddReservation(pClient, addr);

    /* Perform the load. */
    return this->LoadImpl(pOut, addr);
}

template Result MemoryController::LoadReserve<Byte>(MCClient*, Byte*, Address);
template Result MemoryController::LoadReserve<HWord>(MCClient*, HWord*, Address);
template Result MemoryController::LoadReserve<Word>(MCClient*, Word*, Address);
template Result MemoryController::LoadReserve<DWord>(MCClient*, DWord*, Address);
template Result MemoryController::LoadReserve<ByteS>(MCClient*, ByteS*, Address);
template Result MemoryController::LoadReserve<HWordS>(MCClient*, HWordS*, Address);
template Result MemoryController::LoadReserve<WordS>(MCClient*, WordS*, Address);
template Result MemoryController::LoadReserve<DWordS>(MCClient*, DWordS*, Address);
template Result MemoryController::LoadReserve<float>(MCClient*, float*, Address);
template Result MemoryController::LoadReserve<double>(MCClient*, double*, Address);

template<typename WordType>
Result MemoryController::StoreConditional(MCClient* pClient, WordType* pInOut, Address addr) {
    /* Acquire lock on reservation info. */
    std::scoped_lock lock(m_ReserveMutex);

    /* Check if we still have our reservation. */
    if(!this->HasReservation(pClient, addr)) {
        /* Clean pInOut then return. */
        *pInOut = 1;
        return ResultSuccess();
    }

    /* Invalidate any reservations on this address. */
    this->InvalidateReservation(addr);

    /* Save pInOut and clear it. */
    auto tmp = *pInOut;
    *pInOut = 0;

    /* Perform store. */
    return this->StoreImpl(tmp, addr);
}

template Result MemoryController::StoreConditional<Byte>(MCClient*, Byte*, Address);
template Result MemoryController::StoreConditional<HWord>(MCClient*, HWord*, Address);
template Result MemoryController::StoreConditional<Word>(MCClient*, Word*, Address);
template Result MemoryController::StoreConditional<DWord>(MCClient*, DWord*, Address);
template Result MemoryController::StoreConditional<ByteS>(MCClient*, ByteS*, Address);
template Result MemoryController::StoreConditional<HWordS>(MCClient*, HWordS*, Address);
template Result MemoryController::StoreConditional<WordS>(MCClient*, WordS*, Address);
template Result MemoryController::StoreConditional<DWordS>(MCClient*, DWordS*, Address);
template Result MemoryController::StoreConditional<float>(MCClient*, float*, Address);
template Result MemoryController::StoreConditional<double>(MCClient*, double*, Address);

bool MemoryController::HasReservation(MCClient* pClient, Address addr) {
    /* Do nothing if reservations list is emtpy. */
    if(!m_Reservations.empty()) {
        return false;
    }

    /* Cut off lower bits. */
    addr >>= ReserveGranularity;

    /* Check all active reservation entries. */
    for(const auto& reservation : m_Reservations) {
        if(reservation.pClient == pClient && reservation.addr == addr) {
            return true;
        }
    }

    return false;
}

void MemoryController::AddReservation(MCClient* pClient, Address addr) {
    /* If we already have a reservation, clear it. */
    if(!m_Reservations.empty()) {
        m_Reservations.erase(std::find_if(m_Reservations.begin(), m_Reservations.end(), [pClient](const auto& it) { return it.pClient == pClient; }));
    }

    /* Cut off lower bits. */
    addr >>= ReserveGranularity;

    /* Add reservation. */
    m_Reservations.push_back(ReserveEntry{addr, pClient});
}

void MemoryController::InvalidateReservation(Address addr) {
    /* Do nothing if reservations list is emtpy. */
    if(m_Reservations.empty()) {
        return;
    }

    /* Cut off lower bits. */
    addr >>= ReserveGranularity;

    /* Find and erase all resevations on this address. */
    for(auto it = m_Reservations.begin(); it != m_Reservations.end(); it++) {
        if(it->addr == addr) {
            it = m_Reservations.erase(it);
        }
    }
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
