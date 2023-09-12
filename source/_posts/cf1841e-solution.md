---
title: Codeforces 1841E Fill the Matrix 题解
date: 2023-09-12 21:14:39
categories: 题解
---

十点半．/kk

<!-- more -->

## 思路

显然我们的策略是每次找到最长的横线填数，设长度为 $l$，那么贡献就是 $l - 1$．

考虑计算每种不同长度横线的个数，考虑一个位置 $i$，能作为以 $i$ 为右端点的横线的左端点只能是 $[1, i]$ 这段前缀中的后缀最大值，这个可以单调栈维护，然后在弹栈的时候记录一下前驱，就可以算数量了．

## 代码

```cpp
#include <cstdio>
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

int n, m, a[N + 5];
int ct[N + 5], st[N + 5];
void solve() {
	n = rd();
	for (int i = 1; i <= n; i++) a[i] = rd();
	m = rd();

	a[0] = a[n + 1] = n;
	int tp = 0;
	for (int i = 0; i <= n + 1; i++) {
		int p = -1;
		while (tp && a[i] >= a[st[tp]]) {
			if (p != -1) ct[i - st[tp] - 1] += a[st[tp]] - p;
			p = a[st[tp]], tp--;
		}
		if (tp && p != -1) ct[i - st[tp] - 1] += a[i] - p;
		st[++tp] = i;
	}

	int ans = 0;
	for (int i = n; i >= 1; i--) {
		if (m > 0) {
			if (m >= i * ct[i]) {
				m -= i * ct[i];
				ans += (i - 1) * ct[i];
			} else {
				int t = m / i, r = m % i;
				ans += (i - 1) * t;
				if (r) ans += r - 1;
				m = 0;
			}
		} else break;
	}
	printf("%lld\n", ans);

	for (int i = 1; i <= n; i++) ct[i] = 0;
}

signed main() {
	int T = rd();
	while (T--) solve();
	return 0;
}
```
