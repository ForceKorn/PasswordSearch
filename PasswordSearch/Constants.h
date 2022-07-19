#pragma once

#include <string_view>
#include <chrono>

inline static constexpr size_t gsk_maxBruteForcePasswordLength = 8;
inline static constexpr size_t gsk_minThreadsToUse = 3;
inline static constexpr size_t gsk_CurrentPasswordLength = 4;
inline static constexpr auto   gsk_printSleepTime = std::chrono::milliseconds(100);

inline static constexpr std::string_view gsk_cipherAlphabet = "0123456789abcdefghijklmnopqrstuvwxyz";
inline static constexpr size_t gsk_cipherAlphabetSize = gsk_cipherAlphabet.size();

inline static constexpr std::string_view gsk_keyPasswordsLogging = "--log_passwords";

