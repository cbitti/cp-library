#include <bits/stdc++.h>
using namespace std;

// Make sure op(x, x) = x for all elements x of type T
// Make sure op is associative
// Common examples of op inclue min, max, GCD, bitwise AND/OR

template<typename T, typename F>
struct SparseTable {
private:
    vector<vector<T>> table;
    int n;
    F op;
public:
    SparseTable(const vector<T>& a, F f) : op(f) {
        int d = 1;
        n = (int)a.size();
        int k = 32 - __builtin_clz(n);
        table.assign(k, vector<T>(n));
        for (int i = 0; i < n; ++i) table[0][i] = a[i];
        for (int p = 1; p < k; ++p) {
            int u = n - (1 << p) + 1;
            for (int i = 0; i < u; ++i) table[p][i] = op(table[p - 1][i], table[p - 1][i + (1 << (p - 1))]);
        }
    }
    // 0-indexed, inclusive [l, r]
    T query(int l, int r) const {
        int p = 31 - __builtin_clz(r - l + 1);
        return op(table[p][l], table[p][(r - (1 << p) + 1)]);
    }
};