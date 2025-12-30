#include <bits/stdc++.h>
using namespace std;

struct Combinatorics {
private:
    long long p; // prime modulus, must have p > n
    vector<long long> factorial;
    vector<long long> inverse_factorial;
public:
    Combinatorics(long long n, long long q) : p(q) {
        factorial.assign(n + 1, 0);
        inverse_factorial.assign(n + 1, 0);
        factorial[0] = 1;
        for (long long i = 1; i <= n; ++i) factorial[i] = (i * factorial[i - 1]) % p;
        long long exp = p - 2;
        inverse_factorial[n] = 1;
        long long cur = factorial[n];
        while (exp) {
            if (exp & 1ll) inverse_factorial[n] = (inverse_factorial[n] * cur) % p;
            exp >>= 1;
            cur = (cur * cur) % p; 
        }
        for (long long i = n - 1; i >= 0; --i) inverse_factorial[i] = (inverse_factorial[i + 1] * (i + 1)) % p;
    }
    long long fac(long long n) {
        return factorial[n];
    }
    long long invfac(long long n) {
        return inverse_factorial[n];
    }
    long long choose(long long n, long long k) {
        if (n < k) return 0ll;
        else if (k < 0) return 0ll;
        return factorial[n] * inverse_factorial[k] % p * inverse_factorial[n - k] % p;
    }
};