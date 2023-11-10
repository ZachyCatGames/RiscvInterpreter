#include <RiscvEmu/cpu/detail/cpu_MemoryMonitor.h>
#include <RiscvEmu/diag.h>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <thread>

using namespace riscv;

namespace {

void SharedThread(Word index, cpu::detail::MemoryMonitor* pMonitor, DWord roundCount, Address maxAddr) {
    auto ctx = pMonitor->GetContext(index);
    Address addr = 0x1000;

    for(DWord i = 0; i < roundCount; i++) {
        if(maxAddr) {
            addr = static_cast<Address>(rand()) % (maxAddr + 1);
        }
        ctx.AquireSharedAccess(addr);
        ctx.ReleaseSharedAccess();
    }
}

void ExclusiveThread(Word index, cpu::detail::MemoryMonitor* pMonitor, DWord roundCount, Address maxAddr) {
    auto ctx = pMonitor->GetContext(index);
    Address addr = 0x1000;

    for(DWord i = 0; i < roundCount; i++) {
        if(maxAddr) {
            addr = static_cast<Address>(rand()) % (maxAddr + 1);
        }
        ctx.AquireExclusiveAccess(addr);
        ctx.ReleaseExclusiveAccess();
    }
}

} // namespace

int main(int argc, char** argv) {
    if(argc != 3) {
        std::cout << std::format("Usage: {} [shared_count] [exclusive_count] [round_count] [mode=random,fixed] [max_addr]", argv[0]);
    }

    /* Parse arguments. */
    Word sharedCount    = static_cast<Word>(strtol(argv[1], nullptr, 10));
    Word exclusiveCount = static_cast<Word>(strtol(argv[2], nullptr, 10));
    DWord roundCount = static_cast<DWord>(strtol(argv[3], nullptr, 10));
    Address maxAddr = 0;

    if(!strcmp(argv[4], "random")) {
        maxAddr = static_cast<Address>(strtol(argv[5], nullptr, 10));
    }
    else if(!strcmp(argv[4], "fixed")){
        /* ... */
    }
    else {
        std::cout << "Unrecognized Mode: " << argv[4] << std::endl;
    }

    /* Seed rng if we're in random mode. */
    if(maxAddr) {
        srand(static_cast<uint>(time(nullptr)));
    }

    /* Record current time. */
    auto start = std::chrono::high_resolution_clock::now();

    /* Initialize monitor. */
    cpu::detail::MemoryMonitor monitor;
    monitor.Initialize(sharedCount + exclusiveCount);

    /* Start shared threads. */
    auto pSharedThreads = std::make_unique<std::thread[]>(sharedCount);
    for(Word i = 0; i < sharedCount; i++) {
        pSharedThreads[i] = std::thread(SharedThread, i, &monitor, roundCount, maxAddr);
    }

    /* Start exclusive threads. */
    auto pExclusiveThreads = std::make_unique<std::thread[]>(exclusiveCount);
    for(Word i = 0; i < exclusiveCount; i++) {
        pExclusiveThreads[i] = std::thread(ExclusiveThread, sharedCount + i, &monitor, roundCount, maxAddr);
    }

    /* Join shared threads. */
    for(Word i = 0; i < sharedCount; i++) {
        pSharedThreads[i].join();
    }

    /* Join exclusive threads. */
    for(Word i = 0; i < exclusiveCount; i++) {
        pExclusiveThreads[i].join();
    }

    /* Print time taken. */
    std::cout << "Time Taken: " << std::chrono::high_resolution_clock::now() - start << std::endl;
}
