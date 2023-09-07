---
title: Codeforces 1848F Vika and Wiki 题解
date: 2023-09-07 16:40:51
categories: 题解
---

你说的对，但是组合数是一个由 Blaise Pascal 独立发现的开放世界冒险数学概念．

<!-- more -->

## 思路

打个表先．$k$ 次操作后，$j$ 位置上的数会被异或到 $i$ 位置的次数是 $\binom{i}{j - i}$．

那么，第 $2^k$ 次操作后，$a_i$ 会变成 $a_i \oplus a_{i + 2^k \bmod n}$．注意到操作 $n$ 次后会变成 $0$，也就是说肯定存在解．

那么直接考虑从高位到低位确定答案，能填 $0$ 就填 $0$．

## 代码

```cpp
#include <cstdio>
#include <cassert>
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

const int N = 1 << 20;

int n, a[N], b[N];

int main() {
	n = rd(); for(int i = 0; i < n; i++) a[i] = rd();
	bool flg = true; for(int i = 0; i < n; i++) if(a[i]) flg = false;
	if(flg) return puts("0"), 0;
	int mxpw = std::__lg(n), ans = 0;
	for(int p = mxpw - 1; p >= 0; p--) {
		bool flg = true;
		for(int i = 0; i < n; i++) {
			b[i] = a[i] ^ a[(i + (1 << p)) % n];
			if(b[i]) flg = false;
		}
		if(!flg) ans |= 1 << p, memcpy(a, b, sizeof(a));
	}
	printf("%d\n", ans + 1);
	return 0;
}
```
