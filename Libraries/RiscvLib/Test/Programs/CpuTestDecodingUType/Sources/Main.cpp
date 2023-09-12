#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmu/cpu/cpu_EncodeInstruction.h>
#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/cpu/decoder/cpu_InstructionFormat.h>
#include <limits>

namespace riscv {
namespace test {

namespace {

constexpr int TestFieldOpcode() noexcept {
    for(Word i = static_cast<Word>(cpu::Opcode::Min); i < static_cast<Word>(cpu::Opcode::Max); i++) {
        /* Encode instruction with all fields max'd */
        auto inst = cpu::UTypeInstruction(cpu::EncodeUTypeInstruction(static_cast<cpu::Opcode>(i), 
                                                                      cpu::InstMaxRegister, 
                                                                      std::numeric_limits<Word>::max()));

        /* Test that rd is what we expect. */
        if(static_cast<Word>(inst.opcode()) != i) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

constexpr int TestFieldRd() noexcept {
    for(Word i = 0; i < cpu::InstMaxRegister; i++) {
        /* Encode instruction with all fields max'd */
        auto inst = cpu::UTypeInstruction(cpu::EncodeUTypeInstruction(cpu::Opcode::Max, 
                                                                      static_cast<int>(i), 
                                                                      std::numeric_limits<Word>::max()));

        /* Test that rd is what we expect. */
        if(inst.rd() != i) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

constexpr int TestFieldImm() noexcept {
    for(int i = 12; i < 32; i++) {
        auto inst = cpu::UTypeInstruction(cpu::EncodeUTypeInstruction(cpu::Opcode::Max,
                                                                      cpu::InstMaxRegister,
                                                                      1u << i));

        if(inst.imm() != 1u << i) {
            return i;
        }
    }

    return -1;
}

} // namespace

static_assert(TestFieldOpcode() == -1);
static_assert(TestFieldRd()     == -1);
static_assert(TestFieldImm()    == -1);

extern constexpr std::string_view ProgramName = "CpuTestDecodingUType";

Result Main([[maybe_unused]] Args args) {
    return ResultSuccess();
}

} // namespace test
} // namespace riscv
