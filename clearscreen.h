#pragma once

// Includes standard C++
#include <iostream>

void ClearScreen() {
    std::cout << "\033[2J\033[H"; // escape code ANSI, compatible xterm.js
}
