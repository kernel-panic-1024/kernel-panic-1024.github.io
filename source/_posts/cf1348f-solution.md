---
title: Codeforces 1348F Phoenix and Memory 题解
date: 2023-09-18 21:55:18
categories: 题解
---

*2600？

<!-- more -->

## 思路

首先考虑求出一个排列．若存在不止一个满足条件的排列，那么其他排列一定可以通过交换求出的排列中的某两项来求出．

贪个心先，将线段按照左端点升序排序，考虑按照升序确定 $1 \sim n$ 这些值所填的位置，每次取出能够覆盖到 $i$ 的，右端点最小的区间，将 $i$ 放到对应的位置上．由于题目保证有解，这样的构造显然合法．

然后就是确定能否交换两个位置了．设位置 $i$ 和 $j$ 可以交换，那么有：$a_i \le p_j \le b_i, a_j \le p_i \le b_j$，考虑按照 $p_i$ 升序添加元素干掉第一个限制，然后第二个限制相当于看有没有区间覆盖到 $p_i$，拿一棵线段树维护区间覆盖单点查询即可．

## 代码

```cpp
#include <cstdio>
#include <utility>
#include <algorithm>
#include <vector>
#include <queue>
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

const int N = 2e5;

int n;
pii a[N + 5]; int pos[N + 5];
int p[N + 5];

#define lch (p * 2)
#define rch (p * 2 + 1)
#define mid ((cl + cr) / 2)
struct node {
	int c, tg;
	void upd(int v) {
		c = tg = v;
	}
} t[N * 4 + 5];
void pushdown(int p) {
	if(!t[p].tg) return;
	t[lch].upd(t[p].tg), t[rch].upd(t[p].tg);
	t[p].tg = 0;
}
void upd(int l, int r, int v, int p = 1, int cl = 1, int cr = n) {
	if(cl == l && cr == r) return t[p].upd(v);
	pushdown(p);
	if(r <= mid) upd(l, r, v, lch, cl, mid);
	else if(l > mid) upd(l, r, v, rch, mid + 1, cr);
	else upd(l, mid, v, lch, cl, mid), upd(mid + 1, r, v, rch, mid + 1, cr);
}
int que(int x, int p = 1, int cl = 1, int cr = n) {
	if(cl == cr) return t[p].c;
	pushdown(p);
	return (x <= mid) ? que(x, lch, cl, mid) : que(x, rch, mid + 1, cr);
}
#undef lch
#undef rch
#undef mid

struct Q { int x, flg, id; }; std::vector<Q> q[N + 5]; int ans[N + 5], pre[N + 5];

int main() {
	n = rd();
	for(int i = 1; i <= n; i++) a[i] = {rd(), rd()}, pos[i] = i;
	std::sort(pos + 1, pos + 1 + n, [](int i, int j) { return a[i] < a[j]; });

	std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;
	for(int i = 1, j = 1; i <= n; i++) {
		while(j <= n && a[pos[j]].first == i) {
			pq.push({a[pos[j]].second, pos[j]});
			j++;
		}
		p[pq.top().second] = i; pq.pop();
	}

	for(int i = 1; i <= n; i++) pos[i] = i;
	std::sort(pos + 1, pos + 1 + n, [](int i, int j) { return p[i] < p[j]; });
	for(int i = 1; i <= n; i++) {
		auto [l, r] = a[i];
		q[l - 1].push_back({p[i], -1, i});
		q[r].push_back({p[i], 1, i});
	}
	for(int i = 1; i <= n; i++) {
		upd(a[pos[i]].first, a[pos[i]].second, pos[i]);
		for(auto [x, flg, id] : q[i]) {
			int c = que(x);
			if(flg == 1) { if(c != pre[id]) ans[id] = c; }
			else pre[id] = c;
		}
	}
	for(int i = 1; i <= n; i++) {
		if(ans[i] && ans[i] != i) {
			puts("NO");
			for(int j = 1; j <= n; j++) printf("%d%c", p[j], " \n"[j == n]);
			std::swap(p[i], p[ans[i]]);
			for(int j = 1; j <= n; j++) printf("%d%c", p[j], " \n"[j == n]);
			return 0;
		}
	}
	puts("YES");
	for(int i = 1; i <= n; i++) printf("%d%c", p[i], " \n"[i == n]);
	return 0;
}
```
