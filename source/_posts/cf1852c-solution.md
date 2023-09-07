---
title: Codeforces 1852C Ina of the Mountain 题解
date: 2023-09-07 14:44:51
categories: 题解
tags: 贪心
---

看皮套人看的．

<!-- more -->

## 思路

即模 $k$ 意义下全都变成 $0$．如果不模 $k$，这不是我们积木大赛吗！若记 $b_i = a_i - a_{i - 1}$，那么答案就是 $\sum_i [b_i > 0]b_i$．

模 $k$ 可以看做预先给 $a$ 某些位置加 $k$，反映在差分数组上就是选择若干个二元组 $(i, j)$，其中 $i < j$，然后给 $b_i$ 加 $k$，给 $b_j$ 减 $k$．

于是问题转化为进行任意次上述操作，最小化 $\sum_i [b_i > 0]b_i$．

显然我们不会对一个正数加，也不会对一个负数减．于是我们可以从前往后考虑，每遇到正数，就找前面第一个没有使用过的、最小的负数，若使用后更优就直接用．由于放着最小值不用没有收益，所以正确性显然．

## 代码

```cpp
#include <cstdio>
#include <queue>
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
int b[N + 5];
void solve() {
	n = rd(), k = rd(); for(int i = 1; i <= n; i++) a[i] = rd() % k;
	for(int i = 1; i <= n; i++) b[i] = a[i] - a[i - 1];
	std::priority_queue<int, std::vector<int>, std::greater<int>> q;
	int ans = 0;
	for(int i = 1; i <= n; i++) {
		if(b[i] < 0) q.push(b[i]);
		else {
			if(q.size()) {
				int p = q.top();
				if(p + k >= b[i]) ans += b[i];
				else q.pop(), ans += p + k, q.push(b[i] - k);
			} else ans += b[i];
		}
	}
	printf("%lld\n", ans);
}

signed main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
