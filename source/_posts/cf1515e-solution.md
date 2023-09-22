---
title: Codeforces 1515E Phoenix and Computers 题解
date: 2023-09-22 21:54:20
categories: 题解
---

<!-- more -->

## 思路

设 $f_{i, j}$ 表示已经打开了 $i$ 台电脑，已经打开的电脑构成了 $j$ 个连续段的方案数．

考虑对连续段操作：

1. 延长连续段．可以选择在某个连续段两边插入，或间隔一个插入，第二种操作会导致中间的那个未被打开的电脑被打开，有 $2j \times f_{i, j} \to f_{i + 1, j}$，$2j \times f_{i, j} \to f_{i + 2, j}$，

2. 合并连续段．间隔两个格子的连续段可以通过任意打开一台电脑来合并，有 $2(j - 1) \times f_{i, j} \to f_{i + 2, j - 1}$；间隔三个格子的连续段可以通过打开中间的一台电脑合并，有 $(j - 1) \times f_{i, j} \to {i + 3, j - 1}$．

3. 新建连续段．$(j + 1) \times f_{i, j} \to f_{i + 1, j + 1}$．

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

const int N = 400;

int n;
i64 P, f[N + 5][N + 5];

int main() {
	n = rd(), P = rd();
	f[0][0] = 1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j <= i; j++) {
			if (i + 1 <= n) (f[i + 1][j] += 2 * j * f[i][j] % P) %= P;
			if (i + 2 <= n) (f[i + 2][j] += 2 * j * f[i][j] % P) %= P;
			if (0 < j - 1 && i + 2 <= n) (f[i + 2][j - 1] += 2 * (j - 1) * f[i][j] % P) %= P;
			if (0 < j - 1 && i + 3 <= n) (f[i + 3][j - 1] += (j - 1) * f[i][j] % P) %= P;
			(f[i + 1][j + 1] += f[i][j] * (j + 1) % P) %= P;
		}
	}
	printf("%lld\n", f[n][1]);
	return 0;
}
```
