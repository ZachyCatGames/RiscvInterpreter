#pragma once
#include <RiscvEmu/result.h>
#include <RiscvEmu/cpu/cpu_Hart.h>
#include <RiscvEmuTest/test_HartTestCase.h>
#include <iostream>
#include <span>

namespace riscv {
namespace test {

namespace detail {

class TestFrameworkBase {
public:
    static void LogResetFailed(std::string_view name);
    static void LogTestResult(std::string_view name, Result res);
}; // class TestFrameworkBase

} // namespace detail

template<typename SysT, typename... TestsT>
class TestFramework : private detail::TestFrameworkBase {
public:
    using ResetType = Result(*)(SysT*);
    constexpr TestFramework(ResetType reset, const std::tuple<TestsT...>& tests) :
        m_ResetFunc(reset),
        m_Tests(tests) {}

    constexpr Result RunAll(SysT* pSys) const {
        /* Start running tests at test 0. */
        return this->RunAllImpl<0>(pSys);
    }
private:
    template<std::size_t I>
    constexpr Result RunImpl(SysT* pSys) const {
        const auto& test = std::get<I>(m_Tests);

        /* Run reset function. */
        Result res = m_ResetFunc(pSys);
        if(res.IsFailure()) {
            this->LogResetFailed(test.GetName());
            return res;
        }

        /* Run test & log its result. */
        res = test.Run(pSys);
        if(!std::is_constant_evaluated()) {
            this->LogTestResult(test.GetName(), res);
        }

        return ResultSuccess();
    }

    template<std::size_t I>
    constexpr Result RunAllImpl(SysT* pSys) const {
        /* Run test. */
        Result res = this->RunImpl<I>(pSys);
        if(res.IsFailure()) {
            return res;
        }

        /* Run next test. */
        if constexpr(sizeof...(TestsT) > I + 1) {
            return this->RunAllImpl<I + 1>(pSys);
        }

        return ResultSuccess();
    }
private:
    ResetType m_ResetFunc;
    std::tuple<TestsT...> m_Tests;
}; // class TestFramework

} // namespace test
} // namespace riscv
