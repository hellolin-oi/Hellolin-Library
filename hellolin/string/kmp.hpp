#ifndef HELLOLIN_STRING_KMP_HPP
#define HELLOLIN_STRING_KMP_HPP 1
#include <string>
#include <vector>

namespace hellolin {

namespace string {

std::pair<std::vector<int>, std::vector<int>> kmp(std::string a, std::string b) {
    a = '#' + a, b = '#' + b;
    int n = a.size() - 1, m = b.size() - 1;
    std::vector<int> loc;
    std::vector<int> res(m + 1, 0);
    int j = 0;
    for (int i = 2; i <= m; ++i) {
        while (j && b[i] != b[j + 1])
            j = res[j];
        if (b[i] == b[j + 1]) ++j;
        res[i] = j;
    }
    j = 0;
    for (int i = 1; i <= n; ++i) {
        while (j && a[i] != b[j + 1])
            j = res[j];
        if (a[i] == b[j + 1]) ++j;
        if (j == m) {
            loc.push_back(i - m + 1);
            j = res[j];
        }
    }
    return {loc, res};
}

std::vector<int> z_function(std::string a) {
    a = '#' + a;
    int n = a.size() - 1;
    std::vector<int> z(n + 1, 0);
    z[1] = n;
    for (int l = 0, r = 0, i = 2; i <= n; ++i) {
        if (i <= r) z[i] = std::min(z[i - l + 1], r - i + 1);
        while (i + z[i] <= n && a[i + z[i]] == a[1 + z[i]])
            ++z[i];
        if (z[i] + i - 1 > r) {
            l = i;
            r = z[i] + i - 1;
        }
    }
    return z;
}

std::pair<std::vector<int>, std::vector<int>> ex_kmp(std::string a, std::string b) {
    std::vector<int> z = z_function(b);
    a = '#' + a, b = '#' + b;
    int n = a.size() - 1, m = b.size() - 1;
    std::vector<int> k(n + 1, 0);
    for (int l = 0, r = 0, i = 1; i <= n; ++i) {
        if (i <= r) k[i] = std::min(z[i - l + 1], r - i + 1);
        while (i + k[i] <= n && a[i + k[i]] == b[1 + k[i]])
            ++k[i];
        if (k[i] + i - 1 > r) {
            l = i;
            r = k[i] + i - 1;
        }
    }
    return {z, k};
}

} // namespace string

} // namespace hellolin

#endif