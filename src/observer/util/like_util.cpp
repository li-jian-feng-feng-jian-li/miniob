#include <iostream>
#include <string>

inline bool isMatch(const std::string& pattern, const std::string& text) {
    int i = 0; // 索引用于遍历模式串
    int j = 0; // 索引用于遍历文本串

    while (i < pattern.length() && j < text.length()) {
        if (pattern[i] == '%') {
            // 匹配零个到多个任意字符（不包括英文单引号）
            char nextChar = pattern[i + 1];
            while (j < text.length() && (text[j] != nextChar || nextChar == '\'')) {
                j++;
            }
            i += 2;
        } else if (pattern[i] == '_') {
            // 匹配一个任意字符（不包括英文单引号）
            if (text[j] == '\'') {
                return false; // 英文单引号不匹配
            }
            i++;
            j++;
        } else {
            // 普通字符匹配
            if (pattern[i] != text[j]) {
                return false; // 字符不匹配
            }
            i++;
            j++;
        }
    }

    // 检查是否有剩余字符
    while (i < pattern.length() && pattern[i] == '%') {
        i += 2;
    }

    return i == pattern.length() && j == text.length();
}