#include <RiscvEmuTest/test_TestResults.h>
#include <RiscvEmu/result.h>
#include <RiscvEmuTest/test_Args.h>
#include <string>
#include <iostream>
#include <format>

namespace riscv {
namespace test {

extern TestResults Main(Args args);

} // namespace test
} // namespace riscv

int main(int argc, const char* argv[]) {
    /* Run main. */
    riscv::test::Args args(argc, argv);
    riscv::test::TestResults res = riscv::test::Main(args);

    /* Print our results. */
    std::cout << std::format("    Test Results:\n"
                             "        Pass: {}\n"
                             "        Fail: {}\n",
                             res.pass, res.fail);
    
    /* If any tests failed, indicate that. */
    if(res.fail) {
        std::cout << "    WARNING: Tests Failed!\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
