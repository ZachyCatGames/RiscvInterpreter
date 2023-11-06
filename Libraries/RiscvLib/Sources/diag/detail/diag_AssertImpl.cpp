#include <RiscvEmu/diag/detail/diag_AssertImpl.h>
#include <cstdarg>

namespace riscv {
namespace diag {
namespace detail {

namespace {

void PrintGenericMessage(FILE* stream, const std::source_location& location) {
    std::fprintf(stream, "[ASSERTION FAILURE]: %s; %s:%d:%d\n", location.function_name(), location.file_name(), location.line(), location.column());
}

} // namespace

void AssertNoMessageImpl(FILE* stream, bool cond, const std::source_location& location) {
    if(!cond) {
        PrintGenericMessage(stream, location);
        std::abort();
    }
}

void AssertWithMessageImpl(FILE* stream, bool cond, const std::source_location& location, std::string_view format, ...) {
    if(!cond) {
        va_list lst;
        va_start(lst, format);

        PrintGenericMessage(stream, location);
        std::fprintf(stream, "Message: ");
        std::vfprintf(stream, format.data(), lst);
    }
}

} // namespace detail
} // namespace diag
} // namespace riscv
