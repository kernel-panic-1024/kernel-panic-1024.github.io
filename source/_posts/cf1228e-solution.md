---
title: Codeforces 1228E Another Filling the Grid 题解
date: 2023-09-23 12:48:05
categories: 题解
tags:
  - 二项式反演
---

<!-- more -->

## 思路

设 $g_{i, j}$ 表示恰有 $i$ 行 $j$ 列最小值为 $1$ 的方案，$f_{i, j}$ 表示钦定 $i$ 行 $j$ 列，使得剩下的行列最小值不为 $1$ 的方案数，有

$$
f_{n, n} = \sum_{i = 0}^n \sum_{j = 0}^n \binom{n}{i} \binom{n}{j} g_{i, j} \iff g_{n, n} = \sum_{i = 0}^n \sum_{j = 0}^n \binom{n}{i} \binom{n}{j} (-1)^{n - i + n - j} f_{i, j}
$$

其中 $f_{i, j}$ 显然为 $(k - 1)^{n^2 - (n - i)(n - j)} \times k^{(n - i)(n - j)}$．

可以直接计算．

## 代码

```cpp
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

using i64 = long long;
const i64 P = 1e9 + 7;

i64 fpow(i64 b, i64 p) {
	i64 res = 1;
	for (; p; b = b * b % P, p >>= 1) {
		if (p & 1) res = res * b % P;
	}
	return res;
}

const int N = 250;

int n;
i64 k;

i64 C[N + 5][N + 5];
void pre() {
	for (int i = 0; i <= N; i++) C[i][0] = 1;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % P;
		}
	}
}

int main() {
	pre();

	n = rd(), k = rd();

	i64 ans = 0;
	for (int i = 0; i <= n; i++) {
		for (int j = 0; j <= n; j++) {
			i64 coef = C[n][i] * C[n][j] % P * fpow(k, (n - i) * (n - j)) % P * fpow(k - 1, n * n - (n - i) * (n - j)) % P;
			if ((n * 2 - i - j) & 1) (ans += P - coef) %= P;
			else (ans += coef) %= P;
		}
	}
	printf("%lld\n", ans);
	return 0;
}
```
