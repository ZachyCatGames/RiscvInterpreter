#pragma once
#include <RiscvEmu/result.h>
#include <RiscvEmu/riscv_Types.h>
#include <bit>
#include <utility>
#include <memory>

namespace riscv {
namespace mem {
namespace detail {

class MemoryDeviceImpl {
public:
    using PtrType = std::unique_ptr<Byte[]>;

    constexpr MemoryDeviceImpl() noexcept = default;

    MemoryDeviceImpl(PtrType&& pMem);
    MemoryDeviceImpl(NativeWord size);

    Result Initialize(PtrType&& pMem);
    Result Initialize(NativeWord size);

    template<typename WordType>
    Result LoadImpl(WordType* pOut, Address addr);

    template<typename WordType>
    Result StoreImpl(WordType in, Address addr);
private:
    PtrType m_pMem;
}; // class MemoryDeviceImpl

} // namespace detail
} // namespace mem
} // namespace riscv
