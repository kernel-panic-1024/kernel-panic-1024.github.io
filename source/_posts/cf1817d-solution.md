---
title: Codeforces 1817D Toy Machine 题解
date: 2023-09-05 15:09:22
categories: 题解
---

Ad-hoc．/qd

组题人我谢谢你．

<!-- more -->

## 思路

容易 ~~通过瞎摁~~ 观察到，当 $k \le \lceil (n - 2) / 2 \rceil$ 时，可以通过 $k - 1$ 次 $\texttt{LDRU}$ 加上一次 $\texttt{L}$ 将 $k$ 转到最左边．

考虑 $k > \lceil (n - 2) / 2 \rceil$ 时怎么做，我们可以通过镜像上面的操作将目标点转到右上角，然后重复最多 $\lfloor n / 2 \rfloor$ 次 $\texttt{RDRU}$ 将所有的块堆到右上角，此时进行 $\texttt{RU}$ 操作就转化成了 $k = \lceil (n - 2) / 2 \rceil$ 的情况，直接做即可．

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

int n, k;

int main() {
	n = rd(), k = rd();
	int mid = n / 2;
	if(k <= mid) {
		for(int i = 1; i <= k - 1; i++) printf("LDRU");
		puts("L");
	} else {
		int t = n - k - 1;
		for(int i = 1; i <= t - 1; i++) printf("RDLU");
		printf("R");
		for(int i = 1; i <= mid; i++) printf("RDRU");
		printf("LU");
		for(int i = 1; i <= mid - 1; i++) printf("LDRU");
		puts("L");
	}
	return 0;
}
```
