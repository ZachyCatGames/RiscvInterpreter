#include <RiscvEmu/riscv_System.h>
#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/hw/hw_Scheduler.h>
#include <RiscvEmu/mem/mem_MemoryDevice.h>


int main(int argc, const char* argv[]) {
    riscv::System sys;

    sys.AddPeripheral(std::make_unique<riscv::mem::MemoryDevice>(0x1000));
}
