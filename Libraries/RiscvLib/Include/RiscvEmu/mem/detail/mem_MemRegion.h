#pragma once
#include <RiscvEmu/mem/detail/mem_RegionBase.h>
#include <RiscvEmu/mem/mem_RegionInfo.h>
#include <RiscvEmu/mem/mem_Result.h>
#include <memory>
#include <bit>

namespace riscv {
namespace mem {
namespace detail {

class MemRegion : public RegionBase {
public:
    constexpr MemRegion(const RegionInfo& info) :
        RegionBase(info.start, info.length),
        pMem(std::make_unique<Byte[]>(info.length)) {}

    constexpr Byte* GetMem() noexcept { return pMem.get(); }
    constexpr const Byte* GetMem() const noexcept { return pMem.get(); }

    constexpr Result ReadByte(Byte* pOut, Address addr) {
        *pOut = pMem[addr];
        return ResultSuccess();
    }

    constexpr Result ReadHWord(HWord* pOut, Address addr) {
        if constexpr(std::endian::native == std::endian::little) {
            /* On little endian we go from lower to higher.*/
            *pOut = static_cast<HWord>(pMem[addr + 0]) << 0 |
                    static_cast<HWord>(pMem[addr + 1]) << 8;
        }
        else /* if constexpr(std::endian::native == std::endian::big) */ {
            /* On big endian we go from higher to lower*/
            *pOut = static_cast<HWord>(pMem[addr + 0]) << 8 |
                    static_cast<HWord>(pMem[addr + 1]) << 0;
        }
        return ResultSuccess();
    }

    constexpr Result ReadWord(Word* pOut, Address addr) {
        if constexpr(std::endian::native == std::endian::little) {
            /* On little endian we go from lower to higher.*/
            *pOut = static_cast<Word>(pMem[addr + 0]) <<  0 |
                    static_cast<Word>(pMem[addr + 1]) <<  8 |
                    static_cast<Word>(pMem[addr + 2]) << 16 |
                    static_cast<Word>(pMem[addr + 3]) << 24;
        }
        else /* if constexpr(std::endian::native == std::endian::big) */ {
            /* On big endian we go from higher to lower*/
            *pOut = static_cast<Word>(pMem[addr + 0]) << 24 |
                    static_cast<Word>(pMem[addr + 1]) << 16 |
                    static_cast<Word>(pMem[addr + 2]) <<  8 |
                    static_cast<Word>(pMem[addr + 3]) <<  0;
        }
        return ResultSuccess();
    }

    constexpr Result ReadDWord(DWord* pOut, Address addr) {
        if constexpr(std::endian::native == std::endian::little) {
            /* On little endian we go from lower to higher.*/
            *pOut = static_cast<DWord>(pMem[addr + 0]) <<  0 |
                    static_cast<DWord>(pMem[addr + 1]) <<  8 |
                    static_cast<DWord>(pMem[addr + 2]) << 16 |
                    static_cast<DWord>(pMem[addr + 3]) << 24 |
                    static_cast<DWord>(pMem[addr + 4]) << 32 |
                    static_cast<DWord>(pMem[addr + 5]) << 40 |
                    static_cast<DWord>(pMem[addr + 6]) << 48 |
                    static_cast<DWord>(pMem[addr + 7]) << 56;
        }
        else /* if constexpr(std::endian::native == std::endian::big) */ {
            /* On big endian we go from higher to lower*/
            *pOut = static_cast<DWord>(pMem[addr + 0]) << 56 |
                    static_cast<DWord>(pMem[addr + 1]) << 48 |
                    static_cast<DWord>(pMem[addr + 2]) << 40 |
                    static_cast<DWord>(pMem[addr + 3]) << 32 |
                    static_cast<DWord>(pMem[addr + 4]) << 24 |
                    static_cast<DWord>(pMem[addr + 5]) << 16 |
                    static_cast<DWord>(pMem[addr + 6]) <<  8 |
                    static_cast<DWord>(pMem[addr + 7]) <<  0;
        }
        return ResultSuccess();
    }

    constexpr Result WriteByte(Byte in, Address addr) {
        pMem[addr] = in;
        return ResultSuccess();
    }

    constexpr Result WriteHWord(HWord in, Address addr) {
        if constexpr(std::endian::native == std::endian::little) {
            /* On little endian we go from lower to higher. */
            pMem[addr + 0] = in >> 0 & 0xFF;
            pMem[addr + 1] = in >> 8 & 0xFF;
        }
        else /* if constexpr(std::endian::native == std::endian::big) */ {
            /* On big endian we go from higher to lower. */
            pMem[addr + 0] = in >> 8 & 0xFF;
            pMem[addr + 1] = in >> 0 & 0xFF;
        }
        return ResultSuccess();
    }

    constexpr Result WriteWord(Word in, Address addr) {
        if constexpr(std::endian::native == std::endian::little) {
            /* On little endian we go from lower to higher. */
            pMem[addr + 0] = in >>  0 & 0xFF;
            pMem[addr + 1] = in >>  8 & 0xFF;
            pMem[addr + 2] = in >> 16 & 0xFF;
            pMem[addr + 3] = in >> 24 & 0xFF;
        }
        else /* if constexpr(std::endian::native == std::endian::big) */ {
            /* On big endian we go from higher to lower. */
            pMem[addr + 0] = in >> 24 & 0xFF;
            pMem[addr + 1] = in >> 16 & 0xFF;
            pMem[addr + 2] = in >>  8 & 0xFF;
            pMem[addr + 3] = in >>  0 & 0xFF;
        }
        return ResultSuccess();
    }

    constexpr Result WriteDWord(DWord in, Address addr) {
        if constexpr(std::endian::native == std::endian::little) {
            /* On little endian we go from lower to higher. */
            pMem[addr + 0] = in >>  0 & 0xFF;
            pMem[addr + 1] = in >>  8 & 0xFF;
            pMem[addr + 2] = in >> 16 & 0xFF;
            pMem[addr + 3] = in >> 24 & 0xFF;
            pMem[addr + 4] = in >> 32 & 0xFF;
            pMem[addr + 5] = in >> 40 & 0xFF;
            pMem[addr + 6] = in >> 48 & 0xFF;
            pMem[addr + 7] = in >> 56 & 0xFF;
        }
        else /* if constexpr(std::endian::native == std::endian::big) */ {
            /* On big endian we go from higher to lower. */
            pMem[addr + 0] = in >> 56 & 0xFF;
            pMem[addr + 1] = in >> 48 & 0xFF;
            pMem[addr + 2] = in >> 40 & 0xFF;
            pMem[addr + 3] = in >> 32 & 0xFF;
            pMem[addr + 4] = in >> 24 & 0xFF;
            pMem[addr + 5] = in >> 16 & 0xFF;
            pMem[addr + 6] = in >>  8 & 0xFF;
            pMem[addr + 7] = in >>  0 & 0xFF;
        }
        return ResultSuccess();
    }
private:
    std::unique_ptr<Byte[]> pMem;
}; // class MemRegion

} // namespace detail
} // namespace mem
} // namespace riscv
