#pragma once
#include <RiscvEmu/result.h>

namespace riscv {
namespace cpu {

namespace detail {

constexpr inline Word ModuleId = 1;

} // namespace detail

class ResultInvalidInstruction : public result::ErrorBase<detail::ModuleId, 1> {};

class ResultNotImplemented : public result::ErrorBase<detail::ModuleId, 2> {};

/* Memory Access Errors. */
class ResultLoadAccessFault  : public result::ErrorBase<detail::ModuleId, 200> {};
class ResultStoreAccessFault : public result::ErrorBase<detail::ModuleId, 201> {};
class ResultFetchAccessFault : public result::ErrorBase<detail::ModuleId, 202> {};
class ResultLoadPageFault    : public result::ErrorBase<detail::ModuleId, 203> {};
class ResultStorePageFault   : public result::ErrorBase<detail::ModuleId, 204> {};
class ResultFetchPageFault   : public result::ErrorBase<detail::ModuleId, 205> {};

/* Internal translation error. */
class ResultInvalidTranslationMode  : public result::ErrorBase<detail::ModuleId, 210> {};
class ResultNoValidPteFound         : public result::ErrorBase<detail::ModuleId, 211> {};
class ResultCannotAccessMapFromPriv : public result::ErrorBase<detail::ModuleId, 212> {};

} // namespace cpu
} // namespace riscv
