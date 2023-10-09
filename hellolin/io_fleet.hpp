#ifndef HELLOLIN_IO_FLEET_HPP
#define HELLOLIN_IO_FLEET_HPP 1
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>

namespace hellolin {

namespace iof {

#if __cplusplus >= 201703L
#define noignore [[nodiscard]]
#else
#define noignore __attribute__((warn_unused_result))
#endif

template <class T>
inline constexpr bool is_signed_int128_v = std::is_same_v<T, __int128>;
template <class T>
inline constexpr bool is_unsigned_int128_v = std::is_same_v<T, unsigned __int128>;
template <class T>
inline constexpr bool is_int128_v = is_signed_int128_v<T> || is_unsigned_int128_v<T>;
template <class T>
inline constexpr bool is_signed_int_v = is_signed_int128_v<T> || (std::is_integral_v<T> && std::is_signed_v<T>);
template <class T>
inline constexpr bool is_unsigned_int_v = is_unsigned_int128_v<T> || (std::is_integral_v<T> && std::is_unsigned_v<T>);
template <class T>
inline constexpr bool is_int_v = is_int128_v<T> || std::is_integral_v<T>;
template <class T>
inline constexpr bool is_float_v = std::is_floating_point_v<T>;

class io_fleet_base {
public:
    // virtual
    io_fleet_base() = default;
    virtual ~io_fleet_base() = default;

private:
    io_fleet_base(const io_fleet_base &) = delete;
    io_fleet_base &operator=(const io_fleet_base &) = delete;
};

template <class T>
constexpr bool is_io_fleet_v = std::is_base_of_v<io_fleet_base, T>;

enum cmd {
    endl,
    ends,
    flush,
    bin,
    oct,
    dec,
    hex,
    boolalpha,
    noboolalpha,
    showbase,
    noshowbase,
    upper,
    lower,
    fixed,
    flex
};
struct setbase {
    int x;
    setbase(int _x)
        : x(_x) {}
};
struct setfill {
    char x;
    setfill(char _x)
        : x(_x) {}
};
struct setwidth {
    int x;
    setwidth(int _x)
        : x(_x) {}
};
struct setprec {
    int x;
    setprec(int _x)
        : x(_x) {}
};

class in_fleet : public io_fleet_base {
private:
    unsigned int in_size = 2097152;
    std::streambuf *sbuf;
    char *buf, *buf_s, *buf_e;
    int form = 10;
    bool fail = false;
    bool _ismark(char c) {
        return isspace(c) || c == '-' || c == '+';
    }
    int _tointeger(char c) {
        if (!isgraph(c)) return INT_MAX;
        if (isupper(c)) return c - 'A' + 10;
        if (islower(c)) return c - 'a' + 10;
        return c - '0';
        return -1;
    }
    bool _isinteger(char c) {
        return _tointeger(c) < form;
    }
    template <class T, std::enable_if_t<is_int_v<T>, bool> = 0>
    void _gt(T &x) {
        bool f = false;
        x = 0;
        char c = -1;
        while (_ismark(c = getc()) && !fail)
            f = (c == '-' && std::is_signed_v<T>);
        if (fail) return;
        while (_isinteger(c = getc()) && !fail)
            x = x * form + _tointeger(c);
        x = f ? -x : x;
    }
    template <class T, std::enable_if_t<is_float_v<T>, bool> = 0>
    void _gt(T &x) {
        bool f = false;
        x = 0;
        char c = -1;
        while (_ismark(c = getc()) && !fail)
            f = (c == '-' && std::is_unsigned_v<T>);
        if (fail) return;
        while (_isinteger(c = getc()) && !fail)
            x = x * form + _tointeger(c);
        if (c == '.') {
            int64_t pow = 1;
            while (_isinteger(c = getc()) && !fail)
                x += _tointeger(c) / (T) (pow *= form);
        }
        x = f ? -x : x;
    }
    void _gt(bool &x) {
        int64_t y;
        _gt(y);
        x = (bool) y;
    }
    void _gt(char &x) {
        while (isspace(x = getc()) && !fail)
            ;
    }
    void _gt(char *x) {
        int n = 0;
        x[0] = '\0';
        char c = -1;
        while (isspace(c = getc()) && !fail)
            ;
        if (fail) return;
        while (isgraph(c = getc()) && !fail)
            x[n++] = c;
        x[n] = '\0';
    }
    void _gt(std::string &x) {
        x.clear();
        char c = -1;
        while (isspace(c = getc()) && !fail)
            ;
        if (fail) return;
        while (isgraph(c = getc()) && !fail)
            x += c;
    }
    void _gtl(char *x) {
        int n = 0;
        x[0] = '\0';
        char c = -1;
        while (isspace(c = getc()) && !fail)
            ;
        if (fail) return;
        while (c != '\n' && !fail)
            x[n++] = c;
        if (x[n - 1] == '\r') --n;
        x[n] = '\0';
    }
    void _gtl(std::string &x) {
        x.clear();
        char c = -1;
        while (isspace(c = getc()) && !fail)
            ;
        if (fail) return;
        while (c != '\n' && !fail)
            x += c;
        if (x.back() == '\r') x.pop_back();
    }
    void _del() {
        delete[] buf;
        delete[] buf_s;
        delete[] buf_e;
        fail = false;
    }
    void _init(unsigned int _sz) {
        sbuf = std::cin.rdbuf(); // stdin
        in_size = _sz;
        buf = buf_s = buf_e = new char[in_size];
        fail = false;
    }
    void _init(std::streambuf *_sbuf, unsigned int _sz) {
        sbuf = _sbuf;
        in_size = _sz;
        buf = buf_s = buf_e = new char[in_size];
        fail = false;
    }
    void _init(const char *_fi, unsigned int _sz) {
        static std::ifstream ifs(_fi);
        sbuf = ifs.rdbuf(); // stdin
        in_size = _sz;
        buf = buf_s = buf_e = new char[in_size];
        fail = false;
    }

public:
    in_fleet() { _init(2097152); }
    explicit in_fleet(unsigned int _sz) { _init(_sz); }
    explicit in_fleet(std::streambuf *_sbuf, unsigned int _sz = 2097152) { _init(_sbuf, _sz); }
    explicit in_fleet(const char *_fi, unsigned int _sz = 2097152) { _init(_fi, _sz); }
    std::streambuf *rdbuf() { return sbuf; }
    void rdbuf(std::streambuf *_sbuf) {
        _del();
        _init(_sbuf, in_size);
    }
    void redirect(const char *_fi) {
        _del();
        _init(_fi, in_size);
    }

    operator bool() { return !fail; }
    bool operator!() { return fail; }

    noignore char getc() {
        char c = -1;
        if (buf_s == buf_e) {
            buf_s = buf;
            buf_e = buf + sbuf->sgetn(buf, in_size);
        }
        return (buf_s == buf_e ? fail = 1, c : c = *buf_s++);
    }
    char ungetc() {
        return (buf_s == buf ? *buf_s : *--buf_s);
    }
    noignore char peek() {
        char c = -1;
        if (buf_s == buf_e) {
            buf_s = buf;
            buf_e = buf + sbuf->sgetn(buf, in_size);
        }
        return (buf_s == buf_e ? fail = 1, c : c = *buf_s);
    }
    template <class T>
    in_fleet &operator>>(T &x) {
        if (fail) return *this;
        _gt(x);
        return *this;
    }
    in_fleet &operator>>(cmd x) {
        switch (x) {
        case bin:
            form = 2;
            break;
        case oct:
            form = 8;
            break;
        case dec:
            form = 10;
            break;
        case hex:
            form = 16;
            break;
        default:
            break;
        }
        return *this;
    }
    in_fleet &operator>>(setbase x) {
        form = x.x;
        if (form > 36) form = 2;
        return *this;
    }
    in_fleet &getline(char *x) {
        if (fail) return *this;
        _gtl(x);
        return *this;
    }
    in_fleet &getline(std::string &x) {
        if (fail) return *this;
        _gtl(x);
        return *this;
    }
};

class out_fleet : public io_fleet_base {
private:
    std::streambuf *sbuf;
    std::string buf;
    int64_t eps = 1000000;
    int form = 10, prec = 6, width = 6;
    bool boolalpha = false, showbase = false, fixed = false, upper = false;
    char fic = ' ';
    int64_t _pow(int64_t x, uint64_t y) {
        int64_t res = 1;
        for (; y; y >>= 1, x *= x)
            if (y & 1) res *= x;
        return res;
    }
    char _toalpha(int x) {
        if (x < 10) return x + '0';
        return x + (upper ? 'A' : 'a') - 10;
    }
    template <class T, std::enable_if_t<is_int_v<T>, bool> = 0>
    void _pt(T x) {
        static char ch[210];
        char *h = ch + 200, *t = ch + 200;
        bool f = x < 0;
        x = -x;
        if (!x) *h-- = '0';
        while (x) {
            *h-- = _toalpha(x % form);
            x /= form;
        }
        if (showbase) {
            switch (form) {
            case 2:
                *h-- = upper ? 'B' : 'b';
                *h-- = '0';
                break;
            case 8:
                *h-- = '0';
                break;
            case 16:
                *h-- = upper ? 'X' : 'x';
                *h-- = '0';
                break;
            }
        }
        if (f) *h-- = '-';
        fillc(t - h);
        puts(++h, t);
    }
    template <class T, std::enable_if_t<is_float_v<T>, bool> = 0>
    void _pt(T x) {
        if (isnan(x)) {
            _pt(upper ? "NAN" : "nan");
            return;
        }
        if (isinf(x)) {
            if (x > 0)
                _pt(upper ? "INF" : "inf");
            else
                _pt(upper ? "-INF" : "-inf");
            return;
        }
        static char buf[210];
        char *h = buf + 200, *t = buf + 200;
        int64_t p1 = std::floor(x), p2 = std::round(eps * (x - p1));
        if (p2 >= eps) {
            p2 = 0;
            if (p1 < 0)
                --p1;
            else
                ++p1;
        }
        _pt(p1);
        int n = prec;
        while (n--) {
            *h-- = _toalpha(p2 % form);
            p2 /= form;
        }
        if (!fixed) {
            while (*t == '0' && h != t)
                --t;
        }
        if (h != t) *h-- = '.';
        fillc(t - h);
        puts(++h, t);
    }
    void _pt(const void *x) {
        if (x == nullptr) {
            _pt(upper ? "NULLPTR" : "nullptr");
            return;
        }
        int _form = form;
        bool _showbase = showbase;
        form = 16;
        showbase = true;
        _pt((uint64_t) x);
        form = _form;
        showbase = _showbase;
    }
    void _pt(bool x) {
        if (boolalpha) {
            _pt(x ? (upper ? "TRUE" : "true") : (upper ? "FALSE" : "false"));
        } else {
            _pt(x ? '1' : '0');
        }
    }
    void _pt(char x) {
        fillc(1);
        putc(x);
    }
    void _pt(const char *x) {
        fillc(strlen(x));
        puts(x);
    }
    void _pt(const std::string &x) {
        fillc(x.size());
        puts(x);
    }
    void _pt(std::string_view x) {
        fillc(x.size());
        puts(x.data());
    }
    void _pt(cmd x) {
        switch (x) {
        case endl:
            _pt('\n');
            break;
        case ends:
            _pt(' ');
            break;
        case cmd::flush:
            flush();
            break;
        case bin:
            form = 2;
            break;
        case oct:
            form = 8;
            break;
        case dec:
            form = 10;
            break;
        case hex:
            form = 16;
            break;
        case cmd::boolalpha:
            boolalpha = true;
            break;
        case noboolalpha:
            boolalpha = false;
            break;
        case cmd::showbase:
            showbase = true;
            break;
        case noshowbase:
            showbase = false;
            break;
        case cmd::upper:
            upper = true;
            break;
        case lower:
            upper = false;
            break;
        case cmd::fixed:
            fixed = true;
            break;
        case flex:
            fixed = false;
            break;
        }
    }
    void _pt(setbase x) {
        form = std::max(std::min(36, x.x), 2);
        eps = _pow(form, prec);
    }
    void _pt(setfill x) {
        fic = x.x;
    }
    void _pt(setprec x) {
        prec = x.x;
        eps = _pow(form, prec);
    }
    void _pt(setwidth x) {
        width = x.x;
    }
    void _init(std::streambuf *_sbuf = std::cout.rdbuf() /* stdout */) {
        sbuf = _sbuf;
    }
    void _init(const char *_fi) {
        static std::ofstream ofs(_fi);
        sbuf = ofs.rdbuf();
    }

public:
    out_fleet() { _init(); }
    explicit out_fleet(std::streambuf *_sbuf) { _init(_sbuf); }
    explicit out_fleet(const char *_fi) { _init(_fi); }
    std::streambuf *rdbuf() { return sbuf; }
    void rdbuf(std::streambuf *_sbuf) { _init(_sbuf); }
    void redirect(const char *_fi) { _init(_fi); }

    void putc(char x) {
        buf += x;
    }
    void puts(char *x) {
        while (*x != '\0')
            buf += *x++;
    }
    void puts(char *x, int len) {
        while (len--)
            buf += *x++;
    }
    void puts(char *x, char *y) {
        while (x != y)
            buf += *x++;
        buf = *x;
    }
    void puts(std::string s) {
        buf += s;
    }
    void fillc(int len) {
        if (width > len) {
            int fsz = width - len;
            while (fsz--)
                buf += fic;
        }
        width = 0;
    }
    void flush() {
        sbuf->sputn(buf.c_str(), buf.size());
        buf.clear();
    }
    ~out_fleet() override { flush(); }
    template <class T>
    out_fleet &operator<<(T x) {
        _pt(x);
        return *this;
    }
};

} // namespace iof

using enum iof::cmd;
using iof::setbase;
using iof::setfill;
using iof::setprec;
using iof::setwidth;
iof::in_fleet inx;
iof::out_fleet outx;

} // namespace hellolin

#endif