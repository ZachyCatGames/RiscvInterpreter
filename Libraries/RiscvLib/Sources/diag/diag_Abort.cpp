#include <RiscvEmu/diag/diag_Abort.h>

namespace riscv {
namespace diag {

[[noreturn]] void Abort(const std::source_location& location) {
    detail::AbortNoMessageImpl(stderr, location);
}

[[noreturn]] void UnexpectedDefault(const std::source_location& location) {
    detail::UnexpectedDefaultNoMessageImpl(stderr, location);
}

} // namespace diag
} // namespace riscv
