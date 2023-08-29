---
title: QOJ 894 Longest Loose Segment 题解
date: 2023-08-29 22:46:53
categories: 题解
tags: Cartesian 树
---

我们 Cartesian 树这么厉害？

<!-- more -->

## 思路

考虑建出小根 Cartesian 树．

对于每个节点 $u$，计算其对应区间的长度 $\mathrm{siz}_u$，最大值 $\mathrm{mx}_u$ 和最大合法子段长度 $\mathrm{gs}_u$．

考虑合并两个子树的答案．

1. 若最大合法子段不经过 $u$，那么直接继承两颗子树的答案．
2. 若经过，$\min$ 一定在 $u$ 处取到，不妨令 $\max$ 在左子树内取到．那么不断调整我们选取的区间，一定能使区间左端点调整到左子树对应区间的左端点，或将区间右端点调整到 $u$．那么这种情况只需考虑一个端点在 $u$ 对应的区间的左右端点，并且经过 $u$ 的子段．这种情况的答案为 $\min\{\mathrm{siz}_u, a_u + \mathrm{mx}_u - 1\}$．

## 代码

```cpp
#include <algorithm>
#include <cstdio>
#define debug(...) fprintf(stderr, __VA_ARGS__)

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

const int N = 1e6;

int n, m, a[N + 5];

int ch[N + 5][2], mx[N + 5], siz[N + 5], gs[N + 5];
void dfs(int u) {
	mx[u] = a[u], siz[u] = 1;
	for (int i = 0; i < 2; i++) {
		int v = ch[u][i];
		if (!v) continue;
		dfs(v);
		mx[u] = std::max(mx[u], mx[v]);
		siz[u] += siz[v];
		gs[u] = std::max(gs[u], gs[v]);
	}
	gs[u] = std::max(gs[u], std::min(siz[u], a[u] + mx[u] - 1));
}
int st[N + 5], tp;
int solve() {
	for (int i = 1; i <= n; i++) {
		ch[i][0] = ch[i][1] = 0;
		mx[i] = siz[i] = gs[i] = 0;
	}

	tp = 0;
	for (int i = 1; i <= n; i++) {
		int k = tp;
		while (k && a[st[k]] > a[i]) k--;
		if (k) ch[st[k]][1] = i;
		if (k < tp) ch[i][0] = st[k + 1];
		st[++k] = i, tp = k;
	}
	dfs(st[1]);
	return gs[st[1]];
}

int main() {
	n = rd(), m = rd();
	for (int i = 1; i <= n; i++) a[i] = rd();
	printf("%d\n", solve());
	for (int i = 1; i <= m; i++) {
		int k = rd();
		for (int j = 1; j <= k; j++) {
			int x = rd(), y = rd();
			std::swap(a[x], a[y]);
		}
		printf("%d\n", solve());
	}
	return 0;
}
```
