#pragma once
#include <RiscvEmu/result.h>

namespace riscv {
namespace test {

template<typename Derived, typename SysT>
class TestCaseBase {
public:
    constexpr Result Run(SysT* pSys) const {
        return static_cast<const Derived*>(this)->RunImpl(pSys);
    }
}; // class TestCaseBase

} // namespace test
} // namespace riscv
