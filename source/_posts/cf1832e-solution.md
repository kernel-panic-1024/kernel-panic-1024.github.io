---
title: Codeforces 1832E Combinatorics Problem 题解
date: 2023-09-22 18:05:08
categories: 题解
---

<!-- more -->

## 思路

扰动一下：

$$
\begin{aligned}
  b_{i, k}
  &= \sum_{j = 1}^i \binom{i - j + 1}{k} a_j \\
  &= \sum_{j = 1}^i \left(\binom{i - j}{k} + \binom{i - j}{k - 1}\right) a_j \\
  &= \binom{0}{k} a_i + \binom{0}{k - 1} a_i + \sum_{j = 1}^{i - 1} \binom{i - j + 1}{k} a_j + \sum_{j = 1}^{i - 1} \binom{i - j + 1}{k - 1} a_j \\
  &= b_{i - 1, k} + b_{i - 1, k - 1} + [k - 1 = 0] a_i
\end{aligned}
$$

递推即可．

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
const i64 P = 998244353;

const int N = 1e7;
const int K = 5;

int n, k; i64 a[N + 5]; i64 b[N + 5][K + 1];

int main() {
	n = rd(), a[1] = rd();
	i64 x = rd(), y = rd(), m = rd();
	k = rd();

	for(int i = 2; i <= n; i++) {
		a[i] = (x * a[i - 1] % m + y) % m;
	}
	for(int i = 1; i <= n; i++) {
		b[i][0] = (b[i - 1][0] + a[i]) % P;
	}
	for(int i = 1; i <= n; i++) {
		for(int j = 1; j <= k; j++) {
			b[i][j] = (b[i - 1][j] + b[i - 1][j - 1]) % P;
			if(j - 1 == 0) (b[i][j] += a[i]) %= P;
		}
	}
	i64 ans = 0;
	for(int i = 1; i <= n; i++) {
		ans ^= b[i][k] * i;
	}
	printf("%lld\n", ans);
	return 0;
}
```
