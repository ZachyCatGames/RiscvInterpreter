#include <RiscvEmuTest/test_Common.h>
#include <RiscvEmu/cpu/cpu_EncodeInstruction.h>
#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/cpu/cpu_InstructionFormat.h>
#include <limits>

namespace riscv {
namespace test {

namespace {

constexpr int TestFieldOpcode() noexcept {
    for(int i = static_cast<int>(cpu::Opcode::Min); i < static_cast<int>(cpu::Opcode::Max); i++) {
        /* Encode instruction with all fields max'd */
        auto inst = cpu::JTypeInstruction(cpu::EncodeJTypeInstruction(static_cast<cpu::Opcode>(i), 
                                                                      cpu::InstMaxRegister, 
                                                                      std::numeric_limits<Word>::max()));

        /* Test that rd is what we expect. */
        if(static_cast<int>(inst.opcode()) != i) {
            return i;
        }
    }

    return -1;
}

constexpr int TestFieldRd() noexcept {
    for(int i = 0; i < cpu::InstMaxRegister; i++) {
        /* Encode instruction with all fields max'd */
        auto inst = cpu::JTypeInstruction(cpu::EncodeJTypeInstruction(cpu::Opcode::Max, 
                                                                      i, 
                                                                      std::numeric_limits<Word>::max()));

        /* Test that rd is what we expect. */
        if(inst.rd() != i) {
            return i;
        }
    }

    return -1;
}

constexpr int TestFieldImm() noexcept {
    for(int i = 12; i < 13; i++) {
        auto inst = cpu::JTypeInstruction(cpu::EncodeJTypeInstruction(cpu::Opcode::Max,
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

TestResults Main([[maybe_unused]] Args args) {
    return {};
}

} // namespace test
} // namespace riscv
