---
title: Codeforces 1499F Diameter Cuts 题解
date: 2023-09-23 13:41:04
categories: 题解
tags:
  - 动态规划
---

<!-- more -->

## 思路

设 $f_{u, i}$ 表示 $u$ 的子树内，以 $u$ 开始的链最长的长度为 $i$ 的方案数．

转移考虑合并一棵子树 $v$，枚举 $u, v$ 中最长链的长度 $i, j$，有

1. $i + j \le k$ 时，此时可以选择不断开边 $(u, v)$，也可选择断开 $(u, v)$，有转移 $f_{u, \max\{i, j + 1\}} \leftarrow 2 \times f_{u, i} f_{v, j}$．
2. $i + j > k$ 时，此时只能选择断开 $(u, v)$，有转移 $f_{u, \max\{i, j + 1\}} \leftarrow f_{u, i} f_{v, j}$．

看似是 $\Theta(nk^2)$ 的，但事实上若枚举上界定为子树深度，那么可以分析出时间复杂度为 $O(nk)$．分析参考 [这篇博客](https://blog.csdn.net/lyd_7_29/article/details/79854245)．

## 代码

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#define debug(...) fprintf(stderr, __VA_ARGS__)

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

using i64 = long long;
const i64 P = 998244353;

const int N = 5e3;

int n, k; std::vector<int> g[N + 5];
i64 f[N + 5][N + 5]; int dep[N + 5];
void dfs(int u, int fa) {
	for(int v : g[u]) if(v != fa) dfs(v, u);

	static i64 h[N + 5];
	f[u][1] = 1, dep[u] = 1;
	for(int v : g[u]) {
		if(v == fa) continue;
		memset(h, 0, sizeof(h));
		for(int i = 1; i <= dep[u]; i++) {
			for(int j = 1; j <= dep[v]; j++) {
				if(i + j <= k) (h[std::max(i, j + 1)] += f[u][i] * f[v][j] % P) %= P;
				(h[i] += f[u][i] * f[v][j] % P) %= P;
			}
		}
		dep[u] = std::max(dep[u], dep[v] + 1);
		memcpy(f[u], h, sizeof(f[u]));
	}
}

int main() {
	n = rd(), k = rd() + 1;
	for(int i = 1; i < n; i++) {
		int u = rd(), v = rd();
		g[u].push_back(v), g[v].push_back(u);
	}

	dfs(1, 0);
	i64 ans = 0;
	for(int i = 1; i <= k; i++) (ans += f[1][i]) %= P;
	printf("%lld\n", ans);
	return 0;
}
```
