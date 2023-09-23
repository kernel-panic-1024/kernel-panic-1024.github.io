---
title: 洛谷 6072 『MdOI R1』Path 题解
date: 2023-09-23 16:59:27
categories: 题解
tags:
  - Trie
  - DSU on tree
---

二合一．

<!-- more -->

## 思路

先做一遍树上前缀异或和，转化为选出四个点 $a, b, c, d$，使得 $V(a, b) \cap V(c, d) = \varnothing$，最大化 $(v_a \oplus v_b) + (v_c \oplus v_d)$．

为了满足第一个限制，我们可以枚举一个点 $u$，钦定 $a, b$ 在 $u$ 的子树内，$c, d$ 在 $u$ 的子树外，容易发现这覆盖了所有情况．然后两部分独立了，考虑分别求解．

子树内的部分可以采用 01-Trie + DSU on tree 简单解决，子树外的部分就是 [[Ynoi Easy Round 2021] TEST_68](https://www.luogu.com.cn/problem/P8511)．然后把两部分答案拼起来就行．

时间复杂度 $O(n \log n \log V)$，其中 $V$ 是值域．

## 代码

```cpp
#include <cstdio>
#include <cstring>
#include <utility>
#include <vector>
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

const int N = 3e4;
const int LOGV = 30;

int n; std::vector<pii> G[N + 5];

int fa[N + 5]; std::vector<int> g[N + 5];

int val[N + 5];
void dfs(int u, int f) {
	fa[u] = f;
	for (auto [v, w] : G[u]) {
		if (v == f) continue;
		val[v] = val[u] ^ w, dfs(v, u);
	}
}

int st[N + 5], stc[N + 5];

namespace trie {
	int ch[N * LOGV + 5][2], ed[N * LOGV + 5], alct;
	void ins(int x, int id) {
		int p = 0;
		for (int i = LOGV; i >= 0; i--) {
			int c = x >> i & 1;
			if (!ch[p][c]) ch[p][c] = ++alct;
			p = ch[p][c];
		}
		ed[p] = id;
	}
	pii que(int x) {
		int p = 0, res = 0;
		for (int i = LOGV; i >= 0; i--) {
			int c = x >> i & 1;
			if (ch[p][!c]) p = ch[p][!c], res |= 1 << i;
			else p = ch[p][c];
		}
		return {res, ed[p]};
	}
	void clear() {
		for (int i = 0; i <= alct; i++) ch[i][0] = ch[i][1] = ed[i] = 0;
		alct = 0;
	}
};

namespace paradise {
	int dfn[N + 5], rid[N + 5], cid;
	int siz[N + 5], hch[N + 5], chn[N + 5];
	void gethch(int u) {
		siz[u] = 1;
		for (int v : g[u]) {
			gethch(v), siz[u] += siz[v];
			if (siz[v] > siz[hch[u]]) hch[u] = v;
		}
	}
	void getchn(int u, int ct) {
		rid[dfn[u] = ++cid] = u;
		if (hch[u]) getchn(hch[u], ct);
		for (int v : g[u]) {
			if (v == hch[u]) continue;
			getchn(v, v);
		}
	}
	void dfs(int u, bool keep) {
		for (int v : g[u]) {
			if (v == hch[u]) continue;
			dfs(v, 0), st[u] = std::max(st[u], st[v]);
		}

		if (hch[u]) dfs(hch[u], 1), st[u] = std::max(st[u], st[hch[u]]);

		st[u] = std::max(st[u], trie::que(val[u]).first);
		trie::ins(val[u], u);
		for (int v : g[u]) {
			if (v == hch[u]) continue;
			for (int i = dfn[v]; i <= dfn[v] + siz[v] - 1; i++) {
				int w = rid[i];
				st[u] = std::max(st[u], trie::que(val[w]).first);
				trie::ins(val[w], w);
			}
		}
		if (!keep) trie::clear();
	}
	void solve() {
		gethch(1), getchn(1, 1);
		dfs(1, 0);
	}
};

namespace paradox {
	int cur;
	void ins_subtree(int u) {
		trie::ins(val[u], 0);
		cur = std::max(cur, trie::que(val[u]).first);
		for (int v : g[u]) ins_subtree(v);
	}
	void solve_chain(int u) {
		static int st[N + 5]; int tp = 0;
		while (u) st[++tp] = u, u = fa[u];
		for (int i = tp; i >= 1; i--) {
			int u = st[i];
			stc[u] = std::max(stc[u], cur);
			trie::ins(val[u], 0);
			cur = std::max(cur, trie::que(val[u]).first);
			for (int v : g[u]) {
				if (v == st[i - 1]) continue;
				ins_subtree(v);
			}
		}
	}
	void solve() {
		trie::ins(0, 0);
		for (int i = 1; i <= n; i++) trie::ins(val[i], i);
		int mx = -1, u = -1, v = -1;
		for (int i = 1; i <= n; i++) {
			auto [va, j] = trie::que(val[i]);
			if (va > mx) mx = va, u = i, v = j;
		}
		memset(stc, -1, sizeof(stc));
		trie::clear(), cur = 0, solve_chain(u);
		trie::clear(), cur = 0, solve_chain(v);
		for (int i = 1; i <= n; i++) if (stc[i] == -1) stc[i] = mx;
	}
};

signed main() {
	n = rd();
	for (int i = 2; i <= n; i++) {
		int u = rd(), v = rd(), w = rd();
		G[u].emplace_back(v, w), G[v].emplace_back(u, w);
	}
	dfs(1, 0); for (int i = 2; i <= n; i++) g[fa[i]].push_back(i);

	paradise::solve(), paradox::solve();
	int ans = 0;
	for (int i = 2; i <= n; i++) {
		ans = std::max(ans, st[i] + stc[i]);
	}
	printf("%lld\n", ans);
	return 0;
}
```
