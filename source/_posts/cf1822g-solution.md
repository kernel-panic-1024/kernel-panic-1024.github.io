---
title: Codeforces 1822G Magic Triples 题解
date: 2023-09-22 21:08:43
categories: 题解
tags:
  - 根号分治
  - 数学
---

<!-- more -->

## 思路

枚举 $j$，再枚举 $d | a_j$，$a_i$ 和 $a_k$ 分别能取 $a_j / d, a_j \times d$，拿个桶统计即可．

然而这样做的时间复杂度是 $O(n \sqrt{V})$ 的，直接做就寄了．由于有 $a_i \le {10}^9$，那么 $a_j \times d \le {10}^9$．

考虑根号分治，设置阈值 $B$．

1. 当 $a_i \le B$ 的时候直接做，这部分复杂度是 $O(nB^{1 / 2})$．
2. 当 $a_i > B$ 时，有用的因数不超过 $V / B$ 个，直接枚举因数，这部分复杂度是 $O(nV / B)$．

显然 $B$ 取 $V^{2 / 3}$ 时达到平衡，复杂度 $O(nV^{1 / 3})$，可以通过．

## 代码

```cpp
#include <cstdio>
#include <algorithm>
#include <map>
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
const int B = 1e6;
const int V = 1e9;

int n, a[N + 5], b[N + 5];

void solve() {
	std::map<int, int> ct;
	n = rd(); for(int i = 1; i <= n; i++) ct[rd()]++;

	auto calc = [&](int a, int b) {
		if(ct.count(a) && ct.count(b)) return ct[a] * ct[b];
		return 0ll;
	};

	int ans = 0;
	for(auto [k, v] : ct) {
		ans += v * (v - 1) * (v - 2);
		if(k == 1) continue;
		if(k <= B) {
			for(int d = 1; d * d <= k; d++) {
				if(k % d) continue;
				if(d * d == k) {
					ans += calc(k / d, k * d) * v;
				} else {
					if(d != 1) ans += calc(k / d, k * d) * v;
					ans += calc(d, k * k / d) * v;
				}
			}
		} else {
			for(int d = 2; d * k <= V; d++) {
				if(k % d == 0) ans += calc(k / d, k * d) * v;
			}
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
