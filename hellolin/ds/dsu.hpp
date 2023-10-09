#ifndef HELLOLIN_DS_DSU_HPP
#define HELLOLIN_DS_DSU_HPP 1
#include <algorithm>
#include <vector>

namespace hellolin {

namespace ds {

class dsu {
private:
    int n;
    std::vector<int> g, sz;

public:
    dsu() {}
    explicit dsu(int _n)
        : n(_n) {
        g.resize(n + 1);
        for (int i = 1; i <= n; ++i)
            g[i] = i;
        sz.resize(n + 1, 0);
    }

    int find(int x) {
        return g[x] == x ? x : g[x] = find(g[x]);
    }
    bool same(int x, int y) {
        return find(x) == find(y);
    }
    int size(int x) {
        return sz[find(x)];
    }
    void merge(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return;
        if (sz[x] < sz[y]) std::swap(x, y);
        sz[x] += sz[y];
        g[y] = x;
    }
    void erase(int x) {
        --sz[find(x)];
        g[x] = x;
    }
    void swap(int x, int y) {
        int _x = x;
        x = find(x), y = find(y);
        if (x == y) return;
        g[_x] = y;
        --sz[x], ++sz[y];
    }
};

} // namespace ds

} // namespace hellolin

#endif
