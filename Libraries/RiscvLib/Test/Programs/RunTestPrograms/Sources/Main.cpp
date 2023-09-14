#include <RiscvEmu/cfg/cpu/cpu_Extensions.h>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include "CreateProcess.h"

namespace {

constexpr std::string_view g_DefaultLogPath = "/dev/null";

constexpr std::string_view g_DefaultResPath = "Resources";
constexpr std::string_view g_TestCommonPath = "TestCommon.txt";
constexpr std::string_view g_TestFor32Path  = "TestFor32.txt";
constexpr std::string_view g_TestFor64Path  = "TestFor64.txt";

int RunTestFile(std::string_view testFilePath, std::string_view logPath, bool suppressStdout) {
    /* Open test file. */
    std::ifstream testFile(testFilePath);

    if(testFile.fail()) {
        std::cerr << std::format("Failed to open test file: {} (errno = {})\n", testFilePath, errno);
        return -1;
    }

    std::string programPath;
    int pass = 0, fail = 0;
    while(true) {
        /* Read a line. */
        std::getline(testFile, programPath);
        if(testFile.bad()) {
            std::cerr << std::format("Failed to read test file: {} (errno = {})\n", testFilePath, errno);
            return -1;
        }

        /* Check if we've reached EOF. */
        if(testFile.eof()) {
            break;
        }

        /* Run the program. */
        if(RunProgram(programPath, logPath, suppressStdout) == 0) {
            pass++;
        }
        else {
            fail++;
        }
    }

    std::cout << std::format("{}:\n"
                             "    {} Tests Passed\n"
                             "    {} Tests Failed.\n\n",
                             testFilePath, pass, fail);

    return 0;
}

} // namespace

int main(int argc, char** argv) {
    std::string_view resourcePath = g_DefaultResPath;
    std::string_view logPath = g_DefaultLogPath;
    bool suppressStdout = false;

    for(auto i = 1; i < argc; i++) {
        std::string_view curArg = argv[i];

        if(!curArg.compare("-help")) {
            std::cout << std::format("{} [options] \n"
                                     "Options:\n"
                                     "  -help: Print this dialogue.\n"
                                     "  -res [dir]: Load resources from dir.\n"
                                     "  -log [dir]: Saves test logs to dir.\n"
                                     "  -suppress: Suppress test output if -log is specified.",
                                     argv[0]);
            return 0;
        }

        if(!curArg.compare("-res")) {
            /* Use next argument as resource path. */
            resourcePath = argv[i + 1];
            i += 2;
        }

        if(!curArg.compare("-log")) {
            /* Use next argument as log path. */
            logPath = argv[i + 1];
            i += 2;
        }

        if(!curArg.compare("-suppress")) {
            suppressStdout = true;
        }
    }

    /* Run common tests. */
    RunTestFile(std::format("{}/{}", resourcePath, g_TestCommonPath), logPath, suppressStdout);

    /* Run 32/64 exclusive tests. */
    if constexpr(!riscv::cfg::cpu::EnableIsaRV64I) {
        RunTestFile(std::format("{}/{}", resourcePath, g_TestFor32Path), logPath, suppressStdout);
    }
    else {
        RunTestFile(std::format("{}/{}", resourcePath, g_TestFor64Path), logPath, suppressStdout);
    }

}