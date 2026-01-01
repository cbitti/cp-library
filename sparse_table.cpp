#include <bits/stdc++.h>
using namespace std;

// Make sure op(x, x) = x for all elements x of type T
// Make sure op is associative
// Common examples of op inclue min, max, GCD, bitwise AND/OR

template<typename T, typename F>
struct SparseTable {
private:
    vector<T> table;
    int n;
    F op;
public:
    SparseTable(const vector<T>& a, F f) : op(f) {
        n = (int)a.size();
        assert(n > 0);
        int k = 32 - __builtin_clz(n);
        table.resize(k * n);
        for (int i = 0; i < n; ++i) table[i] = a[i];
        for (int p = 1; p < k; ++p) {
            int u = n - (1 << p) + 1;
            for (int i = 0; i < u; ++i) table[p * n + i] = op(table[(p - 1) * n + i], table[(p - 1) * n + (i + (1 << (p - 1)))]);
        }
    }
    // 0-indexed, inclusive [l, r]
    T query(int l, int r) const {
        assert(0 <= l && l <= r && r < n);
        int p = 31 - __builtin_clz(r - l + 1);
        return op(table[p * n + l], table[p * n + (r - (1 << p) + 1)]);
    }
};

// Usage:

int main() {
    vector<int> v = {1, 2, 4, -1, 2};

    // Instantiate with either a functor instance or lambda (like in this example)
    SparseTable table(v, [](int x, int y) {
        return max(x, y);
    });

    int answer = table.query(1, 3); // Value is 4

    return 0;
}