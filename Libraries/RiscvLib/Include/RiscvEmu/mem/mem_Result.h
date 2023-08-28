#pragma once
#include <RiscvEmu/result.h>

namespace riscv {
namespace mem {

namespace detail {

constexpr inline Word ModuleId = 2;

} // namespace detail

class ResultBadMisalignedAddress : public result::ErrorBase<detail::ModuleId, 1> {};

class ResultBadAddress : public result::ErrorBase<detail::ModuleId, 2> {};

class ResultRegionAlreadyExists : public result::ErrorBase<detail::ModuleId, 3> {};

class ResultRegionDoesNotExist : public result::ErrorBase<detail::ModuleId, 4> {};

class ResultInvalidRegionType : public result::ErrorBase<detail::ModuleId, 5> {};

class ResultDeviceAlreadyExists : public result::ErrorBase<detail::ModuleId, 6> {};

class ResultDeviceDoesNotExist : public result::ErrorBase<detail::ModuleId, 7> {};

} // namespace mem
} // namespace riscv
