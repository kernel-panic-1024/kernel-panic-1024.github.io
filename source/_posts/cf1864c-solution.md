---
title: Codeforces 1864C Divisor Chain 题解
date: 2023-08-28 14:41:43
categories: 题解
tags: 构造
---

被 C 暴打了．/jk

constructive algorithms．/tuu

<!-- more -->

## 思路

反过来思考整个过程，我们尝试从 $1$ 开始，给当前数加上自己的一个因数来凑出来 $x$．

可以发现 $2$ 的整数次幂是容易凑出的，因为任何数都是自己的因数，直接倍增即可．

如果我们凑出了 $x$ 二进制表示下的最高位，那么 $x$ 的其他位都是那个数的因数，而且都只被用了一次，那么直接凑上即可．

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

const int N = 1e3;

int x;
int opt[N + 5], tp;
void solve() {
	x = rd();
	int mxp = std::__lg(x);
	tp = 0;
	for(int i = 0; i < mxp; i++) {
		opt[++tp] = (1 << i);
	}
	for(int i = mxp - 1; i >= 0; i--) {
		if(x & (1 << i)) opt[++tp] = (1 << i);
	}
	printf("%d\n", tp + 1);
	while(tp) {
		printf("%d ", x);
		x -= opt[tp--];
	}
	printf("%d\n", x);
}

int main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
