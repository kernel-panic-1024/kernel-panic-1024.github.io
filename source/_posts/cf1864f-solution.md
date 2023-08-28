---
title: Codeforces 1864F Exotic Queries 题解
date: 2023-08-28 15:08:32
categories: 题解
tags:
  - 扫描线
---

Idea: ODT．/jk

<!-- more -->

## 思路

先考虑没有区间询问怎么做．

若我们每次都能将等于某个值的所有数归 $0$，那么答案就是值域区间中值的个数，容易证明这是答案的下界．

考虑将我们未考虑到的操作次数加上去．考虑一个值在序列中所有的出现位置，若相邻位置之间的最大值小于当前值，那么当前值需要的归零次数需要增加一次．统计答案就是统计这样的点对数目．

考虑带上区间询问怎么做．考虑将询问挂右端点上扫描线，需要统计的点对 $(l, r)$ 需要满足

$$
l \le \max_{i = l + 1}^{r - 1} a_i < a_l = a_r
$$

由于总点对数目是 $O(n)$ 的，弄个树状数组维护单点加后缀和，然后暴力加点即可．

当然还需要一棵线段树维护单点赋值区间 $\max$．

## 代码

```cpp
#include <cstdio>
#include <utility>
#include <vector>
#include <cassert>
#define debug(...) fprintf(stderr, __VA_ARGS__)

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

using pii = std::pair<int, int>;
const int N = 1e6;

int n, m, a[N + 5];

namespace segt {
#define lch (p * 2)
#define rch (p * 2 + 1)
#define mid ((cl + cr) / 2)
	int mx[N * 4 + 5];
	void pushup(int p) { mx[p] = std::max(mx[lch], mx[rch]); }
	void upd(int x, int v, int p = 1, int cl = 1, int cr = n) {
		if(cl == cr) return mx[p] = v, void();
		(x <= mid) ? upd(x, v, lch, cl, mid) : upd(x, v, rch, mid + 1, cr);
		pushup(p);
	}
	int que(int l, int r, int p = 1, int cl = 1, int cr = n) {
		if(cl == l && cr == r) return mx[p];
		if(r <= mid) return que(l, r, lch, cl, mid);
		else if(l > mid) return que(l, r, rch, mid + 1, cr);
		else return std::max(que(l, mid, lch, cl, mid), que(mid + 1, r, rch, mid + 1, cr));
	}
#undef lch
#undef rch
#undef mid
};

namespace fwt {
	int t[N + 5];
	inline int lowbit(int x) { return x & (-x); }
	void upd(int x, int d) {
		for(int i = x; i >= 1; i -= lowbit(i)) {
			t[i] += d;
		}
	}
	int que(int x) {
		int res = 0;
		for(int i = x; i <= n; i += lowbit(i)) {
			res += t[i];
		}
		return res;
	}
};

std::vector<int> pos[N + 5];

int s[N + 5];
std::vector<pii> q[N + 5]; int ans[N + 5];

int main() {
	n = rd(), m = rd(); for(int i = 1; i <= n; i++) a[i] = rd();
	for(int i = 1; i <= m; i++) {
		int l = rd(), r = rd();
		q[r].emplace_back(l, i);
	}

	for(int i = 1; i <= n; i++) pos[a[i]].push_back(i);
	for(int r = 1; r <= n; r++) {
		s[r] = s[r - 1] + bool(pos[r].size());
		for(int i = 0, li = pos[r].size(); i < li - 1; i++) {
			fwt::upd(segt::que(pos[r][i], pos[r][i + 1]), 1);
		}
		for(int x : pos[r]) segt::upd(x, r);
		for(auto [l, id] : q[r]) {
			ans[id] = s[r] - s[l - 1] + fwt::que(l);
		}
	}

	for(int i = 1; i <= m; i++) printf("%d\n", ans[i]);
	return 0;
}
```
