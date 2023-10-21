#include <iostream>
#include <string>
#include <vector>

inline bool isMatch(const std::string& pattern, const std::string& str) {
    int m = pattern.size();
    int n = str.size();

    // Create a 2D DP table to store intermediate results
    std::vector<std::vector<bool>> dp(m + 1, std::vector<bool>(n + 1, false));

    // Empty pattern matches empty string
    dp[0][0] = true;

    // Fill in the first row
    for (int i = 1; i <= m; i++) {
        if (pattern[i - 1] == '%') {
            dp[i][0] = dp[i - 1][0];
        }
    }

    // Fill in the DP table
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (pattern[i - 1] == str[j - 1] || pattern[i - 1] == '_') {
                dp[i][j] = dp[i - 1][j - 1];
            } else if (pattern[i - 1] == '%') {
                dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
            }
        }
    }

    return dp[m][n];
           
}