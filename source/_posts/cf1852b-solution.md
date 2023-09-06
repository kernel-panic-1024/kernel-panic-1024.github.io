---
title: Codeforces 1852B Imbalanced Arrays 题解
date: 2023-09-07 07:38:57
categories: 题解
tags: 构造
---

构造．/oh

<!-- more -->

## 思路

首先观察到，如果同时存在 $a_i = n, a_j = 0$，那么一定无解，因为这要求 $b_i + b_j < 0$ 且 $b_i + b_j > 0$，显然矛盾．

考虑 $x = \argmax_i |b_i|$，那么 $a_x$ 只可能填 $0$ 或 $n$，具体填哪个取决于 $b_x$ 的符号．又由于 $-n \le b_i \le n, b_i \not= 0$，且不存在 $b_i + b_j = 0$，也就是说 $|b_i|$ 构成一个长度为 $n$ 的排列，所以如果 $a$ 中同时不存在 $0$ 或 $n$，那么也无解．

于是我们可以这样构造：从大到小分配 $n \sim 1$，每次找到 $a_i = 0$ 或 $a_i = n$，设当前最大没有分配的数为 $\mathrm{cur}$，若 $a_i = 0$，则 $b_i = -\mathrm{cur}$，否则 $b_i = \mathrm{cur}$．

显然可以在对 $a$ 排序后利用双指针优化．

## 代码

```cpp
#include <cstdio>
#include <utility>
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

const int N = 1e5;

int n; struct pii { int v, pos; } a[N + 5];
int ans[N + 5];
void solve() {
	n = rd(); for(int i = 1; i <= n; i++) a[i] = {rd(), i};
	std::sort(a + 1, a + 1 + n, [](pii a, pii b) { return a.v > b.v; });
	int i = 1, j = n, cur = n;
	while(i <= j) {
		if((a[i].v == j) + (a[j].v == i - 1) == 1) {
			if(a[i].v == j) {
				ans[a[i].pos] = cur;
				cur--, i++;
			} else {
				ans[a[j].pos] = -cur;
				cur--, j--;
			}
		} else {
			return puts("NO"), void();
		}
	}
	puts("YES");
	for(int i = 1; i <= n; i++) {
		printf("%d%c", ans[i], " \n"[i == n]);
	}
}

int main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
