#include <RiscvEmu/diag/detail/diag_AbortImpl.h>
#include <RiscvEmu/diag/detail/diag_PrintSourceLocation.h>
#include <cstdlib>
#include <cstdarg>

namespace riscv {
namespace diag {
namespace detail {

[[noreturn]] void AbortImpl() {
    std::abort();
}

[[noreturn]] void AbortNoMessageImpl(FILE* stream, const std::source_location& location) {
    PrintSourceLocation(stream, "ABORT", location);
    AbortImpl();
}

[[noreturn]] void AbortWithMessageImpl(FILE* stream, const std::source_location& location, std::string_view format, ...) {
    va_list lst;
    va_start(lst, format);

    PrintMessageWithSourceLocation(stream, "ABORT", location, format, lst);

    AbortImpl();
}

[[noreturn]] void UnexpectedDefaultNoMessageImpl(FILE* stream, const std::source_location location) {
    PrintSourceLocation(stream, "UNEXPECTED DEFAULT", location);
    AbortImpl();
}

[[noreturn]] void UnexpectedDefaultWithMessageImpl(FILE* stream, const std::source_location& location, std::string_view format, ...){
    va_list lst;
    va_start(lst, format);

    PrintMessageWithSourceLocation(stream, "UNEXPECTED DEFAULT", location, format, lst);

    AbortImpl();
}


} // namespace detail
} // namespace diag
} // namespace riscv
