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
        auto inst = cpu::RTypeInstruction(cpu::EncodeRTypeInstruction(static_cast<cpu::Opcode>(i), 
                                                                      cpu::Funct3::Max, 
                                                                      cpu::Funct7::Max, 
                                                                      cpu::InstMaxRegister, 
                                                                      cpu::InstMaxRegister, 
                                                                      cpu::InstMaxRegister));

        /* Test that rd is what we expect. */
        if(static_cast<Word>(inst.opcode()) != i) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

constexpr int TestFieldFunct3() noexcept {
    for(Word i = static_cast<Word>(cpu::Funct3::Min); i < static_cast<Word>(cpu::Funct3::Max); i++) {
        /* Encode instruction with all fields max'd */
        auto inst = cpu::RTypeInstruction(cpu::EncodeRTypeInstruction(cpu::Opcode::Max, 
                                                                      static_cast<cpu::Funct3>(i), 
                                                                      cpu::Funct7::Max, 
                                                                      cpu::InstMaxRegister, 
                                                                      cpu::InstMaxRegister, 
                                                                      cpu::InstMaxRegister));

        /* Test that rd is what we expect. */
        if(static_cast<Word>(inst.funct3()) != i) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

constexpr int TestFieldFunct7() noexcept {
    for(Word i = static_cast<Word>(cpu::Funct7::Min); i < static_cast<Word>(cpu::Funct7::Max); i++) {
        /* Encode instruction with all fields max'd */
        auto inst = cpu::RTypeInstruction(cpu::EncodeRTypeInstruction(cpu::Opcode::Max, 
                                                                      cpu::Funct3::Max, 
                                                                      static_cast<cpu::Funct7>(i), 
                                                                      cpu::InstMaxRegister, 
                                                                      cpu::InstMaxRegister, 
                                                                      cpu::InstMaxRegister));

        /* Test that rd is what we expect. */
        if(static_cast<Word>(inst.funct7()) != i) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

constexpr int TestFieldRd() noexcept {
    for(Word i = 0; i < 31; i++) {
        /* Encode instruction with all fields max'd */
        auto inst = cpu::RTypeInstruction(cpu::EncodeRTypeInstruction(cpu::Opcode::Max, 
                                                                      cpu::Funct3::Max, 
                                                                      cpu::Funct7::Max, 
                                                                      static_cast<int>(i), 
                                                                      cpu::InstMaxRegister, 
                                                                      cpu::InstMaxRegister));

        /* Test that rd is what we expect. */
        if(inst.rd() != i) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

constexpr int TestFieldRs1() noexcept {
    for(Word i = 0; i < 31; i++) {
        /* Encode instruction with all fields max'd */
        auto inst = cpu::RTypeInstruction(cpu::EncodeRTypeInstruction(cpu::Opcode::Max, 
                                                                      cpu::Funct3::Max, 
                                                                      cpu::Funct7::Max, 
                                                                      cpu::InstMaxRegister, 
                                                                      static_cast<int>(i), 
                                                                      cpu::InstMaxRegister));

        /* Test that rd is what we expect. */
        if(inst.rs1() != i) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

constexpr int TestFieldRs2() noexcept {
    for(Word i = 0; i < 31; i++) {
        /* Encode instruction with all fields max'd */
        auto inst = cpu::RTypeInstruction(cpu::EncodeRTypeInstruction(cpu::Opcode::Max, 
                                                                      cpu::Funct3::Max, 
                                                                      cpu::Funct7::Max, 
                                                                      cpu::InstMaxRegister, 
                                                                      cpu::InstMaxRegister, 
                                                                      static_cast<int>(i)));

        /* Test that rd is what we expect. */
        if(inst.rs2() != i) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

} // namespace

static_assert(TestFieldOpcode() == -1);
static_assert(TestFieldFunct3() == -1);
static_assert(TestFieldFunct7() == -1);
static_assert(TestFieldRd()     == -1);
static_assert(TestFieldRs1()    == -1);
static_assert(TestFieldRs2()    == -1);

extern constexpr std::string_view ProgramName = "CpuTestRTypeDecoding";

Result Main([[maybe_unused]] Args args) {
    return ResultSuccess();
}

} // namespace test
} // namespace riscv
