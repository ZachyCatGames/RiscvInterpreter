#include <RiscvEmu/riscv_System.h>
#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/hw/hw_Scheduler.h>
#include <RiscvEmu/mem/mem_MemoryDevice.h>
#include <RiscvEmu/cpu/detail/cpu_IntegerMultiply.h>
#include <iostream>

int main() {
    uint64_t out[2]{0};

    uint64_t a  = riscv::cpu::detail::Multiply64U(out, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);

    std::cout << out[0] << std::endl;
    std::cout << out[1] << std::endl;
    std::cout << a << std::endl;

    riscv::System sys;

    sys.AddPeripheral(std::make_unique<riscv::mem::MemoryDevice>(0x1000));
}
