---
title: Codeforces 1861E Non-Intersecting Subpermutations 题解
date: 2023-09-03 10:33:25
categories: 题解
tags:
  - 动态规划
---

被数数薄纱了．

不拆贡献，天诛地灭．

<!-- more -->

## 思路

首先观察到，对于一个特定的序列，我们可以贪心地去计算划分数，能划分就划分，不会影响答案．

于是考虑设 $f_{i, j}$ 表示考虑了前 $i$ 位，目前无重复数字的后缀长度为 $j$ 的序列个数．

每个满足要求的子串对答案的贡献为 $1$，故答案为

$$
\sum_{i = 1}^n f_{i, k} \times k^{n - i}
$$

考虑 $f_{i, j}$ 的转移：

1. 考虑在长度为 $j$ 的后缀后添加一个在该后缀中没有出现过的数字，转移有两种类型
    - 上次未凑出完整的无重复数字的子串：$f_{i, j} \leftarrow (k - j + 1) \times f_{i - 1, j - 1}, 2 \le j \le k$．
    - 上次凑出来了：$f_{i, 1} \leftarrow k \times f_{i - 1, k}$．
2. 如果添加了一个出现过的数字，由于我们考虑的是所有序列，所以会从后缀长度 $\ge j$ 的所有状态转移一次：$f_{i, j} \leftarrow f_{i - 1, p}$，$j \le p < k$．

显然可以使用前缀和优化．

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

const int N = 4e3;

using i64 = long long;
const i64 P = 998244353;

i64 fpow(i64 b, i64 p) {
	i64 res = 1;
	for (; p; b = b * b % P, p >>= 1) {
		if (p & 1) res = res * b % P;
	}
	return res;
}

int n, k;
i64 f[N + 5][N + 5], s[N + 5][N + 5];

int main() {
	n = rd(), k = rd();
	f[1][1] = s[1][1] = k;
	for (int i = 2; i <= n; i++) {
		for (int j = 1; j < k; j++) {
			(f[i][j] += s[i - 1][j]) %= P;
			(f[i][j] += f[i - 1][j - 1] * (k - j + 1) % P) %= P;
		}
		f[i][k] = f[i - 1][k - 1];
		(f[i][1] += f[i - 1][k] * k % P) %= P;
		for (int j = k - 1; j >= 1; j--) s[i][j] = (s[i][j + 1] + f[i][j]) % P;
	}
	i64 ans = 0;
	for (int i = 1; i <= n; i++) {
		(ans += f[i][k] * fpow(k, n - i) % P) %= P;
	}
	printf("%lld\n", ans);
	return 0;
}
```
