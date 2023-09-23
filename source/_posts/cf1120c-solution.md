---
title: Codeforces 1120C Compress String 题解
date: 2023-09-23 13:28:19
categories: 题解
tags:
  - 动态规划
  - 字符串
---

<!-- more -->

## 思路

设 $f_i$ 表示考虑了前 $i$ 个字符的最小代价，有转移

$$
\begin{aligned}
  f_i &\leftarrow f_{i - 1} + a \\
  f_i &\leftarrow f_{i - k} + b
\end{aligned}
$$

其中 $k$ 满足 $s[i - k + 1 : i]$ 是 $s[: i - k]$ 的子串．

这样决策点很难找，不妨直接枚举子串的结束位置 $j$，设 $\mathrm{lcs}_{i, j}$ 表示前缀 $s[: i]$ 和 $s[: j]$ 的最长公共后缀，有转移 $f_i \leftarrow f_{\max\{i - \mathrm{lcs}_{i, j}, j\}}$．这样就能做到 $\Theta(n)$ 转移了．

## 代码

```cpp
#include <algorithm>
#include <cstdio>
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

const int N = 5e3;

int n, a, b;
char s[N + 5];

int lcs[N + 5][N + 5];
int f[N + 5];

int main() {
	n = rd(), a = rd(), b = rd(), scanf("%s", s + 1);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (s[i] == s[j]) lcs[i][j] = lcs[i - 1][j - 1] + 1;
		}
	}
	memset(f, 63, sizeof(f)), f[0] = 0;
	for (int i = 1; i <= n; i++) {
		f[i] = f[i - 1] + a;
		for (int j = 1; j < i; j++) {
			f[i] = std::min(f[i], f[std::max(i - lcs[i][j], j)] + b);
		}
	}
	printf("%d\n", f[n]);
	return 0;
}
```
