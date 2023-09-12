---
title: Codeforces 1846E Rudolf and Snowflakes 题解
date: 2023-09-12 20:56:09
categories: 题解
---

比不了．

<!-- more -->

## 思路

即确定是否存在 $k$ 和 $n$ 满足 $\sum\limits_{i = 0}^{n} k^i = x$，其中 $k \ge 2, n \ge 2$．

考虑等比求和公式，上式等于 $(k^{n + 1} - 1) / (k - 1)$．顶上那个指数的可能数量是 $\log$ 级别的，于是考虑枚举 $n$，二分 $k$，最后检验是否合法就行．

注意精度和炸 `long long`．

## 代码

```cpp
#include <cmath>
#include <cstdio>
#define debug(...) fprintf(stderr, __VA_ARGS__)

using i64 = long long;

inline i64 rd() {
	i64 x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

bool check(int k, int n, i64 x) {
	__int128_t kn = 1;
	for (int i = 1; i <= n; i++) kn *= k;
	return (kn - 1) / (k - 1) <= x;
}

i64 fpow(i64 b, i64 p, i64 P) {
	i64 res = 1;
	for (; p; b = b * b % P, p >>= 1) {
		if (p & 1) res = res * b % P;
	}
	return res;
}

i64 calc(i64 k, i64 n, i64 P) {
	return (fpow(k, n, P) + P - 1) % P * fpow(k - 1, P - 2, P) % P;
}

bool trysolve(i64 x, int n) {
	int l = 2, r = (int)pow(x, 1. / (n - 1)) + 1, ans = 1;
	while (l <= r) {
		int mid = (l + r) / 2;
		if (check(mid, n, x)) l = mid + 1, ans = mid;
		else r = mid - 1;
	}
	return calc(ans, n, 998244353) == x % 998244353 && calc(ans, n, 1000000007) == x % 1000000007;
}

void solve() {
	i64 n = rd();
	for (int i = 3; i <= 63; i++) {
		if (trysolve(n, i)) return puts("YES"), void();
	}
	puts("NO");
}

int main() {
	int T = rd();
	while (T--) solve();
	return 0;
}
```
