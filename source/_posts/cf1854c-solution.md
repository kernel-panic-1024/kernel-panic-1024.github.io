---
title: Codeforces 1854C Expected Destruction 题解
date: 2023-09-12 20:31:43
description:
categories:
---

预期的崩溃．

<!-- more -->

## 思路

若集合是可重的，那么答案就是 $\sum_i m + 1 - a_i$．考虑去掉由于插入重复数字导致集合元素减少对期望造成的贡献．

发现只有原序列中相邻的元素才可能造成这样的贡献，于是考虑枚举这样的元素对，除去贡献．

考虑 $(a_i, a_{i + 1})$ 的时候，直接设 $f_{x, y}$ 表示 $a_i$ 加了 $x$ 次，$a_j$ 加了 $y$ 次，转移显然．当 $x$ 和 $y$ 的差值等于 $a_{i + 1} - a_i$ 时，就直接贡献到答案上．

## 代码

```cpp
#include <cstdio>
#include <algorithm>
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

using i64 = long long;
const i64 P = 1e9 + 7;
const i64 i2 = 500000004;

const int N = 1e3;

int n, m, a[N + 5];
i64 f[N + 5][N + 5];

int main() {
	n = rd(), m = rd();
	for(int i = 1; i <= n; i++) a[i] = rd();

	i64 ans = 0;
	for(int i = 1; i <= n; i++) (ans += (m + 1 - a[i])) %= P;
	for(int i = 1; i < n; i++) {
		memset(f, 0, sizeof(f)), f[0][0] = 1;
		int tot = (m + 1 - a[i]) + (m + 1 - a[i + 1]);
		for(int x = 0; x <= m + 1 - a[i] && x <= tot; x++) {
			for(int y = 0; x + y <= tot; y++) {
				if(x - y == a[i + 1] - a[i]) {
					(ans += P - (m + 1 - a[i] - x) * f[x][y] % P) %= P;
				} else {
					(f[x][y + 1] += f[x][y] * i2 % P) %= P;
					(f[x + 1][y] += f[x][y] * i2 % P) %= P;
				}
			}
		}
	}
	printf("%lld\n", ans);
	return 0;
}
```

## 参考

wzj_zhzx_oicon, [_Solution CF1854C_](https://www.luogu.com.cn/blog/BreakPlus/solution-cf1854c)
