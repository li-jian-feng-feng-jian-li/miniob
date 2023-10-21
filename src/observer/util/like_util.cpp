#include <iostream>
#include <string>

inline bool isMatch(const std::string& pattern, const std::string& text) {
    size_t patternIndex = 0;
    size_t textIndex = 0;

    while (patternIndex < pattern.size() || textIndex < text.size()) {
        if (pattern[patternIndex] == '%') {
            // 匹配零个到多个任意字符，直到下一个字符匹配或字符串结束
            patternIndex++;
            char nextChar = (patternIndex < pattern.size()) ? pattern[patternIndex] : '\0';

            while (textIndex < text.size() && text[textIndex] != nextChar) {
                if (text[textIndex] != '\'') {
                    textIndex++;
                } else {
                    // 忽略英文单引号
                    textIndex++;
                    while (textIndex < text.size() && text[textIndex] != '\'' && text[textIndex] != '\0') {
                        textIndex++;
                    }
                    if (textIndex < text.size() && text[textIndex] == '\'') {
                        textIndex++; // 跳过英文单引号
                    }
                }
            }
        } else if (pattern[patternIndex] == '_') {
            // 匹配一个任意字符，忽略英文单引号
            if (textIndex < text.size() && text[textIndex] != '\'') {
                textIndex++;
            } else if (textIndex < text.size() && text[textIndex] == '\'') {
                textIndex++; // 跳过英文单引号
                while (textIndex < text.size() && text[textIndex] != '\'' && text[textIndex] != '\0') {
                    textIndex++;
                }
                if (textIndex < text.size() && text[textIndex] == '\'') {
                    textIndex++; // 跳过英文单引号
                }
            }
            patternIndex++;
        } else {
            // 普通字符，要求相等，忽略英文单引号
            if (textIndex < text.size() && pattern[patternIndex] != text[textIndex]) {
                return false;
            }
            textIndex++;
            patternIndex++;
        }
    }

    // 如果两者都同时结束，说明匹配成功
    return (patternIndex == pattern.size() && textIndex == text.size());
}