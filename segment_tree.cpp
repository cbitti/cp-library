#include <bits/stdc++.h>
using namespace std;

template <typename Node, typename Tag>
class LazySegTree {
    int n;
    vector<Node> tree;
    vector<Tag> lazy;

    // Helper to apply a tag to a node and accumulate it if it's not a leaf
    void apply(int p, const Tag& tag) {
        tree[p].apply(tag);
        // If p < n, it's an internal node, so we must store the lazy tag for its children
        if (p < n) lazy[p].compose(tag);
    }

    // Recompute the node values moving bottom-up to the root
    void build(int p) {
        while (p > 1) {
            p >>= 1;
            tree[p] = Node::unite(tree[p << 1], tree[p << 1 | 1]);
            tree[p].apply(lazy[p]); // Apply any existing lazy tag pending on this node
        }
    }

    // Push lazy tags top-down from the root to node p
    void push(int p) {
        // __builtin_clz counts leading zeros. 31 - clz gives the position of the highest set bit.
        // This loops dynamically exactly according to the depth of node p.
        for (int s = 31 - __builtin_clz(p); s > 0; --s) {
            int i = p >> s;
            apply(i << 1, lazy[i]);
            apply(i << 1 | 1, lazy[i]);
            lazy[i] = Tag::identity(); // Clear the parent's tag
        }
    }

public:
    LazySegTree(const vector<Node>& init) {
        n = init.size();
        if (n == 0) return;
        
        tree.assign(2 * n, Node::identity());
        lazy.assign(n, Tag::identity());

        // 1. Initialize leaves
        for (int i = 0; i < n; ++i) {
            tree[n + i] = init[i];
        }
        // 2. Initialize internal nodes bottom-up (this automatically merges widths as well)
        for (int i = n - 1; i > 0; --i) {
            tree[i] = Node::unite(tree[i << 1], tree[i << 1 | 1]);
        }
    }

    void update(int l, int r, const Tag& tag) {
        if (l > r) return;
        
        // Translate to leaf indices and convert to half-open interval [l, r)
        l += n; 
        r += n + 1; 
        int l0 = l, r0 = r - 1;

        // Push pending tags down the paths to the boundaries
        push(l0);
        push(r0);

        // Process the range bottom-up
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) apply(l++, tag);
            if (r & 1) apply(--r, tag);
        }

        // Recompute the tree upwards from the boundaries
        build(l0);
        build(r0);
    }

    Node query(int l, int r) {
        if (l > r) return Node::identity();
        
        // Translate to leaf indices and convert to half-open interval [l, r)
        l += n; 
        r += n + 1; 
        
        // Push pending tags down the paths to the boundaries
        push(l);
        push(r - 1);

        Node res_l = Node::identity();
        Node res_r = Node::identity();

        // Accumulate the range bottom-up
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res_l = Node::unite(res_l, tree[l++]);
            if (r & 1) res_r = Node::unite(tree[--r], res_r); // Order matters here!
        }

        // Final merge
        return Node::unite(res_l, res_r);
    }
};

// Tag template
struct Tag {
    long long add_val;

    // Apply a new update to this existing tag
    void compose(const Tag& other) {
        add_val += other.add_val;
    }

    static Tag identity() { return {0}; }
};

// Node template
struct Node {
    long long val;
    int width;

    // Apply a tag to this node
    void apply(const Tag& tag) {
        val += tag.add_val * width;
    }

    // Merge two nodes
    static Node unite(const Node& a, const Node& b) {
        return {a.val + b.val, a.width + b.width};
    }

    static Node identity() { return {0, 0}; }
};