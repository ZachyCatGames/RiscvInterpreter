#include <RiscvEmu/mem/mem_MemoryDevice.h>

namespace riscv {
namespace mem {

Result MemoryDevice::LoadByte  (Byte* pOut, Address addr) { return m_Dev.LoadImpl<Byte>(pOut, addr); }
Result MemoryDevice::LoadHWord (HWord* pOut, Address addr) { return m_Dev.LoadImpl<HWord>(pOut, addr); }
Result MemoryDevice::LoadWord  (Word* pOut, Address addr) { return m_Dev.LoadImpl<Word>(pOut, addr); }
Result MemoryDevice::LoadDWord (DWord* pOut, Address addr) { return m_Dev.LoadImpl<DWord>(pOut, addr); }

Result MemoryDevice::StoreByte (Byte in, Address addr) { return m_Dev.StoreImpl<Byte>(in, addr); }
Result MemoryDevice::StoreHWord(HWord in, Address addr) { return m_Dev.StoreImpl<HWord>(in, addr); }
Result MemoryDevice::StoreWord (Word in, Address addr) { return m_Dev.StoreImpl<Word>(in, addr); }
Result MemoryDevice::StoreDWord(DWord in, Address addr) { return m_Dev.StoreImpl<DWord>(in, addr); }

} // namespace mem
} // namespace riscv
