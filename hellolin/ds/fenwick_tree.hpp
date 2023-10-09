#ifndef HELLOLIN_DS_FENWICK_TREE_HPP
#define HELLOLIN_DS_FENWICK_TREE_HPP 1
#include <vector>

namespace hellolin {

namespace ds {

template <class T>
class fenwick_tree {
private:
    int n;
    std::vector<int> tr;

public:
    fenwick_tree() {}
    explicit fenwick_tree(int _n)
        : n(_n) {
        tr.resize(n + 1, 0);
    }
    void add(int p, T w) {
        for (; p <= n; p += p & -p)
            tr[p] += w;
    }
    T sum(int p) {
        T res = 0;
        for (; p >= 1; p -= p & -p)
            res += tr[p];
        return res;
    }
    T sum(int l, int r) {
        return sum(r) - sum(l - 1);
    }
};

} // namespace ds

} // namespace hellolin

#endif