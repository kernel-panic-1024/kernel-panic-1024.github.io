---
title: Codeforces 1839D Ball Sorting 题解
date: 2023-09-12 21:36:51
categories: 题解
---

彻底腐烂！

<!-- more -->

## 思路

我们的策略是选出一个上升子序列，这个上升子序列将原序列划分成了若干部分，然后每个部分放一个白球，进行消除．

于是设 $f_{i, j}$ 表示考虑了 $[1, i]$ 这个前缀，$i$ 在上升子序列里，被划分出了 $j$ 部分，上升子序列的最长长度．

若 $a_{i - 1} < a_i$，那么可以直接继承 $f_{i - 1, j}$．否则直接转移．

注意答案要做前缀 $\min$．

## 代码

```cpp
#include <cstdio>
#include <algorithm>
#define debug(...) fprintf(stderr, __VA_ARGS__)

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

const int N = 500;

int n, a[N + 5];
int f[N + 5][N + 5];

void solve() {
	n = rd(); for(int i = 1; i <= n; i++) a[i] = rd();
	for(int i = 0; i <= n; i++) for(int j = 0; j <= n; j++) f[i][j] = -1e9;
	f[0][0] = 0;
	for(int i = 1; i <= n; i++) {
		for(int j = 0; j <= i; j++) {
			if(a[i - 1] < a[i]) f[i][j] = f[i - 1][j] + 1;
			if(j) {
				for(int k = 0; k < i - 1; k++) {
					if(a[k] < a[i]) f[i][j] = std::max(f[i][j], f[k][j - 1] + 1);
				}
			}
		}
	}
	int ans = f[n][0];
	for(int k = 1; k <= n; k++) {
		ans = std::max(ans, f[n][k]);
		for(int i = 1; i <= n; i++) {
			ans = std::max(ans, f[i][k - 1]);
		}
		printf("%d%c", n - ans, " \n"[k == n]);
	}
}

int main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
