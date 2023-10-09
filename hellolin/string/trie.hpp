#ifndef HELLOLIN_STRING_TRIE_HPP
#define HELLOLIN_STRING_TRIE_HPP 1
#include <string>
#include <vector>

namespace hellolin {

namespace string {

class trie {
private:
    int n, cnt;
    std::vector<std::vector<int>> nxt;
    std::vector<int> ex;

public:
    trie() {}
    explicit trie(int _n, int ch = 300)
        : n(_n) {
        ex.resize(n, 0);
        nxt = decltype(nxt)(n, std::vector<int>(n, 0));
    }
    void clear() {
        ex.resize(n, 0);
        nxt = decltype(nxt)(n, std::vector<int>(n, 0));
    }
    void insert(std::string s) {
        int t = 0;
        for (auto &i : s) {
            if (!nxt[t][(int) i]) nxt[t][(int) i] = ++cnt;
            t = nxt[t][(int) i];
            ++ex[t];
        }
    }
    int find(std::string s) {
        int t = 0;
        for (auto &i : s) {
            if (!nxt[t][(int) i]) return 0;
            t = nxt[t][(int) i];
        }
        return ex[t];
    }
};

} // namespace string

} // namespace hellolin

#endif