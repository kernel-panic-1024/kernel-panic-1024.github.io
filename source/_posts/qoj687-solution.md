---
title: QOJ 687 Topological Ordering 题解
date: 2023-08-29 22:39:50
categories: 题解
tags: 动态规划
---

我是傻逼．

<!-- more -->

## 思路

考虑枚举 $u$，对其他所有 $v$ 计算 $v$ 在 $u$ 之后加入拓扑序的方案数．

设 $f_S$ 表示已经加入了集合 $S$ 中点的拓扑序数量，$g_S$ 表示从全集开始删到只剩 $S$ 中点的拓扑序数量．这个可以通过简单的 DP 得到．

计算 $\mathrm{ans}_{u, v}$ 时，枚举加入 $v$ 前的点集，我们有：

$$
\mathrm{ans}_{u, v} = \sum_{u \in S, v \not\in S} f_S \times g_{S \cup \{v\}}
$$

时间复杂度 $O(2^n n^2)$，卡卡常就过去了．

## 代码

```cpp
#include <cstdio>
#include <vector>
#define debug(...) fprintf(stderr, __VA_ARGS__)

#define int long long

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

const int N = 20;

int n, m; int nxt[N], pre[N];
int f[1 << N], g[1 << N];
void solve() {
	n = rd(), m = rd();
	for(int i = 1; i <= m; i++) {
		int u = rd() - 1, v = rd() - 1;
		nxt[u] |= 1 << v, pre[v] |= 1 << u;
	}
	int U = (1 << n) - 1;
	f[0] = 1;
	for(int S = 0; S <= U; S++) {
		for(int i = 0; i < n; i++) {
			if((S & (1 << i)) || (S & pre[i]) != pre[i]) continue;
			f[S | (1 << i)] += f[S];
		}
	}
	g[U] = 1;
	for(int S = U; S >= 0; S--) {
		for(int i = 0; i < n; i++) {
			if(!(S & (1 << i)) || (S & nxt[i])) continue;
			g[S ^ (1 << i)] += g[S];
		}
	}
	for(int u = 0; u < n; u++) {
		for(int v = 0; v < n; v++) {
			if(u == v) printf("0");
			else {
				int ans = 0, msk = U ^ (1 << u) ^ (1 << v);
				for(int S = msk;; S = (S - 1) & msk) {
					if((S & pre[v]) == pre[v]) ans += f[S] * g[S | (1 << v)];
					if(!S) break;
				}
				printf("%lld", ans);
			}
			printf("%c", " \n"[v == n - 1]);
		}
	}

	for(int i = 0; i <= U; i++) f[i] = g[i] = 0;
	for(int i = 0; i < n; i++) nxt[i] = pre[i] = 0;
}

signed main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
