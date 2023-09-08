#include <RiscvEmuTest/test_TestFramework.h>

namespace riscv {
namespace test {
namespace detail {

void TestFrameworkBase::LogResetFailed(std::string_view name, Result res) {
    std::cout << "    Reset failed during test [Result = " << res.GetValue() << "]: " << name << std::endl;
}

void TestFrameworkBase::LogTestResult(std::string_view name, Result res) {
    std::cout << "    Test finished [Result = " << res.GetValue() << "]: " << name << std::endl;
}

} // namespace detail
} // namespace test
} // namespace riscv
