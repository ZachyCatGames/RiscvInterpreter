#include <RiscvEmu/riscv_System.h>
#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmu/hw/hw_Scheduler.h>
#include <RiscvEmu/mem/mem_MemoryDevice.h>
#include <RiscvEmu/cpu/detail/cpu_IntegerMultiply.h>
#include <RiscvEmu/cpu/detail/cpu_MemoryMonitor.h>
#include <RiscvEmu/diag.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <span>

void SharedThread(riscv::Word index, riscv::cpu::detail::MemoryMonitor* pMonitor) {
    for(int i = 0; i < rand() % 100; i++);

    auto ctx = pMonitor->GetContext(index);

    for(int i = 0; i < 100000; i++) {
        ctx.AquireSharedAccess(0x1000u);
        ctx.ReleaseSharedAccess();
    }
}

void TestMemMonitor() {
    constexpr size_t count = 8;
    std::thread threads[count];
    riscv::cpu::detail::MemoryMonitor monitor;
    monitor.Initialize(count);

    auto start = std::chrono::high_resolution_clock::now();

    for(riscv::Word i = 0; i < count; i++) {
        threads[i] = std::thread(SharedThread, i, &monitor);
    }

    for(riscv::Word i = 0; i < count; i++) {
        threads[i].join();
    }

    std::cout << std::chrono::high_resolution_clock::now() - start << std::endl;
}

int main() {
    uint64_t out[2]{0};

    TestMemMonitor();
    return 0;
    uint64_t a  = riscv::cpu::detail::Multiply64U(out, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);

    std::cout << out[0] << std::endl;
    std::cout << out[1] << std::endl;
    std::cout << a << std::endl;

    riscv::diag::Assert(0);
    riscv::diag::Assert(0, "hi\n");

    riscv::System sys;

    sys.AddPeripheral(std::make_unique<riscv::mem::MemoryDevice>(0x1000));
}
