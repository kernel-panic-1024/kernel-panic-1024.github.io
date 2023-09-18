---
title: Codeforces 735E Ostap and Tree 题解
date: 2023-09-18 21:38:57
categories: 题解
---

hard．

真的要退役了．

<!-- more -->

## 思路

设 $f_{u, i}$ 表示 $u$ 子树中，存在一个黑点与 $u$ 之间的距离为 $i$，且距离大于 $i$ 的点的限制均被满足的方案数．

考虑合并 $v$ 这棵子树，枚举 $i, j$，转移如下：

1. 若 $i + j \le 2k$，那么两条路径直接拼接不会导致有点不被满足，那么直接 $f_{u, i} \times f_{v, j} \to f_{u, \min\{i, j + 1\}}$．
2. 若 $i + j > 2k$，不妨设 $u$、$v$ 子树中达到 $i$、$j$ 限制的点分别为 $s$、$t$，那么 $s \to t$ 这条链上一定存在某些节点不被满足，此时有 $f_{u, i} \times f_{v, j} \to f_{u, \max\{i, j + 1\}}$．

答案即为 $\sum\limits_{i \le k} f_{1, i}$．

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
const i64 P = 1e9 + 7;

const int N = 100;
const int K = 50;

int n, k; std::vector<int> g[N + 5];

i64 f[N + 5][N + 5];
void dfs(int u, int fa) {
	for(int v : g[u]) if(v != fa) dfs(v, u);

	f[u][0] = f[u][k + 1] = 1;

	static i64 h[N + 5];
	for(int v : g[u]) {
		if(v == fa) continue;
		memset(h, 0, sizeof(h));
		for(int i = 0; i <= k * 2; i++) {
			for(int j = 0; j <= k * 2; j++) {
				if(i + j <= k * 2) {
					(h[std::min(i, j + 1)] += f[u][i] * f[v][j] % P) %= P;
				} else {
					(h[std::max(i, j + 1)] += f[u][i] * f[v][j] % P) %= P;
				}
			}
		}
		memcpy(f[u], h, sizeof(f[u]));
	}
}

int main() {
	n = rd(), k = rd();
	for(int i = 1; i < n; i++) {
		int u = rd(), v = rd();
		g[u].push_back(v), g[v].push_back(u);
	}
	dfs(1, 0);

	i64 ans = 0;
	for(int i = 0; i <= k; i++) (ans += f[1][i]) %= P;
	printf("%lld\n", ans);
	return 0;
}
```

## 参考

skylee, [_题解 CF735E 【Ostap and Tree】_](https://www.luogu.com.cn/blog/skylee/solution-cf735e)
