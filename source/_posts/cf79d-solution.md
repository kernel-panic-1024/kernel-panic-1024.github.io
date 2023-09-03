---
title: Codeforces 79D Password 题解
date: 2023-09-03 10:20:14
categories: 题解
tags:
  - 动态规划
  - 差分
---

见过了 trick 还做不出来．

要退役了．

<!-- more -->

## 思路

考虑原序列的差分序列，题意转化为：每次改变 $x$ 和 $x + a_i$ 的状态，将有 $\le 2k$ 个 $1$ 的序列变成全 $0$ 的．

首先可以通过 $n$ 次最短路计算出改变任意两个位置状态的代价．由于边权为 $1$，01 bfs 即可．

然后就直接设 $f_{S}$ 表示将 $S$ 内的位置全变成 $0$ 的最小代价，枚举仍不是 $0$ 的位置，转移即可．

## 代码

```cpp
#include <cstdio>
#include <cstring>
#include <queue>
#define debug(...) fprintf(stderr, __VA_ARGS__)

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

const int N = 10000;

void chmin(int &x, int y) { if(x > y) x = y; }

int n, k, m, a[N + 5], len[N + 5];

int dis[21][N + 5];
int f[1 << 20];

int main() {
	n = rd(), k = rd(), m = rd();
	for(int i = 1; i <= k; i++) a[rd()] = 1;
	for(int i = 1; i <= m; i++) len[i] = rd();
	for(int i = n + 1; i >= 1; i--) a[i] = ((a[i] - a[i - 1] + 2) & 1);
	static int b[N + 5]; int tp = 0;
	for(int i = 1; i <= n + 1; i++) if(a[i]) b[++tp] = i;
	for(int i = 1; i <= tp; i++) {
		memset(dis[i], -1, sizeof(dis[i]));
		std::queue<int> q; q.push(b[i]), dis[i][b[i]] = 0;
		while(q.size()) {
			int u = q.front(); q.pop();
			for(int j = 1; j <= m; j++) {
				{
					int v = u - len[j];
					if(1 <= v && dis[i][v] == -1) {
						dis[i][v] = dis[i][u] + 1;
						q.push(v);
					}
				}
				{
					int v = u + len[j];
					if(v <= n + 1 && dis[i][v] == -1) {
						dis[i][v] = dis[i][u] + 1;
						q.push(v);
					}
				}
			}
		}
	}
	memset(f, 63, sizeof(f)), f[0] = 0;
	for(int S = 0; S < (1 << tp); S++) {
		for(int i = 1; i <= tp; i++) {
			if(S & (1 << (i - 1))) continue;
			for(int j = 1; j <= tp; j++) {
				if(i == j) continue;
				if(S & (1 << (j - 1))) continue;
				if(dis[i][b[j]] == -1) continue;
				int T = S | (1 << (i - 1)) | (1 << (j - 1));
				chmin(f[T], f[S] + dis[i][b[j]]);
			}
		}
	}
	printf("%d\n", (f[(1 << tp) - 1] > 1e9) ? -1 : f[(1 << tp) - 1]);;
	return 0;
}
```
