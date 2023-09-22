---
title: Codeforces 1830B The BOSS Can Count Pairs 题解
date: 2023-09-22 18:43:29
categories: 题解
tags: 根号分治
---

<!-- more -->

## 思路

考虑 $a_i a_j = b_i + b_j \le 2n$，那么考虑根号分治．

枚举 $k \le \sqrt{2n}$，不妨设 $a_i = k$，那么有 $k a_j = b_i + b_j \iff k a_j - b_j = b_i$，拿个桶计数就行了．$a_j = k$ 时由于钦定 $i < j$，需要单独拿出来扫一遍．

## 代码

```cpp
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#define debug(...) fprintf(stderr, __VA_ARGS__)

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

using i64 = long long;

const int N = 2e5;

int n; i64 a[N + 5], b[N + 5];

int ct[N + 5], ct2[N + 5];
i64 calc(i64 k) {
	memset(ct, 0, sizeof(ct));
	memset(ct2, 0, sizeof(ct2));
	std::vector<int> idx;
	for(int i = 1; i <= n; i++) {
		if(a[i] < k) continue;
		else if(a[i] == k) idx.push_back(i);
		else {
			i64 t = k * a[i] - b[i];
			if(1 <= t && t <= n) ct[t]++;
		}
	}
	i64 res = 0;
	for(int i : idx) {
		i64 t = k * k - b[i];
		res += ct2[b[i]];
		if(1 <= t && t <= n) ct2[t]++;
	}
	for(int i : idx) res += ct[b[i]];
	return res;
}

void solve() {
	n = rd();
	for(int i = 1; i <= n; i++) a[i] = rd();
	for(int i = 1; i <= n; i++) b[i] = rd();
	int lim = (int)sqrt(n * 2) + 1;
	i64 ans = 0;
	for(int i = 1; i <= lim; i++) ans += calc(i);
	printf("%lld\n", ans);
}

int main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
