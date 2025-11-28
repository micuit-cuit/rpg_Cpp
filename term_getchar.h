// term_getchar.hpp — header-only simple getch / kbhit for POSIX
#pragma once

// Includes standard C++
#include <chrono>
#include <fcntl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

// Includes conditionnels
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
class TermiosGuard {
    termios oldt;
    bool active = false;
public:
    TermiosGuard() = default;
    void enableRaw() {
        if (active) return;
        tcgetattr(STDIN_FILENO, &oldt);
        termios raw = oldt;
        raw.c_lflag &= ~(ECHO | ICANON | ISIG);
        // raw.c_iflag &= ~(IXON | ICRNL);
        // raw.c_oflag &= ~(OPOST);
        // raw.c_cc[VMIN] = 0;
        // raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        active = true;
    }
    void disable() {
        if (!active) return;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldt);
        active = false;
    }
    ~TermiosGuard() { disable(); }
};

// read single char (blocking)
inline int getch_blocking() {
    unsigned char c;
    while (true) {
        ssize_t n = read(STDIN_FILENO, &c, 1);
        if (n == 1) return c;
    }
}

// read single char non-blocking, returns -1 if none
inline int getch_nonblocking() {
    unsigned char c;
    // set non-blocking on fd
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    ssize_t n = read(STDIN_FILENO, &c, 1);
    // restore flags
    fcntl(STDIN_FILENO, F_SETFL, flags);
    if (n == 1) return c;
    return -1;
}

// wait with timeout (ms) and read char if available, else -1
inline int getch_timeout(int timeout_ms) {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
    int rv = select(STDIN_FILENO + 1, &set, nullptr, nullptr, &tv);
    if (rv > 0) {
        unsigned char c;
        ssize_t n = read(STDIN_FILENO, &c, 1);
        if (n == 1) return c;
    }
    return -1;
}
#ifdef __EMSCRIPTEN__
EM_JS(int, getch, (), {
    return window.lastChar.charCodeAt(0) || -1;
});
#endif
// détecte s'il y a une touche dans le buffer
int kbhit() {
    unsigned char ch;
    int n = read(STDIN_FILENO, &ch, 1);
    if (n > 0) {
        ungetc(ch, stdin); // remet le caractère pour le lire plus tard
        return 1;
    }
    return 0;
}