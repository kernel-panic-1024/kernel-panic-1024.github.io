---
title: Codeforces 1858D Trees and Segments 题解
date: 2023-08-31 21:25:36
categories: 题解
---

为什么大家都这么强大？

<!-- more -->

## 思路

不妨设 $f_i$ 表示 $0$ 最长连续段长度为 $i$ 时，最长的 $1$ 连续段的长度．那么对于一个  $x$，答案即为 $\max\limits_i \{x \times i + f_i\}$．

考虑如何求出 $f_i$，首先我们可以钦定一个连续段 $[l, r]$ 为 $0$，不妨设需要的修改次数为 $\mathrm{ct}$，那么我们要求的就是 $[1, l - 1]$ 这个前缀和 $[r + 1, n]$ 这个后缀中，修改 $k - \mathrm{ct}$ 次能够得到的最长 $1$ 连续段，这个也可以简单 DP 求出．

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

const int N = 3e3;

int n, k, a[N + 5];

int s[N + 5];
int pre[N + 5][N + 5], suf[N + 5][N + 5];
int f[N + 5];

char buf[N + 5];
void solve() {
	n = rd(), k = rd(), scanf("%s", buf + 1);
	for (int i = 1; i <= n; i++) a[i] = buf[i] - '0';

	for (int i = 1; i <= n; i++) s[i] = s[i - 1] + (!a[i]);
	for (int i = 1; i <= n; i++) {
		for (int j = 0; j <= n; j++) pre[i][j] = pre[i - 1][j];
		for (int j = 1; j <= i; j++) {
			int ct = s[i] - s[j - 1];
			pre[i][ct] = std::max(pre[i][ct], i - j + 1);
		}
	}
	for (int i = n; i >= 1; i--) {
		for (int j = 0; j <= n; j++) suf[i][j] = suf[i + 1][j];
		for (int j = i; j <= n; j++) {
			int ct = s[j] - s[i - 1];
			suf[i][ct] = std::max(suf[i][ct], j - i + 1);
		}
	}
	memset(f, -63, sizeof(f)), f[0] = pre[n][k];
	for (int i = 1; i <= n; i++) {
		int ct = 0;
		for (int j = i; j <= n; j++) {
			ct += a[j];
			if (ct > k) break;
			f[j - i + 1] = std::max(f[j - i + 1], std::max(pre[i - 1][k - ct], suf[j + 1][k - ct]));
		}
	}
	for (int i = 1; i <= n; i++) {
		int mx = 0;
		for (int j = 0; j <= n; j++) {
			mx = std::max(mx, i * j + f[j]);
		}
		printf("%d%c", mx, " \n"[i == n]);
	}

	for (int i = 0; i <= n; i++) {
		for (int j = 0; j <= n; j++) {
			pre[i][j] = suf[i][j] = 0;
		}
	}
}

int main() {
	int T = rd();
	while (T--) solve();
	return 0;
}
```
