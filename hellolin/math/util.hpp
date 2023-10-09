#ifndef HELLOLIN_MATH_UTIL_HPP
#define HELLOLIN_MATH_UTIL_HPP 1
#include <vector>

namespace hellolin {

namespace math {

class basis {
    using val_type = unsigned long long;

private:
    std::vector<val_type> a;

public:
    basis() { a.resize(64, 0); }
    void insert(val_type x) {
        for (int i = 63; ~i; --i) {
            if (!(x >> i)) continue;
            if (!a[i]) {
                a[i] = x;
                break;
            }
            x ^= a[i];
        }
    }
    val_type query(val_type x = 0) {
        val_type res = x;
        for (int i = 63; ~i; --i) {
            if ((res ^ a[i]) > res)
                res ^= a[i];
        }
        return res;
    }
};

} // namespace math

} // namespace hellolin

#endif