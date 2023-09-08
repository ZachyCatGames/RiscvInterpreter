#include <RiscvEmuTest/test_TestFramework.h>

namespace riscv {
namespace test {
namespace detail {

void TestFrameworkBase::LogResetFailed(std::string_view name) {
    std::cout << "    Reset failed during test " << name << "!" << std::endl;
}

void TestFrameworkBase::LogTestResult(std::string_view name, Result res) {
    std::cout << "    Test " << name << " finished with Result = " << res.GetValue() << std::endl;
}

} // namespace detail
} // namespace test
} // namespace riscv
