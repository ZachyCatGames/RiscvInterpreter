#pragma once
#include <RiscvEmu/result.h>
#include <string_view>

namespace riscv {
namespace test {

template<typename Derived, typename SysT>
class TestCaseBase {
public:
    constexpr Result Run(SysT* pSys) const {
        return static_cast<const Derived*>(this)->RunImpl(pSys);
    }

    constexpr std::string_view GetName() const noexcept { return m_Name; }
protected:
    constexpr TestCaseBase(std::string_view name) noexcept :
        m_Name(name) {}
private:
    std::string_view m_Name;
}; // class TestCaseBase

} // namespace test
} // namespace riscv
