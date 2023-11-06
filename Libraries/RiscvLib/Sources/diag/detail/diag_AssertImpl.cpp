#include <RiscvEmu/diag/detail/diag_AbortImpl.h>
#include <RiscvEmu/diag/detail/diag_AssertImpl.h>
#include <RiscvEmu/diag/detail/diag_PrintSourceLocation.h>
#include <cstdarg>

namespace riscv {
namespace diag {
namespace detail {

void AssertNoMessageImpl(FILE* stream, const std::source_location& location) {
    PrintSourceLocation(stream, "ASSERTION FAILURE", location);
    AbortImpl();
}

void AssertWithMessageImpl(FILE* stream, const std::source_location& location, std::string_view format, ...) {
    va_list lst;
    va_start(lst, format);

    PrintMessageWithSourceLocation(stream, "ASSERTION FAILURE", location, format, lst);
    AbortImpl();
}

} // namespace detail
} // namespace diag
} // namespace riscv
