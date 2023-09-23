---
title: Codeforces 1817B Fish Graph 题解
date: 2023-09-23 14:19:46
categories: 题解
---

不会 bfs 了．

<!-- more -->

## 思路

将环上外挂两个节点的节点称作关键节点，显然关键节点的度数 $\ge 4$．

考虑枚举关键节点 $u$，我们只需要搜出一个过 $u$ 的最小简单环就能直接构造．

如何搜最小简单环？考虑以 $u$ 为根的 bfs 树，若 bfs 过程中访问到了一条横叉边 $(x, y)$，满足 $x$ 和 $y$ 在 $u$ 的不同子树内，那么 $u \to x \to y \to u$ 显然是一个合法的简单环，而 bfs 过程中访问到的第一个这样的横叉边构成的环显然是最小的．记录 bfs 树上的父边就能还原方案．

## 代码

```cpp
#include <cstdio>
#include <vector>
#include <queue>
#include <utility>
#include <cassert>
#include <cstring>
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

const int N = 2e3;

int n, m; std::vector<int> g[N + 5];
int d[N + 5];

int pre[N + 5], bel[N + 5];
bool trysolve(int s) {
	memset(pre, 0, sizeof(pre));
	memset(bel, 0, sizeof(bel));
	static int q[N + 5]; int hd = 1, tl = 0;
	for(int u : g[s]) q[++tl] = u, pre[u] = s, bel[u] = u;
	while(hd <= tl) {
		int u = q[hd++];
		for(int v : g[u]) {
			if(v == s) continue;
			if(pre[v]) {
				if(bel[u] == bel[v]) continue;
				static pii ans[N + 5]; int tp = 0;
				static bool flg[N + 5]; memset(flg, 0, sizeof(flg));
				ans[++tp] = {u, v};
				for(int p = u; p != s; p = pre[p]) ans[++tp] = {pre[p], p}, flg[p] = true;
				for(int p = v; p != s; p = pre[p]) ans[++tp] = {pre[p], p}, flg[p] = true;
				int ct = 0;
				for(int p : g[s]) {
					if(flg[p]) continue;
					ct++, ans[++tp] = {s, p};
					if(ct == 2) break;
				}
				assert(ct == 2);
				puts("YES");
				printf("%d\n", tp);
				for(int i = 1; i <= tp; i++) {
					auto [x, y] = ans[i];
					printf("%d %d\n", x, y);
				}
				return true;
			} else {
				pre[v] = u, bel[v] = bel[u];
				q[++tl] = v;
			}
		}
	}
	return false;
}

void solve() {
	n = rd(), m = rd();
	for(int i = 1; i <= m; i++) {
		int u = rd(), v = rd(); d[u]++, d[v]++;
		g[u].push_back(v), g[v].push_back(u);
	}
	for(int i = 1; i <= n; i++) {
		if(d[i] < 4) continue;
		if(trysolve(i)) goto EXIT;
	}
	puts("NO");

EXIT:
	for(int i = 1; i <= n; i++) d[i] = 0, g[i].clear();
}

int main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```

## 参考

Alex_Wei, [_CF1817B Fish Graph_](https://www.luogu.com.cn/blog/AlexWei/solution-cf1817b)
