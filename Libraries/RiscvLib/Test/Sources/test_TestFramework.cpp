#include <RiscvEmuTest/test_TestFramework.h>

namespace riscv {
namespace test {
namespace detail {

void TestFrameworkBase::LogResetFailed(std::size_t i) {
    std::cout << "    Reset failed during test " << i << "!" << std::endl;
}

void TestFrameworkBase::LogTestResult(std::size_t i, Result res) {
    std::cout << "    Test " << i << " finished with Result = " << res.GetValue() << std::endl;
}

} // namespace detail
} // namespace test
} // namespace riscv
