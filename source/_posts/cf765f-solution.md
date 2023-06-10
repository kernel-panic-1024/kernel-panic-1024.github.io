---
title: Codeforces 765F Souvenirs 题解
date: 2023-06-10 16:18:25
categories: 题解
tags:
  - 扫描线
  - 线段树
---

You are wrong. Here is why.

<!-- more -->

## 思路

只考虑 $a_i \ge a_j (i < j)$ 时的答案．剩下的答案反转值域再做一遍即可．

我们将把询问挂在右端点上扫描线．设当前扫描到的右端点为 $r$，维护 $p_i$ 表示 $\min\limits_{i < p \le r} |a_p - a_i|$，答案就是 $p_i$ 的一个后缀 $\min$，这个树状数组搞一下即可．

考虑 $r - 1 \rightarrow r$ 的过程，我们需要计算加入 $a_r$ 对之前的 $p_i$ 的贡献．先找到最大的 $1 \le x < r$，满足 $a_x \ge a_r$，显然 $a_r$ 能对 $p_x$ 造成贡献．但是还会存在 $a_y \ge a_r (y < r)$，如果暴力枚举的话可以被卡到 $O(n^2)$，无法接受．发现 $a_r$ 若能对 $p_y$ 造成贡献，不仅要有 $a_y \ge a_r$，还需要有 $a_y - a_r < a_x - a_y$，不然扫描到 $x$ 时更新的答案更优秀，也就是说 $a_r < a_y < \left\lfloor \dfrac{a_r + a_x}{2} \right\rfloor$，找到所有的 $y$ 的过程每次区间长度减半，所以只会产生 $O(\log V)$ 次更新．

那么使用权值线段树维护每个值最后出现的位置，扫描时枚举能贡献到的点即可．时间复杂度 $O(n \log^2 V)$．

## 代码

```cpp
#include <cstdio>
#include <cstring>
#include <utility>
#include <vector>

int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = (c != '-') ? 1 : -1, c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return x * f;
}

constexpr int N = 1e5;
constexpr int M = 3e5;
constexpr int inf = 1e9;
constexpr int LOGV = 31;

int n, m;
int a[N + 5];
std::vector<std::pair<int, int>> q[N + 5];
int ans[M + 5];

namespace fwt {
	int t[N + 5];
	int lowbit(int x) { return x & (-x); }
	void init() {
		memset(t, 63, sizeof(t));
	}
	void upd(int x, int v) {
		for (int i = x; i >= 1; i -= lowbit(i)) {
			t[i] = std::min(t[i], v);
		}
	}
	int que(int x) {
		int res = inf;
		for (int i = x; i <= n; i += lowbit(i)) {
			res = std::min(res, t[i]);
		}
		return res;
	}
};

namespace segtree {
#define lch (t[p].ch[0])
#define rch (t[p].ch[1])
#define mid ((cl + cr) / 2)
	struct node {
		int ch[2];
		int mx;
	} t[M * LOGV + 5];
	int rt, alct;
	void init() {
		memset(t, 0, sizeof(t)), rt = 0, alct = 0;
	}
	void pushup(int p) { t[p].mx = std::max(t[lch].mx, t[rch].mx); }
	void upd(int &p, int x, int v, int cl = 0, int cr = inf) {
		if (!p) p = ++alct;
		if (cl == cr) return t[p].mx = v, void();
		(x <= mid) ? upd(lch, x, v, cl, mid)
		: upd(rch, x, v, mid + 1, cr);
		pushup(p);
	}
	int que(int p, int l, int r, int cl = 0, int cr = inf) {
		if (!p || l > r) return 0;
		if (cl == l && cr == r) return t[p].mx;
		if (r <= mid) return que(lch, l, r, cl, mid);
		else if (l > mid) return que(rch, l, r, mid + 1, cr);
		else return std::max(que(lch, l, mid, cl, mid), que(rch, mid + 1, r, mid + 1, cr));
	}
#undef lch
#undef rch
#undef mid
};

using segtree::rt;

int main() {
	n = rd();
	for (int i = 1; i <= n; i++) {
		a[i] = rd();
	}
	m = rd();
	for (int i = 1; i <= m; i++) {
		int l = rd(), r = rd();
		q[r].emplace_back(l, i);
	}

	memset(ans, 63, sizeof(ans));
	fwt::init();
	for (int r = 1; r <= n; r++) {
		int cur = segtree::que(rt, a[r], inf);
		while (cur) {
			fwt::upd(cur, a[cur] - a[r]);
			cur = segtree::que(rt, a[r], (a[r] + a[cur] + 1) / 2 - 1);
		}
		for (auto [l, id] : q[r]) {
			ans[id] = std::min(ans[id], fwt::que(l));
		}
		segtree::upd(rt, a[r], r);
	}
	fwt::init(), segtree::init();
	for (int i = 1; i <= n; i++) a[i] = inf - a[i];
	for (int r = 1; r <= n; r++) {
		int cur = segtree::que(rt, a[r], inf);
		while (cur) {
			fwt::upd(cur, a[cur] - a[r]);
			cur = segtree::que(rt, a[r], (a[r] + a[cur] + 1) / 2 - 1);
		}
		for (auto [l, id] : q[r]) {
			ans[id] = std::min(ans[id], fwt::que(l));
		}
		segtree::upd(rt, a[r], r);
	}

	for (int i = 1; i <= m; i++) {
		printf("%d\n", ans[i]);
	}

	return 0;
}
```