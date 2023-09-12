---
title: Codeforces 1840G1 In Search of Truth (Easy Version) 题解
date: 2023-09-12 21:20:47
categories: 题解
---

不像我这么菜．/kk

<!-- more -->

## 思路

即构造一个长度 $\le 2023$ 的序列 $a_i$，使得 $\forall i \le 10^6, \exists l, r, \sum\limits_{k = l}^r a_k = i$．

考虑光速幂的构造，设置块长 $B$，显然有 $n = \lfloor n / B \rfloor B + n \bmod B$．那么前面丢 $B$ 个 $1$，后面丢 $\lfloor 10^6 / B \rfloor$ 个 $B$．显然 $B = \sqrt{10^6} = 10^3$ 时最优．

## 代码

```cpp
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

const int N = 1e6;

int vis[N + 5];

int ask(int x) {
	printf("+ %d\n", x), fflush(stdout);
	return rd();
}

int main() {
	memset(vis, -1, sizeof(vis));
	vis[rd()] = 0;
	for (int i = 1; i <= 1000; i++) {
		int x = ask(1);
		if (vis[x] != -1) {
			printf("! %d\n", i - vis[x]), fflush(stdout);
			return 0;
		}
		vis[x] = i;
	}
	for (int i = 1; i <= 1000; i++) {
		int x = ask(1000);
		if (vis[x] != -1) {
			printf("! %d\n", (i + 1) * 1000 - vis[x]);
			return 0;
		}
		vis[x] = (i + 1) * 1000;
	}
	return 0;
}
```

## 参考

zltqwq, [_CF1840G1 题解_](https://www.luogu.com.cn/blog/275273/solution-cf1840g1)
