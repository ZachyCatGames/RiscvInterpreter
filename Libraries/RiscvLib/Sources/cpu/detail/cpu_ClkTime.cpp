#include <RiscvEmu/cpu/detail/cpu_ClkTime.h>
#include <time.h>

namespace riscv {
namespace cpu {
namespace detail {

namespace {

DWord GetCurrentTimeImpl() {
    timespec time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
    return static_cast<DWord>(time.tv_nsec);
}

} // namespace

Result ClkTime::Initialize() {
    /* Set offset to current time. */
    m_Offset = GetCurrentTimeImpl();

    return ResultSuccess();
}

Result ClkTime::GetCurrentTime(DWord* pOut) {
    /* Take difference between current time and offset. */
    *pOut = GetCurrentTimeImpl() - m_Offset;
    return ResultSuccess();
}

Result ClkTime::SetCurrentTime(DWord in) {
    /* Set offset to the difference between the current time and the requested time. */
    m_Offset = GetCurrentTimeImpl() - in;
    return ResultSuccess();
}

} // namespace detail
} // namespace cpu
} // namespace riscv