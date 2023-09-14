#pragma once
#include <string_view>

namespace riscv {
namespace test {

class Args {
public:
    class ForwardIterator {
    public:
        constexpr ForwardIterator& operator++() noexcept {
            ++m_Argv;
            return *this;
        }

        constexpr ForwardIterator operator++(int) noexcept {
            auto old = *this;
            ++m_Argv;
            return old;
        }

        constexpr ForwardIterator& operator--() noexcept {
            --m_Argv;
            return *this;
        }

        constexpr ForwardIterator operator--(int) noexcept {
            auto old = *this;
            --m_Argv;
            return old;
        }

        constexpr bool operator==(const ForwardIterator& rhs) const noexcept {
            return this->m_Argv == rhs.m_Argv;
        }

        constexpr std::string_view operator*() const noexcept { return *m_Argv; }
        constexpr const char** operator->() const noexcept { return m_Argv; }

        friend constexpr ForwardIterator operator+(const ForwardIterator& lhs, int rhs) noexcept {
            return ForwardIterator(lhs.m_Argv + rhs);
        }

        friend constexpr ForwardIterator operator+(int lhs, const ForwardIterator& rhs) noexcept {
            return ForwardIterator(rhs.m_Argv + lhs);
        }

        friend constexpr ForwardIterator operator-(const ForwardIterator& lhs, int rhs) noexcept {
            return ForwardIterator(lhs.m_Argv - rhs);
        }

        constexpr auto operator<=>(const ForwardIterator& rhs) noexcept {
            return this->m_Argv <=> rhs.m_Argv;
        }
    private:
        friend class Args;
        constexpr ForwardIterator(const char** argv) noexcept :
            m_Argv(argv) {}
    private:
        const char** m_Argv;
    };
public:
    using iterator = ForwardIterator;
    using const_iterator = ForwardIterator;
public:
    constexpr Args(int argc, const char** argv) noexcept :
        m_Argc(argc),
        m_Argv(argv) {}

    constexpr auto size() const noexcept { return m_Argc; }
    constexpr std::string_view operator[](std::size_t offs) const noexcept {
        return std::string_view(m_Argv[offs]);
    }

    constexpr iterator begin() const noexcept { return ForwardIterator(m_Argv); }
    constexpr iterator cbegin() const noexcept { return this->begin(); }

    constexpr iterator end() const noexcept { return ForwardIterator(m_Argv + m_Argc); }
    constexpr iterator cend() const noexcept { return this->end(); }

private:
    int m_Argc;
    const char ** m_Argv;
}; // class Args

} // namespace test
} // namespace riscv
