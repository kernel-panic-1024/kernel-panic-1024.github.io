---
title: Codeforces 1278F Cards 题解
date: 2023-05-22 23:15:19
categories: 题解
tags:
  - 组合数学
  - 第二类 Stirling 数
---

．

<!-- more -->

## 思路

由于洗牌时排列等概率生成，那么每次牌堆顶是王牌的概率为 $\dfrac{1}{m}$，且每次洗牌互不影响．

设 $p = \dfrac{1}{m}$，由概率相关知识可得，$x \sim B(n, p)$．$x^k$ 的期望为：

$$
E(x^k) = \sum_{i = 0}^n \binom{n}{i} p^i (1 - p)^{n - i} i^k
$$

联想到 「联合省选 2020 A」组合数问题 中的技巧，我们将 $x^k$ 展开成下降幂，化简得到：

$$
\begin{aligned}
  E(x^k)
  &= \sum_{i = 0}^n \binom{n}{i} p^i (1 - p)^{n - i} \sum_{j = 0}^k {k \brace j} i^{\underline{j}} \\
  &= \sum_{j = 0}^k {k \brace j} \sum_{i = 0}^n \binom{n}{i} i^{\underline{j}} p^i (1 - p)^{n - i} \\
  &= \sum_{j = 0}^k {k \brace j} \sum_{i = 0}^n \binom{n - j}{i - j} n^{\underline{j}} p^i (1 - p)^{n - i} \\
  &= \sum_{j = 0}^k {k \brace j} n^{\underline{j}} \sum_{i = 0}^n \binom{n - j}{i - j}  p^i (1 - p)^{n - i} \\
  &= \sum_{j = 0}^k {k \brace j} n^{\underline{j}} \sum_{t = 0}^{n - j} \binom{n - j}{t}  p^{t + j} (1 - p)^{n - j - t} \\
  &= \sum_{j = 0}^k {k \brace j} n^{\underline{j}} p^j \sum_{t = 0}^{n - j} \binom{n - j}{t}  p^t (1 - p)^{n - j - t} \\
  &= \sum_{j = 0}^k {k \brace j} n^{\underline{j}} p^j (p + 1 - p)^n \\
  &= \sum_{j = 0}^k {k \brace j} n^{\underline{j}} p^j
\end{aligned}
$$

$O(k^2)$ 递推第二类 Stirling 数后即可 $O(k^2)$ 计算答案．

## 代码

```cpp
#include <cstdio>

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

using i64 = long long;

const int N = 5e3;
const i64 P = 998244353;
i64 fpow(i64 b, i64 p) {
	i64 res = 1;
	for (; p; b = b * b % P, p >>= 1) {
		if (p & 1) res = res * b % P;
	}
	return res;
}

int n, m, k;
i64 S[N + 5][N + 5];

int main() {
	n = rd(), m = rd(), k = rd();

	S[0][0] = 1;
	for (int i = 1; i <= k; i++) {
		for (int j = 1; j <= k; j++) {
			S[i][j] = (S[i - 1][j - 1] + j * S[i - 1][j] % P) % P;
		}
	}

	i64 p = fpow(m, P - 2);
	i64 ans = 0;
	for (int i = 0; i <= k; i++) {
		i64 nj = 1;
		for (i64 j = n; j >= n - i + 1; j--) (nj *= j) %= P;
		(ans += S[k][i] * nj % P * fpow(p, i) % P) %= P;
	}

	printf("%lld\n", ans);
	return 0;
}
```
