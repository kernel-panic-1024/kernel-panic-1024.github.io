---
title: 洛谷 9575 「TAOI-2」喵了个喵 Ⅳ 题解
date: 2023-09-05 13:34:02
categories: 题解
tags:
  - 构造
---

喵了个喵．/qd

申必构造题．

<!-- more -->

## 思路

首先 $n$ 是偶数的情况是容易的，$x$ 取 $1$ 后直接均分即可．

然后讨论 $n$ 为奇数的情况．将序列中所有数除去它们的 $\gcd$，不妨记 $\gcd$ 为 $g$，我们直接取 $x = 2g$，这样除去 $g$ 后的序列中与 $x$ 的 $\gcd$ 就只有 $1$ 和 $2$ 两种，不妨记个数分别为 $c_1, c_2$．

1. 当 $c_1$ 为奇数时，$c_2$ 为偶数，此时无解．因为 $c_1 + 2c_2$ 此时为奇数，无法均分．
2. 当 $c_1$ 为偶数时，$c_2$ 为奇数，瞎搞均分下即可．

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

const int N = 1e5;

int n, a[N + 5], flg[N + 5];

int main() {
	n = rd(); for(int i = 1; i <= n; i++) a[i] = rd();
	if(n % 2 == 0) {
		puts("1");
		for(int i = 1; i <= n / 2; i++) printf("1");
		for(int i = 1; i <= n / 2; i++) printf("0");
		puts("");
		return 0;
	}
	int gcd = a[1];
	for(int i = 2; i <= n; i++) gcd = std::__gcd(a[i], gcd);
	for(int i = 1; i <= n; i++) a[i] /= gcd;
	int ct1 = 0;
	for(int i = 1; i <= n; i++) ct1 += a[i] & 1;
	if(ct1 & 1) return puts("-1"), 0;
	int ct2 = n - ct1;
	int p = ct1 / 2 - 1, q = (ct2 + 1) / 2;
	for(int i = 1; i <= n; i++) {
		if(a[i] & 1) {
			if(p) flg[i] = 1, p--;
		} else {
			if(q) flg[i] = 1, q--;
		}
	}
	printf("%d\n", gcd * 2);
	for(int i = 1; i <= n; i++) printf("%d", flg[i]);
	puts("");
	return 0;
}
```
