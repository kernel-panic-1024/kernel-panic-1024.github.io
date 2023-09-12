---
title: Codeforces 1707C DFS Trees 题解
date: 2023-09-12 19:47:42
categories: 题解
---

想不到批话了．

<!-- more -->

## 思路

由于边权互不相同，原图的 MST 确定，直接弄出来．

题目中给的那个伪代码就是会找到原图的一个 dfs 树，想到 dfs 树合法的条件就是不存在横叉边，若当 MST 的根确定为 $i$ 时，存在作为横叉边的非树边，那么 $i$ 一定不合法．

这个判定条件是充要的，由于我们找到的是 MST，所以任意一条非树边 $(u, v)$ 加入后构成的环中，$(u, v)$ 的边权一定是最大的，那么无论走到 $u$ 或 $v$ 中的哪一个点，都不会先走 $(u, v)$．

首先随便钦定一个根，然后依次考虑所有非树边 $(u, v)$．

- 若 $u$ 是 $v$ 的祖先或 $v$ 是 $u$ 的祖先，那么这条链上除开 $u$ 和 $v$ 的点都要被日掉．
- 不然，除了 $u$ 和 $v$ 子树内的点都要被日掉．

树上差分打标记即可．

## 代码

```cpp
#include <cstdio>
#include <utility>
#include <vector>
#include <cassert>
#define debug(...) fprintf(stderr, __VA_ARGS__)

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
const int LOGN = 20;

int n, m; pii e[N * 2 + 5]; int ect;

namespace dsu {
	int fa[N + 5];
	void init(int n) { for(int i = 1; i <= n; i++) fa[i] = i; }
	int find(int x) { return x == fa[x] ? x : fa[x] = find(fa[x]); }
};

std::vector<int> g[N + 5];
void add(int u, int v) {
	g[u].push_back(v);
	g[v].push_back(u);
}

int fa[LOGN + 1][N + 5], dep[N + 5];
void dfs(int u, int f) {
	fa[0][u] = f, dep[u] = dep[f] + 1;
	for(int v : g[u]) if(v != f) dfs(v, u);
}

int LCA(int u, int v) {
	if(dep[u] < dep[v]) std::swap(u, v);
	int d = dep[u] - dep[v];
	for(int i = LOGN; i >= 0; i--) {
		if(d & (1 << i)) u = fa[i][u];
	}
	if(u == v) return u;
	for(int i = LOGN; i >= 0; i--) {
		if(fa[i][u] != fa[i][v]) u = fa[i][u], v = fa[i][v];
	}
	assert(fa[0][u] == fa[0][v]);
	return fa[0][u];
}

// a is anc of u
int shift(int u, int a) {
	int d = dep[u] - dep[a] - 1;
	assert(d >= 0);
	for(int i = LOGN; i >= 0; i--) {
		if(d & (1 << i)) u = fa[i][u];
	}
	return u;
}

int t[N + 5];

void calc(int u, int f) {
	for(int v : g[u]) {
		if(v == f) continue;
		t[v] += t[u], calc(v, u);
	}
}

int main() {
	n = rd(), m = rd();
	dsu::init(n);
	for(int i = 1; i <= m; i++) {
		int u = rd(), v = rd();
		if(u == v) continue;
		int fu = dsu::find(u), fv = dsu::find(v);
		if(fu == fv) { e[++ect] = {u, v}; continue; }
		dsu::fa[fu] = fv, add(u, v);
	}
	dfs(1, 0);
	for(int i = 1; i <= LOGN; i++) {
		for(int j = 1; j <= n; j++) {
			fa[i][j] = fa[i - 1][fa[i - 1][j]];
		}
	}
	for(int i = 1; i <= ect; i++) {
		auto [u, v] = e[i];
		int w = LCA(u, v);
		if(w == u) {
			int x = shift(v, u);
			t[x]++, t[v]--;
		} else if(w == v) {
			int x = shift(u, v);
			t[x]++, t[u]--;
		} else {
			t[1]++, t[u]--, t[v]--;
		}
	}
	calc(1, 0);

	for(int i = 1; i <= n; i++) printf("%d", !bool(t[i]));
	puts("");
	return 0;
}
```
