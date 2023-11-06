#include <RiscvEmu/diag/detail/diag_DebugLogImpl.h>
#include <RiscvEmu/diag/detail/diag_PrintSourceLocation.h>
#include <cstdarg>

namespace riscv {
namespace diag {
namespace detail {

void DebugPrintImpl(FILE* stream, const std::source_location& location, std::string_view format, ...) {
    va_list lst;
    va_start(lst, format);

    PrintMessageWithSourceLocation(stream, "DEBUG LOG", location, format, lst);
}

} // namespace detail
} // namespace diag
} // namespace riscv
