---
title: Codeforces 1852A Ntarsis' Set 题解
date: 2023-09-06 22:01:57
categories: 题解
tags: 二分
---

learn how to use binary search．

<!-- more -->

## 思路

考虑二分最大的 $p$，使得 $[1, p]$ 都能在 $k$ 轮操作之后被删掉，答案显然就是 $p + 1$．

判定答案时，我们不妨换一下对删数操作的定义．若当前数的排名是 $x$，我们直接将删数操作定义为 $x \leftarrow x - \sum_i [a_i \le x]$．按照这个定义在没有被删除的元素上操作的结果和原定义是吻合的，而在被删除的元素上则带来了一些比原定义更好的性质．若存在某个 $a_i = x$，我们考察进行这个操作后会发生什么：

1. $[1, x]$ 中的元素被删空，此时 $\forall j \le i, a_j = j$，即本次删了一个前缀，操作一次之后 $x = 0$，正好对应删空．
2. $[1, x]$ 中的元素未被删空，此时 $\sum_i [a_i \le x] \not= x$，操作后 $x \not= 0$，正好对应没删空．

那么判定就直接暴力进行 $k$ 轮操作，判定 $x = 0$ 即可．

## 代码

```cpp
#include <cstdio>
#include <algorithm>
#define debug(...) fprintf(stderr, __VA_ARGS__)

#define int long long

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

const int N = 2e5;

int n, k, a[N + 5];

bool check(int x) {
	for(int i = 1; i <= k; i++) {
		int pos = std::upper_bound(a + 1, a + 1 + n, x) - a - 1;
		x -= pos;
	}
	return x == 0;
}

void solve() {
	n = rd(), k = rd(); for(int i = 1; i <= n; i++) a[i] = rd();
	if(a[1] != 1) return puts("1"), void();
	int l = 1, r = 1e18, ans = 1e18 + 1;
	while(l <= r) {
		int mid = (l + r) / 2;
		if(check(mid)) l = mid + 1, ans = mid;
		else r = mid - 1;
	}
	printf("%lld\n", ans + 1);
}

signed main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
