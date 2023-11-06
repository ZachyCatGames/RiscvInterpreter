#include <RiscvEmu/diag/detail/diag_PrintSourceLocation.h>
#include <mutex>

namespace riscv {
namespace diag {
namespace detail {

namespace {

std::mutex g_Mutex;

void PrintSourceLocationImpl(FILE* stream, std::string_view logType, const std::source_location& location) {
    std::fprintf(stream, "[%s]: %s; %s:%d:%d\n", logType.data(), location.function_name(), location.file_name(), location.line(), location.column());
}

} // namespace

void PrintSourceLocation(FILE* stream, std::string_view logType, const std::source_location& location) {
    std::scoped_lock lock(g_Mutex);
    PrintSourceLocationImpl(stream, logType, location);
}

void PrintMessageWithSourceLocation(FILE* stream, std::string_view logType, const std::source_location& location, std::string_view format, va_list formatList) {
    std::scoped_lock lock(g_Mutex);
    PrintSourceLocationImpl(stream, logType, location);
    std::fprintf(stream, "  Message: ");
    std::vfprintf(stream, format.data(), formatList);
}

} // namespace detail
} // namespace diag
} // namespace riscv
