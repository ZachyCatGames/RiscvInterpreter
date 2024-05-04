#include <RiscvEmu/mem/detail/mem_MemoryDeviceImpl.h>

namespace riscv {
namespace mem {
namespace detail {

MemoryDeviceImpl::MemoryDeviceImpl(PtrType&& pMem) : m_pMem(std::move(pMem)) {}
MemoryDeviceImpl::MemoryDeviceImpl(NativeWord size) : m_pMem(std::make_unique<Byte[]>(size)) {}

Result MemoryDeviceImpl::Initialize(PtrType&& pMem) {
    m_pMem = std::move(pMem);
    return ResultSuccess();
}
Result MemoryDeviceImpl::Initialize(NativeWord size) {
    m_pMem = std::make_unique<Byte[]>(size);
    return ResultSuccess();
}

template<>
Result MemoryDeviceImpl::LoadImpl<Byte>(Byte* pOut, Address addr) {
    *pOut = m_pMem[addr];
    return ResultSuccess();
}

template<>
Result MemoryDeviceImpl::LoadImpl<HWord>(HWord* pOut, Address addr) {
    if constexpr(std::endian::native == std::endian::little) {
        /* On little endian we go from lower to higher.*/
        *pOut = static_cast<HWord>(m_pMem[addr + 0] << 0) |
                static_cast<HWord>(m_pMem[addr + 1] << 8);
    }
    else /* if constexpr(std::endian::native == std::endian::big) */ {
        /* On big endian we go from higher to lower*/
        *pOut = static_cast<HWord>(m_pMem[addr + 0] << 8) |
                static_cast<HWord>(m_pMem[addr + 1] << 0);
    }
    return ResultSuccess();
}

template<>
Result MemoryDeviceImpl::LoadImpl<Word>(Word* pOut, Address addr) {
    if constexpr(std::endian::native == std::endian::little) {
        /* On little endian we go from lower to higher.*/
        *pOut = static_cast<Word>(m_pMem[addr + 0] <<  0) |
                static_cast<Word>(m_pMem[addr + 1] <<  8) |
                static_cast<Word>(m_pMem[addr + 2] << 16) |
                static_cast<Word>(m_pMem[addr + 3] << 24);
    }
    else /* if constexpr(std::endian::native == std::endian::big) */ {
        /* On big endian we go from higher to lower*/
        *pOut = static_cast<Word>(m_pMem[addr + 0] << 24) |
                static_cast<Word>(m_pMem[addr + 1] << 16) |
                static_cast<Word>(m_pMem[addr + 2] <<  8) |
                static_cast<Word>(m_pMem[addr + 3] <<  0);
    }
    return ResultSuccess();
}

template<>
Result MemoryDeviceImpl::LoadImpl<DWord>(DWord* pOut, Address addr) {
    if constexpr(std::endian::native == std::endian::little) {
        /* On little endian we go from lower to higher.*/
        *pOut = static_cast<DWord>(m_pMem[addr + 0]) <<  0 |
                static_cast<DWord>(m_pMem[addr + 1]) <<  8 |
                static_cast<DWord>(m_pMem[addr + 2]) << 16 |
                static_cast<DWord>(m_pMem[addr + 3]) << 24 |
                static_cast<DWord>(m_pMem[addr + 4]) << 32 |
                static_cast<DWord>(m_pMem[addr + 5]) << 40 |
                static_cast<DWord>(m_pMem[addr + 6]) << 48 |
                static_cast<DWord>(m_pMem[addr + 7]) << 56;
    }
    else /* if constexpr(std::endian::native == std::endian::big) */ {
        /* On big endian we go from higher to lower*/
        *pOut = static_cast<DWord>(m_pMem[addr + 0]) << 56 |
                static_cast<DWord>(m_pMem[addr + 1]) << 48 |
                static_cast<DWord>(m_pMem[addr + 2]) << 40 |
                static_cast<DWord>(m_pMem[addr + 3]) << 32 |
                static_cast<DWord>(m_pMem[addr + 4]) << 24 |
                static_cast<DWord>(m_pMem[addr + 5]) << 16 |
                static_cast<DWord>(m_pMem[addr + 6]) <<  8 |
                static_cast<DWord>(m_pMem[addr + 7]) <<  0;
    }
    return ResultSuccess();
}

template<>
Result MemoryDeviceImpl::StoreImpl<Byte>(Byte in, Address addr) {
    m_pMem[addr] = in;
    return ResultSuccess();
}

template<>
Result MemoryDeviceImpl::StoreImpl<HWord>(HWord in, Address addr) {
    if constexpr(std::endian::native == std::endian::little) {
        /* On little endian we go from lower to higher. */
        m_pMem[addr + 0] = static_cast<Byte>(in >> 0u & 0xFFu);
        m_pMem[addr + 1] = static_cast<Byte>(in >> 8u & 0xFFu);
    }
    else /* if constexpr(std::endian::native == std::endian::big) */ {
        /* On big endian we go from higher to lower. */
        m_pMem[addr + 0] = static_cast<Byte>(in >> 8 & 0xFF);
        m_pMem[addr + 1] = static_cast<Byte>(in >> 0 & 0xFF);
    }
    return ResultSuccess();
}

template<>
Result MemoryDeviceImpl::StoreImpl<Word>(Word in, Address addr) {
        if constexpr(std::endian::native == std::endian::little) {
            /* On little endian we go from lower to higher. */
            m_pMem[addr + 0] = static_cast<Byte>(in >>  0 & 0xFF);
            m_pMem[addr + 1] = static_cast<Byte>(in >>  8 & 0xFF);
            m_pMem[addr + 2] = static_cast<Byte>(in >> 16 & 0xFF);
            m_pMem[addr + 3] = static_cast<Byte>(in >> 24 & 0xFF);
        }
        else /* if constexpr(std::endian::native == std::endian::big) */ {
            /* On big endian we go from higher to lower. */
            m_pMem[addr + 0] = static_cast<Byte>(in >> 24 & 0xFF);
            m_pMem[addr + 1] = static_cast<Byte>(in >> 16 & 0xFF);
            m_pMem[addr + 2] = static_cast<Byte>(in >>  8 & 0xFF);
            m_pMem[addr + 3] = static_cast<Byte>(in >>  0 & 0xFF);
        }
        return ResultSuccess();
}

template<>
Result MemoryDeviceImpl::StoreImpl<DWord>(DWord in, Address addr) {
        if constexpr(std::endian::native == std::endian::little) {
            /* On little endian we go from lower to higher. */
            m_pMem[addr + 0] = static_cast<Byte>(in >>  0 & 0xFF);
            m_pMem[addr + 1] = static_cast<Byte>(in >>  8 & 0xFF);
            m_pMem[addr + 2] = static_cast<Byte>(in >> 16 & 0xFF);
            m_pMem[addr + 3] = static_cast<Byte>(in >> 24 & 0xFF);
            m_pMem[addr + 4] = static_cast<Byte>(in >> 32 & 0xFF);
            m_pMem[addr + 5] = static_cast<Byte>(in >> 40 & 0xFF);
            m_pMem[addr + 6] = static_cast<Byte>(in >> 48 & 0xFF);
            m_pMem[addr + 7] = static_cast<Byte>(in >> 56 & 0xFF);
        }
        else /* if constexpr(std::endian::native == std::endian::big) */ {
            /* On big endian we go from higher to lower. */
            m_pMem[addr + 0] = static_cast<Byte>(in >> 56 & 0xFF);
            m_pMem[addr + 1] = static_cast<Byte>(in >> 48 & 0xFF);
            m_pMem[addr + 2] = static_cast<Byte>(in >> 40 & 0xFF);
            m_pMem[addr + 3] = static_cast<Byte>(in >> 32 & 0xFF);
            m_pMem[addr + 4] = static_cast<Byte>(in >> 24 & 0xFF);
            m_pMem[addr + 5] = static_cast<Byte>(in >> 16 & 0xFF);
            m_pMem[addr + 6] = static_cast<Byte>(in >>  8 & 0xFF);
            m_pMem[addr + 7] = static_cast<Byte>(in >>  0 & 0xFF);
        }
        return ResultSuccess();
}

} // namespace detail
} // namespace mem
} // namespace riscv
