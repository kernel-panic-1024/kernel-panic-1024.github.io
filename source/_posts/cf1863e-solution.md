---
title: Codeforces 1863E Speedrun 题解
date: 2023-08-31 21:54:50
categories: 题解
tags: 动态规划
---

ckain 巨佬百忙之中抽出时间教我做题，感恩．

<!-- more -->

## 思路

容易发现，若我们第一天的开始时间已经确定了，那么整个流程都是确定的，我们只需要决策第一天有多少活动需要推迟一天完成即可．

尝试设 $f_{u, 0 / 1}$ 表示 $u$ 这个活动不推迟 / 推迟一天完成时，依赖 $u$ 的事件的最晚完成时间．为了完成后继事件 $v$ 到 $f_{u, 1}$ 的转移，我们需要判定 $u$ 推迟一天后是否会影响到 $v$ 的完成．如果影响了，则需要从 $f_{v, 1}$ 转移．

麻烦之处在于，$u \to v$ 这条边造成的时间限制不一定是紧的，也就是说 $v$ 可能需要在 $u$ 完成后推迟很久才能被完成，此时尝试推迟 $u$ 的完成不会对 $v$ 的完成造成影响．

我们可以先尝试钦定顺序做这些活动，给每个节点先钦定一个完成日期，若 $u$ 的前驱 $p$ 的完成时刻晚于 $u$，那么 $u$ 需要推迟一天．然后这样就可以快速计算推迟造成的影响了，而由于我们按照逆拓扑序进行 DP，所以推迟不会对后面递推到的点造成影响，无后效性．

## 代码

```cpp
#include <cstdio>
#include <cassert>
#include <vector>
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

const int N = 2e5;

void chmax(int &x, int y) { if(x < y) x = y; }
void chmin(int &x, int y) { if(x > y) x = y; }

int n, m, k, a[N + 5];
std::vector<int> g[N + 5]; int ind[N + 5];

int p[N + 5], sft[N + 5];
int f[N + 5][2];
int pre[N + 5], suf[N + 5];

int calc(int u) { return k * sft[u] + a[u]; }

void solve() {
	n = rd(), m = rd(), k = rd();
	for(int i = 1; i <= n; i++) a[i] = rd();
	for(int i = 1; i <= m; i++) {
		int u = rd(), v = rd();
		g[u].push_back(v), ind[v]++;
	}

	static int q[N + 5]; int hd = 1, tl = 0;
	static int d0[N + 5]; int tp = 0;
	for(int i = 1; i <= n; i++) if(!ind[i]) q[++tl] = d0[++tp] = i;
	int cid = 0;
	while(hd <= tl) {
		int u = q[hd++]; p[++cid] = u;
		for(int v : g[u]) {
			chmax(sft[v], sft[u] + (a[u] > a[v]));
			if(!--ind[v]) q[++tl] = v;
		}
	}
	assert(cid == n);

	for(int i = n; i >= 1; i--) {
		int u = p[i];
		f[u][0] = calc(u), f[u][1] = calc(u) + k;
		for(int v : g[u]) {
			chmax(f[u][0], f[v][0]);
			chmax(f[u][1], f[v][calc(u) + k > calc(v)]);
		}
	}

	std::sort(d0 + 1, d0 + 1 + tp, [](int i, int j) { return a[i] < a[j]; });
	pre[0] = 0; for(int i = 1; i <= tp; i++) pre[i] = std::max(pre[i - 1], f[d0[i]][1]);
	suf[tp + 1] = 0; for(int i = tp; i >= 0; i--) suf[i] = std::max(suf[i + 1], f[d0[i]][0]);
	int ans = 1e18;
	for(int i = 0; i < tp; i++) {
		chmin(ans, std::max(pre[i], suf[i + 1]) - calc(d0[i + 1]));
	}
	printf("%lld\n", ans);

	for(int i = 1; i <= n; i++) {
		g[i].clear(), ind[i] = 0;
		sft[i] = 0, f[i][0] = f[i][1] = 0;
	}
}

signed main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
