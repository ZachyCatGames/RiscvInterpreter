#pragma once
#include <RiscvEmu/riscv_Types.h>
#include <RiscvEmu/result.h>
#include <RiscvEmu/mem/mem_MemoryController.h>

namespace riscv {
namespace mem {

class MCClient {
public:
    /** Move constructor. */
    MCClient(MCClient&&) = default;

    /**
     * @brief Perform a load from memory.
     * 
     * This loads a WordType from memory.
     * 
     * WordType can be any of the following: 
     * Byte, HWord, Word, DWord, ByteS, HWordS, WordS, DWordS, float, or double.
     * 
     * Depending on the device being written to, addr may or may not be required to 
     * be aligned to some boundry.
     * 
     * This operation is not atomic.
     * 
     * @param pOut Location to store the loaded word.
     * @param addr Byte address of word to load.
     * @return Result code.
    */
    template<typename WordType>
    Result Load(WordType* pOut, Address addr) { return m_pCtlr->LoadImpl(pOut, addr); }

    /**
     * @brief Perform a store to memory.
     * 
     * This stores a WordType to memory.
     * 
     * WordType can be any of the following:
     * Byte, HWord, Word, DWord, ByteS, HWordS, WordS, DWordS, float, or double.
     * 
     * Depending on the device being written to, addr may or may not be required to 
     * be aligned to some boundry.
     * 
     * This operation is atomic.
     * 
     * @param in   Word to store to memory.
     * @param addr Byte address to store word at.
     * @return Result code.
    */
    template<typename WordType>
    Result Store(WordType in, Address addr) { return m_pCtlr->StoreImpl(in, addr); }

    /**
     * @brief Perform a load from memory and register a reservation.
     * 
     * This loads a WordType from memory.
     * 
     * WordType can be any of the following: 
     * Byte, HWord, Word, DWord, ByteS, HWordS, WordS, DWordS, float, or double.
     * 
     * After performing the load a reservation entry for addr will be created.
     * 
     * The provided address must be aligned to sizeof(WordType).
     * 
     * This operation is atomic.
     * 
     * @param pOut      Location to store loaded word.
     * @param addr      Byte address of word to load and add a registration for.
     * @return Result code.
    */
    template<typename WordType>
    Result LoadReserve(WordType* pOut, Address addr) { m_pCtlr->LoadReserve(this, pOut, addr); }

    /**
     * @brief Perform a conditional store based on reservation entries.
     * 
     * This stores a WordType to memory only if the specified client has a reservation 
     * for the provided address. If the store succeeds the provided word is overwritten
     * with 0, otherwise it is overwritten with 1.
     * 
     * WordType can be any of the following: 
     * Byte, HWord, Word, DWord, ByteS, HWordS, WordS, DWordS, float, or double.
     * 
     * The provided address must be aligned to sizeof(WordType).
     * 
     * This operation is atomic.
     * 
     * @param pInOut    Location containing the word to be written.
     * @param addr      Byte address to store word at.
     * @return Result code.
    */
    template<typename WordType>
    Result StoreConditional(WordType* pInOut, Address addr) { return m_pCtlr->StoreConditional(this, pInOut, addr); }
private:
    MCClient();
    MCClient(const MCClient&) = delete;
    MCClient(MemoryController* pCtlr);
private:
    friend class MemoryController;
    MemoryController* m_pCtlr;
}; // class MCClient

} // namespace mem
} // namespace riscv
