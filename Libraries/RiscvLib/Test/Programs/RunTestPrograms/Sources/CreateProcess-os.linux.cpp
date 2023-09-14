#include <iostream>
#include <format>
#include <string>
#include <vector>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int RunProgram(std::string_view programPath, std::string_view logPath, bool suppressStdout) {
    /* Create stdin pipe. */
    int stdinPipes[2];
    if(pipe2(stdinPipes, O_CLOEXEC) != 0) {
        std::cerr << std::format("Failed to create pipe for stdin (pipe errno = {})\n", errno);
        return -1;
    }

    /* Create pipe for stdout and stderr. */
    int stdoutPipes[2];
    if(pipe2(stdoutPipes, O_CLOEXEC) != 0) {
        std::cerr << std::format("Failed to create pipe for stdout (pipe errno = {})\n", errno);
        return -1;
    }

    /* Fork ourself. */
    int pid = fork();
    if(pid == 0) {
        /* Redirect stdin. */
        if(dup2(stdinPipes[0], STDIN_FILENO) == -1) {
            std::cerr << std::format("{}: Failed to redirect stdin (dup2 errno = {})\n", programPath, errno);
            return -1;
        }

        /* Redirect stdout. */
        if(dup2(stdoutPipes[1], STDOUT_FILENO) == -1) {
            std::cerr << std::format("{}: Failed to redirect stdout (dup2 errno = {})\n", programPath, errno);
            return -1;
        }

        /* Redirect stderr. */
        if(dup2(stdoutPipes[1], STDERR_FILENO) == -1) {
            std::cerr << std::format("{}: Failed to redirect stderr (dup2 errno = {})\n", programPath, errno);
            return -1;
        }

        /* Execute new program. */
        if(execlp(programPath.data(), programPath.data()) != 0) {
            std::cerr << std::format("{}: Failed to run (exec errno = {})\n", programPath, errno);
            return -1;
        }
        return 0;
    }

    /* Close pipes given to child. */
    close(stdinPipes[0]);
    close(stdoutPipes[1]);

    /* Open our log file. */
    int logFileFd = open(logPath.data(), O_RDWR | O_CREAT, 777);
    if(logFileFd == -1) {
        std::cerr << std::format("{}: Failed to open log file (open errno = {})\n", programPath, errno);
    }

    /* Move to EOF. */
    lseek(logFileFd, 0, SEEK_END);

    /* Read from output pipe until it's closed. */
    std::vector<char> charBuf(0x1000);
    ssize_t readSize = 0;
    while(true) {
        readSize = read(stdoutPipes[0], charBuf.data(), charBuf.size());
        if(readSize == 0) {
            break;
        }

        /* Write data to our log file. */
        write(logFileFd, charBuf.data(), static_cast<size_t>(readSize));

        /* Write data to our stdout. */
        if(!suppressStdout) {
            write(STDOUT_FILENO, charBuf.data(), static_cast<size_t>(readSize));
        }
    }

    /* Wait for process to exit. */
    int returnCode = 0;
    waitpid(pid, &returnCode, 0);

    return returnCode;
}