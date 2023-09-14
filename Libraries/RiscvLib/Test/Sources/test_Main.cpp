#include <RiscvEmu/result.h>
#include <RiscvEmuTest/test_Args.h>
#include <string>
#include <span>
#include <ranges>
#include <iostream>

namespace riscv {
namespace test {

extern const std::string_view ProgramName;

extern Result Main(Args args);

} // namespace test
} // namespace riscv

int main(int argc, const char* argv[]) {
    /* Announce that's we've started. */
    std::cerr << "Program " << riscv::test::ProgramName << " has started..." << std::endl;

    /* Run main. */
    riscv::test::Args args(argc, argv);
    riscv::Result res = riscv::test::Main(args);
    std::cerr << "Program " << riscv::test::ProgramName << " finished with Result = " << res.GetValue() << std::endl;

    return res.GetValue();
}
