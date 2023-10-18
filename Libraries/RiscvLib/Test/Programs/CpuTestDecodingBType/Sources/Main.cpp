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
        auto inst = cpu::BTypeInstruction(cpu::EncodeBTypeInstruction(static_cast<cpu::Opcode>(i), 
                                                                      cpu::Funct3::Max, 
                                                                      cpu::InstMaxRegister, 
                                                                      cpu::InstMaxRegister, 
                                                                      std::numeric_limits<Word>::max()));

        /* Test that rd is what we expect. */
        if(static_cast<int>(inst.opcode()) != i) {
            return i;
        }
    }

    return -1;
}

constexpr int TestFieldFunct3() noexcept {
    for(int i = static_cast<int>(cpu::Funct3::Min); i < static_cast<int>(cpu::Funct3::Max); i++) {
        /* Encode instruction with all fields max'd */
        auto inst = cpu::BTypeInstruction(cpu::EncodeBTypeInstruction(cpu::Opcode::Max, 
                                                                      static_cast<cpu::Funct3>(i), 
                                                                      cpu::InstMaxRegister, 
                                                                      cpu::InstMaxRegister, 
                                                                      std::numeric_limits<Word>::max()));

        /* Test that rd is what we expect. */
        if(static_cast<int>(inst.funct3()) != i) {
            return i;
        }
    }

    return -1;
}

constexpr int TestFieldRs1() noexcept {
    for(int i = 0; i < cpu::InstMaxRegister; i++) {
        /* Encode instruction with all fields max'd */
        auto inst = cpu::BTypeInstruction(cpu::EncodeBTypeInstruction(cpu::Opcode::Max, 
                                                                      cpu::Funct3::Max, 
                                                                      i, 
                                                                      cpu::InstMaxRegister, 
                                                                      std::numeric_limits<Word>::max()));

        /* Test that rd is what we expect. */
        if(inst.rs1() != i) {
            return i;
        }
    }

    return -1;
}

constexpr int TestFieldRs2() noexcept {
    for(int i = 0; i < cpu::InstMaxRegister; i++) {
        /* Encode instruction with all fields max'd */
        auto inst = cpu::BTypeInstruction(cpu::EncodeBTypeInstruction(cpu::Opcode::Max, 
                                                                      cpu::Funct3::Max, 
                                                                      cpu::InstMaxRegister, 
                                                                      i, 
                                                                      std::numeric_limits<Word>::max()));

        /* Test that rd is what we expect. */
        if(inst.rs2() != i) {
            return i;
        }
    }

    return -1;
}

constexpr int TestFieldImm() noexcept {
    for(int i = 1; i < 13; i++) {
        auto inst = cpu::BTypeInstruction(cpu::EncodeBTypeInstruction(cpu::Opcode::Max,
                                                                      cpu::Funct3::Max,
                                                                      cpu::InstMaxRegister,
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
static_assert(TestFieldFunct3() == -1);
static_assert(TestFieldRs1()    == -1);
static_assert(TestFieldRs2()    == -1);
static_assert(TestFieldImm()    == -1);

TestResults Main([[maybe_unused]] Args args) {
    return {};
}

} // namespace test
} // namespace riscv
