---
title: Codeforces 1819B The Butcher 题解
date: 2023-09-23 14:11:49
categories: 题解
---

什么 b 题．

<!-- more -->

## 思路

首先可以找到最大的长和宽，这可能是原长方形的或宽．

然后可以算面积和，就可以初步判定长宽的合法性．

考虑如何确定某个长宽是合法的．考虑模拟，每次选出最长的长或宽尝试去剪，若某次选不出来就寄了．可以用两个 `multiset` 维护．

## 代码

```cpp
#include <cstdio>
#include <set>
#include <algorithm>
#include <utility>
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

using pii = std::pair<int, int>;
using Set = std::multiset<pii, std::greater<pii>>;

int n; pii ans[3];

bool check(Set a, Set b, int n, int m) {
	while(a.size() && b.size()) {
		auto [x1, y1] = *a.begin();
		auto [x2, y2] = *b.begin();
		if(x1 == n) {
			m -= y1;
			a.erase(a.lower_bound({x1, y1}));
			b.erase(b.lower_bound({y1, x1}));
		} else if(x2 == m) {
			n -= y2;
			a.erase(a.lower_bound({y2, x2}));
			b.erase(b.lower_bound({x2, y2}));
		} else return false;
	}
	return true;
}

void solve() {
	n = rd();
	Set a, b;
	int siz = 0;
	for(int i = 1; i <= n; i++) {
		int x = rd(), y = rd();
		a.insert({x, y}), b.insert({y, x});
		siz += x * y;
	}
	std::set<pii> ans;
	{
		int n = (*a.begin()).first;
		if(siz % n == 0) {
			if(check(a, b, n, siz / n)) ans.insert({n, siz / n});
		}
	}
	{
		int m = (*b.begin()).first;
		if(siz % m == 0) {
			if(check(a, b, siz / m, m)) ans.insert({siz / m, m});
		}
	}
	printf("%lld\n", (int)ans.size());
	for(auto [x, y] : ans) {
		printf("%lld %lld\n", x, y);
	}
}

signed main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
