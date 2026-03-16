// Assumes T is either int or long long depending on how large the prefix sums can get
// Will need adjustments for non-primitive elements that form an abelian group
template <typename T>
struct Fenwick {
    int n;
    vector<T> v;
    Fenwick(int k) {
        assert(k > 0);
        n = k;
        v.assign(n + 1, T{});
    }
    void update(int i, T delta) {
        ++i;
        while (i <= n) {
            v[i] += delta;
            i += (i & -i);
        }
    }
    T query(int i) {
        ++i;
        T result = 0;
        while (i > 0) {
            result += v[i];
            i -= (i & -i);
        }
        return result;
    }
    // This method assumes v[i] = 0 if the element at the ith index is not in the set the BIT represents
    // While v[i] = 1 if the element at the ith index is in the set
    int getKthElementIndex(T k) {
        int pos = 0;
        int digit = 1 << (bit_width((unsigned int)n) - 1);
        while (digit) {
            if (pos + digit <= n && k > v[pos + digit]) {
                pos += digit;
                k -= v[pos];
            }
            digit >>= 1;
        }
        return pos;
    }
};