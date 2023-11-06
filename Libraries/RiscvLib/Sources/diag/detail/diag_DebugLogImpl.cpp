#include <RiscvEmu/diag/detail/diag_DebugLogImpl.h>
#include <cstdarg>

namespace riscv {
namespace diag {
namespace detail {

void DebugPrintImpl(FILE* stream, const std::source_location& location, std::string_view format, ...) {
    va_list lst;
    va_start(lst, format);

    std::fprintf(stream, "[DEBUG LOG]: %s; %s:%d:%d\n  Message: ", location.function_name(), location.file_name(), location.line(), location.column());
    std::vfprintf(stream, format.data(), lst);
}

} // namespace detail
} // namespace diag
} // namespace riscv
