#pragma once
#include <string_view>

namespace riscv {
namespace test {

class Args {
public:
    constexpr Args(int argc, const char** argv) noexcept :
        m_Argc(argc),
        m_Argv(argv) {}

    constexpr auto size() const noexcept { return m_Argc; }
    constexpr std::string_view operator[](std::size_t offs) const noexcept {
        return std::string_view(m_Argv[offs]);
    }
private:
    int m_Argc;
    const char ** m_Argv;
}; // class Args

} // namespace test
} // namespace riscv
