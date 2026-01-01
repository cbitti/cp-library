#include <bits/stdc++.h>
using namespace std;

template <typename Node, typename Tag>
class LazySegTree {
    int n;
    vector<Node> tree;
    vector<Tag> lazy;

    void push(int v) {
        // 1. Apply parent tag to children
        tree[2 * v].apply(lazy[v]);
        tree[2 * v + 1].apply(lazy[v]);

        // 2. Push tag into children's tags
        lazy[2 * v].compose(lazy[v]);
        lazy[2 * v + 1].compose(lazy[v]);

        // 3. Reset parent tag
        lazy[v] = Tag::identity();
    }

    void pull(int v) {
        tree[v] = Node::unite(tree[2 * v], tree[2 * v + 1]);
    }

    void build(const vector<Node>& init, int v, int tl, int tr) {
        if (tl == tr) {
            tree[v] = init[tl];
        } else {
            int tm = (tl + tr) / 2;
            build(init, 2 * v, tl, tm);
            build(init, 2 * v + 1, tm + 1, tr);
            pull(v);
        }
    }

    void update(int v, int tl, int tr, int l, int r, const Tag& tag) {
        if (l > r) return;
        if (l == tl && r == tr) {
            tree[v].apply(tag);
            lazy[v].compose(tag);
        } else {
            push(v);
            int tm = (tl + tr) / 2;
            update(2 * v, tl, tm, l, min(r, tm), tag);
            update(2 * v + 1, tm + 1, tr, max(l, tm + 1), r, tag);
            pull(v);
        }
    }

    Node query(int v, int tl, int tr, int l, int r) {
        if (l > r) return Node::identity();
        if (l == tl && r == tr) return tree[v];
        push(v);
        int tm = (tl + tr) / 2;
        Node left_res = query(2 * v, tl, tm, l, min(r, tm));
        Node right_res = query(2 * v + 1, tm + 1, tr, max(l, tm + 1), r);
        return Node::unite(left_res, right_res);
    }

public:
    LazySegTree(const vector<Node>& init) {
        n = init.size();
        tree.resize(4 * n, Node::identity());
        lazy.resize(4 * n, Tag::identity());
        if (n > 0) build(init, 1, 0, n - 1);
    }

    void update(int l, int r, const Tag& tag) {
        update(1, 0, n - 1, l, r, tag);
    }

    Node query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }
};

// Tag template
struct SumTag {
    long long add_val;

    // Apply a new update to this existing tag
    void compose(const SumTag& other) {
        add_val += other.add_val;
    }

    static SumTag identity() { return {0}; }
};

// Node template
struct SumNode {
    long long val;
    int width;

    // Apply a tag to this node
    void apply(const SumTag& tag) {
        val += tag.add_val * width;
    }

    // Merge two nodes
    static SumNode unite(const SumNode& a, const SumNode& b) {
        return {a.val + b.val, a.width + b.width};
    }

    static SumNode identity() { return {0, 0}; }
};