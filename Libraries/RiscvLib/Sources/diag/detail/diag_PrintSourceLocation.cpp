#include <RiscvEmu/diag/detail/diag_PrintSourceLocation.h>

namespace riscv {
namespace diag {
namespace detail {

void PrintSourceLocation(FILE* stream, std::string_view logType, const std::source_location& location) {
    std::fprintf(stream, "[%s]: %s; %s:%d:%d", logType.data(), location.function_name(), location.file_name(), location.line(), location.column());
}

void PrintMessageWithSourceLocation(FILE* stream, std::string_view logType, const std::source_location& location, std::string_view format, va_list formatList) {
    PrintSourceLocation(stream, logType, location);
    std::fprintf(stream, "\n  Message: ");
    std::vfprintf(stream, format.data(), formatList);
}

} // namespace detail
} // namespace diag
} // namespace riscv
