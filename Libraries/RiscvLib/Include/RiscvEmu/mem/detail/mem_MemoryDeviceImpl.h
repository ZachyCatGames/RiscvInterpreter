#pragma once
#include <RiscvEmu/result.h>
#include <RiscvEmu/riscv_Types.h>
#include <bit>

namespace riscv {
namespace mem {
namespace detail {

template<typename T>
class MemoryDeviceImpl {
public:
    constexpr MemoryDeviceImpl() noexcept = default;

    constexpr MemoryDeviceImpl(T&& pMem) :
        m_pMem(std::forward<T>(pMem)) {}

    constexpr void Initialize(T&& pMem) {
        m_pMem = std::forward<T>(pMem);
    }

    constexpr Result ReadByteImpl(Byte* pOut, Address addr) {
        *pOut = m_pMem[addr];
        return ResultSuccess();
    }

    constexpr Result ReadHWordImpl(HWord* pOut, Address addr) {
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

    constexpr Result ReadWordImpl(Word* pOut, Address addr) {
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

    constexpr Result ReadDWordImpl(DWord* pOut, Address addr) {
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

    constexpr Result WriteByteImpl(Byte in, Address addr) {
        m_pMem[addr] = in;
        return ResultSuccess();
    }

    constexpr Result WriteHWordImpl(HWord in, Address addr) {
        if constexpr(std::endian::native == std::endian::little) {
            /* On little endian we go from lower to higher. */
            m_pMem[addr + 0] = in >> 0 & 0xFF;
            m_pMem[addr + 1] = in >> 8 & 0xFF;
        }
        else /* if constexpr(std::endian::native == std::endian::big) */ {
            /* On big endian we go from higher to lower. */
            m_pMem[addr + 0] = in >> 8 & 0xFF;
            m_pMem[addr + 1] = in >> 0 & 0xFF;
        }
        return ResultSuccess();
    }

    constexpr Result WriteWordImpl(Word in, Address addr) {
        if constexpr(std::endian::native == std::endian::little) {
            /* On little endian we go from lower to higher. */
            m_pMem[addr + 0] = in >>  0 & 0xFF;
            m_pMem[addr + 1] = in >>  8 & 0xFF;
            m_pMem[addr + 2] = in >> 16 & 0xFF;
            m_pMem[addr + 3] = in >> 24 & 0xFF;
        }
        else /* if constexpr(std::endian::native == std::endian::big) */ {
            /* On big endian we go from higher to lower. */
            m_pMem[addr + 0] = in >> 24 & 0xFF;
            m_pMem[addr + 1] = in >> 16 & 0xFF;
            m_pMem[addr + 2] = in >>  8 & 0xFF;
            m_pMem[addr + 3] = in >>  0 & 0xFF;
        }
        return ResultSuccess();
    }

    constexpr Result WriteDWordImpl(DWord in, Address addr) {
        if constexpr(std::endian::native == std::endian::little) {
            /* On little endian we go from lower to higher. */
            m_pMem[addr + 0] = in >>  0 & 0xFF;
            m_pMem[addr + 1] = in >>  8 & 0xFF;
            m_pMem[addr + 2] = in >> 16 & 0xFF;
            m_pMem[addr + 3] = in >> 24 & 0xFF;
            m_pMem[addr + 4] = in >> 32 & 0xFF;
            m_pMem[addr + 5] = in >> 40 & 0xFF;
            m_pMem[addr + 6] = in >> 48 & 0xFF;
            m_pMem[addr + 7] = in >> 56 & 0xFF;
        }
        else /* if constexpr(std::endian::native == std::endian::big) */ {
            /* On big endian we go from higher to lower. */
            m_pMem[addr + 0] = in >> 56 & 0xFF;
            m_pMem[addr + 1] = in >> 48 & 0xFF;
            m_pMem[addr + 2] = in >> 40 & 0xFF;
            m_pMem[addr + 3] = in >> 32 & 0xFF;
            m_pMem[addr + 4] = in >> 24 & 0xFF;
            m_pMem[addr + 5] = in >> 16 & 0xFF;
            m_pMem[addr + 6] = in >>  8 & 0xFF;
            m_pMem[addr + 7] = in >>  0 & 0xFF;
        }
        return ResultSuccess();
    }
private:
    T m_pMem;
}; // class MemoryDeviceImpl

} // namespace detail
} // namespace mem
} // namespace riscv
