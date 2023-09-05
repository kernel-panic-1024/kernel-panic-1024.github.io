---
title: Codeforces 1764E Doremy's Number Line 题解
date: 2023-09-05 14:49:21
categories: 题解
tags: 贪心
---

greedy．/tuu

<!-- more -->

## 思路

若 $k \le a_1$，那么可以直接涂掉；若 $k > a_1 + b_1$，那么不可能涂掉．我们只需要解决 $a_1 < k \le a_1 + b_1$ 的情况．

$1$ 显然要拿出来涂 $k$，所以先踢掉它．事实上，我们只需要判定抛开 $1$ 后，剩下的操作能涂到的最大位置是否 $\ge k - b_1$ 即可．

不妨设 $p = \argmax\limits_i a_i$，分类讨论其对能取到的最大右端点 $r$ 的贡献．

1. 不由 $p$ 取到，此时其他点都可以任意被涂了，所以 $r = \max_{i \not= p} a_i + b_i$．
2. 由 $p$ 取到，此时 $p$ 可以扩展剩余 $n - 2$ 个位置取到的最大值 $r^\prime$．

那么按照 $a_i$ 排序后取一遍 $\max$ 即可．

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

const int N = 1e5;

int n, k; struct node { int a, b; } a[N + 5];
void solve() {
	n = rd(), k = rd();
	for(int i = 1; i <= n; i++) a[i] = {rd(), rd()};

	if(k <= a[1].a) return puts("YES"), void();
	if(k > a[1].a + a[1].b) return puts("NO"), void();
	if(n == 1) return puts("NO"), void();

	std::sort(a + 2, a + 1 + n, [](node a, node b) {
		return a.a != b.a ? a.a < b.a : a.b < b.b;
	});
	int bound = k - a[1].b;
	int mx = a[2].a, t = a[2].a + a[2].b;
	if(mx >= bound) return puts("YES"), void();
	for(int i = 3; i <= n; i++) {
		t = std::max(t, a[i - 1].a + a[i - 1].b);
		mx = std::max({a[i].a, std::min(mx, a[i].a) + a[i].b, t});
		if(mx >= bound) return puts("YES"), void();
	}
	puts("NO");
}

signed main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
