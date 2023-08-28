#pragma once
#include <RiscvEmu/riscv_Types.h>

namespace riscv {
namespace mem {

enum class RegionType {
    Memory,
    IO
}; // enum class RegionType

struct RegionInfo {
    NativeWord start;
    NativeWord length;
    RegionType type;
}; // struct RegionInfo

} // namespace mem
} // namespace riscv
