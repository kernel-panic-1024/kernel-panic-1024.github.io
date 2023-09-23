---
title: Codeforces 1811G Vlad and the Nice Paths 题解
date: 2023-09-23 14:39:46
categories: 题解
tags:
  - 动态规划
  - 组合数学
---

怄火．

<!-- more -->

## 思路

先考虑如何求出最长长度．

设 $f_i$ 表示以 $i$ 结束的好路径的最长长度是多少，转移枚举之前的位置 $j$，其中 $j$ 满足区间 $[j, i]$ 中 $a_i$ 出现的次数 $\ge k$．

考虑如何计数，设区间 $[j, i]$ 中 $a_i$ 的出现次数为 $\mathrm{ct}$，由于钦定选 $a_i$，那么次数的贡献系数就是 $\binom{\mathrm{ct} - 1}{k - 1}$．

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

using i64 = long long;
const i64 P = 1e9 + 7;

const int N = 5e3;

int n, k, a[N + 5];
int f[N + 5];

i64 C[N + 5][N + 5];
void pre() {
	for(int i = 0; i <= N; i++) C[i][0] = 1;
	for(int i = 1; i <= N; i++) {
		for(int j = 1; j <= N; j++) {
			C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % P;
		}
	}
}

i64 g[N + 5];

void solve() {
	n = rd(), k = rd();
	for(int i = 1; i <= n; i++) a[i] = rd();
	g[0] = 1;
	for(int i = 1; i <= n; i++) {
		int ct = 1;
		for(int j = i - 1; j >= 0; j--) {
			if(ct >= k) {
				if(f[j] + k == f[i]) (g[i] += g[j] * C[ct - 1][k - 1] % P) %= P;
				else if(f[j] + k > f[i]) f[i] = f[j] + k, g[i] = g[j] * C[ct - 1][k - 1] % P;
			}
			ct += (a[j] == a[i]);
		}
	}
	int mx = 0; i64 ans = 0;
	for(int i = 0; i <= n; i++) {
		if(f[i] > mx) mx = f[i], ans = g[i];
		else if(f[i] == mx) (ans += g[i]) %= P;
	}
	printf("%lld\n", ans);
	for(int i = 0; i <= n; i++) f[i] = g[i] = 0;
}

int main() {
	pre();
	int T = rd();
	while(T--) solve();
	return 0;
}
```
