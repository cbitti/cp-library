#include <bits/stdc++.h>
using namespace std;

// Sieve + lpf(smallest prime factor for log(n) factorization)
// Primality check: lpf[n] == n
// Complexity is O(SIEVEMAX)
constexpr int SIEVEMAX = 10000000;
vector<int> lpf(SIEVEMAX + 1);
vector<int> phi(SIEVEMAX + 1);
vector<int> pr;
// Run this at the start of main()
void sieve() {
    phi[1] = 1;
    for (int i = 2; i <= SIEVEMAX; ++i) {
        if (lpf[i] == 0) {
            pr.push_back(i);
            lpf[i] = i;
            phi[i] = i - 1;
        }
        for (int j = 0; i * pr[j] <= SIEVEMAX; ++j) {
            lpf[i * pr[j]] = pr[j];
            if (pr[j] == lpf[i]) {
                phi[i * pr[j]] = phi[i] * pr[j];
                break;
            }
            else phi[i * pr[j]] = phi[i] * (pr[j] - 1);
        }
    }
}

vector<int> getPrimeFactors(int n) {
    vector<int> factors;
    while (n > 1) {
        factors.push_back(lpf[n]);
        n /= lpf[n];
    }
    return factors;
}

map<int, int> getPrimeFactorCounts(int n) {
    map<int, int> counts;
    while (n > 1) {
        auto it = counts.find(lpf[n]);
        if (it == counts.end()) counts.emplace(lpf[n], 1);
        else ++it->second;
        n /= lpf[n];
    }
    return counts;
}

// Returns n^k (mod p)
// Complexity is log_2(k)
// Requires that (p - 1)^2 fits in a long long and k >= 0
long long powMod(long long n, long long k, long long p) {
    n %= p;
    long long result = 1;
    while (k) {
        if (k & 1ll) result = (result * n) % p;
        n = (n * n) % p;
        k >>= 1;
    }
    return result;
}
