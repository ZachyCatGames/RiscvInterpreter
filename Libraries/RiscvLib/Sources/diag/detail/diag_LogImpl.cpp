#include <RiscvEmu/diag/detail/diag_LogImpl.h>
#include <RiscvEmu/diag/detail/diag_PrintSourceLocation.h>
#include <cstdarg>

namespace riscv {
namespace diag {
namespace detail {

void LogImpl(FILE* stream, std::string_view format, ...) {
    va_list lst;
    va_start(lst, format);

    PrintMessage(stream, format, lst);
}

} // namespace detail
} // namespace diag
} // namespace riscv
