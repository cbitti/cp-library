#include <bits/stdc++.h>
using namespace std;

using ll  = long long;
using pii = pair<int,int>;

#define all(v) begin(v), end(v)

// Make sure Op(x, x) = x for all elements x of type T; common examples of Op inclue min, max, GCD, bitwise AND/OR
// Make sure Op is associative
template<typename T, typename Op>
struct SparseTable {
private:
    vector<T> table;
    vector<int> lg2;
    Op op;
    int n;
public:
    SparseTable(const vector<T>& a) {
        op = Op();
        int d = 1;
        n = (int)a.size();
        int k = 0;
        while (d <= n) {
            ++k;
            d <<= 1;
        }
        table.assign((ll)n * k, T{});
        for (int i = 0; i < n; ++i) table[i] = a[i];
        for (int p = 1; p < k; ++p) {
            int u = n - (1 << p) + 1;
            for (int i = 0; i < u; ++i) table[p*n + i] = op(table[(p - 1)*n + i], table[(p - 1)*n + (i + (1 << (p - 1)))]);
        }
        lg2.assign(n + 1, 0);
        int p = 0;
        d = 1;
        for (int i = 1; i <= n; ++i) {
            while (d <= i) {
                ++p;
                d <<= 1;
            }
            lg2[i] = p - 1;
        }
    }
    T query(int l, int r) const {
        int p = lg2[r - l + 1];
        return op(table[p*n + l], table[p*n + (r - (1 << p) + 1)]);
    }
};