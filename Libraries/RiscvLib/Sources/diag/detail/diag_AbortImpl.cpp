#include <RiscvEmu/diag/detail/diag_AbortImpl.h>
#include <RiscvEmu/diag/detail/diag_PrintSourceLocation.h>
#include <cstdlib>
#include <cstdarg>

namespace riscv {
namespace diag {
namespace detail {

void AbortImpl() {
    std::abort();
}

void AbortNoMessageImpl(FILE* stream, const std::source_location& location) {
    PrintSourceLocation(stream, "ABORT", location);
    AbortImpl();
}

void AbortWithMessageImpl(FILE* stream, const std::source_location& location, std::string_view format, ...) {
    va_list lst;
    va_start(lst, format);

    PrintMessageWithSourceLocation(stream, "ABORT", location, format, lst);

    AbortImpl();
}

void UnexpectedDefaultNoMessageImpl(FILE* stream, const std::source_location location) {
    PrintSourceLocation(stream, "UNEXPECTED DEFAULT", location);
    AbortImpl();
}

void UnexpectedDefaultWithMessageImpl(FILE* stream, const std::source_location& location, std::string_view format, ...){
    va_list lst;
    va_start(lst, format);

    PrintMessageWithSourceLocation(stream, "UNEXPECTED DEFAULT", location, format, lst);

    AbortImpl();
}


} // namespace detail
} // namespace diag
} // namespace riscv
