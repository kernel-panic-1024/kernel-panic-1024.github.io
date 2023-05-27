---
title: 洛谷 3732 [HAOI2017] 供给侧改革
date: 2023-05-27 23:24:34
categories: 题解
tags:
  - Trie
  - 线段树
  - 随机
---

来自可爱学妹的带 $\log$ 巨大常数做法．

<!-- more -->

## 思路

记 $S_i$ 代表由 $i$ 开始的后缀．

首先，由于数据随机，不妨猜测 LCP 的最大长度不超过某个小常数 $K$．

询问比较诡异，猜测难以高效地在线解决，于是考虑离线．我们将询问挂在右端点上，扫描线解决每个询问．

扫描线上直接对于当前扫描的右端点 $r$ 维护 $X_i = \operatorname{data}(i, r)$，询问就是对 $X_i$ 区间求和，丢到线段树上即可．

考虑移动右端点，插入 $S_r$ 长度为 $K$ 的前缀时对 $X_i$ 的影响．由于 $K$ 较小，我们不妨暴力修改．具体而言，我们枚举 $S_r$ 长度为 $1 \sim K$ 的前缀 $T$，钦定它为 LCP，然后找到 $r$ 之前最后一个含有 $T$ 这个前缀的位置 $p$，对于 $1 \le j \le p$ 的 $X_j$，都要对 $|T|$ 取 $\max$．

如何找到这个 $p$？考虑 Trie 的性质，对于一个串 $S$，它的一个前缀在 Trie 上对应的节点在 $S$ 对应的节点的祖先链上．那么我们维护一个插入了所有 $i < r, S_i$ 的 Trie，然后对于每个节点 $u$ 维护一个值 $\mathrm{mx}_u$，表示最后一次在插入时经过 $u$ 这个节点是在插入 $S_{\mathrm{mx}_u}$ 时，那么 $u$ 节点所对应的串最后一次作为前缀出现就是在 $S_{\mathrm{mx}_u}$ 中．

枚举 $S_r$ 的前缀再在 Trie 上查找出对应节点显然不优，我们可以先找到 $S_r$ 长度为 $K$ 的前缀在 Trie 上对应的节点，然后从下往上遍历祖先链上的点进行修改．

由于对于 $i < j$，$\operatorname{data}(i, r)$ 中包含了 $\operatorname{data}(j, r)$ 的贡献，所以 $X_i$ 单调不增，取 $\max$ 操作一定是对某个区间的覆盖，可以在线段树上二分出目标区间的左端点，然后区间覆盖．

时间复杂度 $O(qK\log n)$，$K$ 取 $32$ 即可通过本题．

## 代码

```cpp
#include <cstdio>
#include <utility>
#include <vector>

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

using pii = std::pair<int, int>;

const int N = 1e5;
const int K = 32;

int n, q;
char a[N + 5];

std::vector<pii> qu[N + 5];
int ans[N + 5];

int ch[N * K + 5][2], dep[N * K + 5], mx[N * K + 5], alct;
void ins(char *s, int len, int id) {
	int p = 0;
	for (int i = 1; i <= len; i++) {
		int d = s[i] - '0';
		if (!ch[p][d]) ch[p][d] = ++alct;
		p = ch[p][d], mx[p] = id, dep[p] = i;
	}
}

#define lch (p * 2)
#define rch (p * 2 + 1)
#define mid ((t[p].l + t[p].r) / 2)
struct node {
	int l, r;
	int sum, mn;
	int tg;
	void upd(int d) {
		sum = d * (r - l + 1);
		mn = d, tg = d;
	}
} t[N * 4 + 5];
void pushup(int p) {
	t[p].sum = t[lch].sum + t[rch].sum;
	t[p].mn = std::min(t[lch].mn, t[rch].mn);
}
void pushdown(int p) {
	if (!t[p].tg) return;
	t[lch].upd(t[p].tg);
	t[rch].upd(t[p].tg);
	t[p].tg = 0;
}
void build(int p = 1, int cl = 1, int cr = n) {
	t[p].l = cl, t[p].r = cr;
	if (cl == cr) return;
	build(lch, cl, mid), build(rch, mid + 1, cr);
}
void upd(int l, int r, int d, int p = 1) {
	if (t[p].l == l && t[p].r == r) return t[p].upd(d);
	pushdown(p);
	if (r <= mid) upd(l, r, d, lch);
	else if (l > mid) upd(l, r, d, rch);
	else upd(l, mid, d, lch), upd(mid + 1, r, d, rch);
	pushup(p);
}
int que(int l, int r, int p = 1) {
	if (t[p].l == l && t[p].r == r) return t[p].sum;
	pushdown(p);
	if (r <= mid) return que(l, r, lch);
	else if (l > mid) return que(l, r, rch);
	else return que(l, mid, lch) + que(mid + 1, r, rch);
}
int getpos(int k, int p = 1) {
	if (t[p].l == t[p].r) return t[p].mn < k ? t[p].l : 0;
	pushdown(p);
	return (t[lch].mn < k) ? getpos(k, lch) : getpos(k, rch);
}
#undef lch
#undef rch
#undef mid

int st[N + 5];
void solve(char *s, int len, int id) {
	int p = 0, tp = 0;
	for (int i = 1; i <= len; i++) {
		int d = s[i] - '0';
		if (!ch[p][d]) break;
		p = ch[p][d], st[++tp] = p;
	}
	while (tp) {
		int u = st[tp];
		int pos = getpos(dep[u]);
		if (pos && pos <= mx[u]) {
			upd(pos, mx[u], dep[u]);
		}
		tp--;
	}
	ins(s, len, id);
}

int main() {
	n = rd(), q = rd();
	scanf("%s", a + 1);
	for (int i = 1; i <= q; i++) {
		int l = rd(), r = rd();
		qu[r].emplace_back(l, i);
	}

	build();
	for (int i = 1; i <= n; i++) {
		solve(a + i - 1, std::min(n - i + 1, K), i);
		for (auto [l, id] : qu[i]) ans[id] = que(l, i);
	}

	for (int i = 1; i <= q; i++) printf("%d\n", ans[i]);
	return 0;
}
```
