#include <RiscvEmu/result.h>
#include <RiscvEmuTest/test_Args.h>
#include <string>
#include <span>
#include <ranges>
#include <iostream>

namespace riscv {
namespace test {

Result Main(Args args) __attribute__((weak));

} // namespace test
} // namespace riscv

int main(int argc, const char* argv[]) {
    riscv::test::Args args(argc, argv);

    /* Make sure the test defines Main. */
    if(!riscv::test::Main) {
        std::cerr << "Program " << args[0] << " does not define Main!" << std::endl;
        return -1;
    }

    /* Run main. */
    riscv::Result res = riscv::test::Main(args);
    std::cerr << "Program " << args[0] << " finished with Result = " << res.GetValue() << std::endl;
}
