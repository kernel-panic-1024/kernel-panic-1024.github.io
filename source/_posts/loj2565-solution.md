---
title: LOJ 2565 「SDOI2018」旧试题 题解
date: 2023-05-09 22:58:43
categories: 题解
tags:
  - Mobius 反演
  - 三元环计数
---

大受震撼．

<!-- more -->

## 思路

首先我们有 $\sigma_0(xyz) = \sum\limits_{i | x} \sum\limits_{j | y} \sum\limits_{k | z} [i \perp j][j \perp k][i \perp k]$．

然后开始大力推柿子：

$$
\def\flr#1{\left\lfloor #1 \right\rfloor}
\begin{aligned}
  \sum_{i = 1}^a \sum_{j = 1}^b \sum_{k = 1}^c \sigma_0(ijk)
  &= \sum_{i = 1}^a \sum_{j = 1}^b \sum_{k = 1}^c \sum_{x | i} \sum_{y | j} \sum_{z | k} [x \perp y][y \perp z][x \perp z] \\
  &= \sum_{x = 1}^a \sum_{y = 1}^b \sum_{z = 1}^c [x \perp y][y \perp z][x \perp z] \flr{a / x} \flr{b / y} \flr{c / z} \\
  &= \sum_{x = 1}^a \sum_{y = 1}^b \sum_{z = 1}^c \sum_{r | (x, y)} \mu(r) \sum_{s | (y, z)} \mu(s) \sum_{t | (x, z)} \mu(t) \flr{a / x} \flr{b / y} \flr{c / z} \\
  &= \sum_{r = 1}^{\min\{a, b\}} \mu(r) \sum_{s = 1}^{\min\{b, c\}} \mu(s) \sum_{t = 1}^{\min\{a, c\}} \mu(t) \sum_{[r, t] | x} \flr{a / x} \sum_{[r, s] | y} \flr{b / y} \sum_{[s, t] | z} \flr{c / z} \\
\end{aligned}
$$

定义 $f_a(x) = \sum\limits_{x | i} \lfloor a / i \rfloor$，显然可以预处理 $f_a$、$f_b$、$f_c$．那么我们要求的就是：

$$
\sum_{r = 1}^{\min\{a, b\}} \sum_{s = 1}^{\min\{b, c\}} \sum_{t = 1}^{\min\{a, c\}} \mu(r)\mu(s)\mu(t)f_a([r, t])f_b([r, s])f_c([s, t])
$$

然后你会发现，我们反演了一通，计算复杂度没变．~~止步于此．~~

咋办？考虑直接枚举对答案贡献不为 $0$ 的三元组 $(r, s, t)$．由于 $\mu(x)$ 很可能为 $0$，而 $f_a(x)$ 在 $x > a$ 时也为 $0$，直觉告诉我们这样的三元组不会很多．

有一种枚举这样的三元组的方式：考虑一张无向图 $G = (V, E)$，对于所有的 $u, v$，若 $\mu(u) \not= 0, \mu(v) \not= 0, [u, v] \le \max\{a, b, c\}$，那么 $(u, v) \in E$，这样的 $G$ 中的每个三元环都对应一个贡献不为 $0$ 且元素互异的三元组．跑一遍三元环计数即可获得所有元素互异的三元组造成的贡献．对于相同元素造成的贡献，拉出来单独统计即可．

实现上，为了避免 $O(n^2)$ 连边，我们枚举一个数 $g$ 充当 $(u, v)$，再枚举 $u = ig, v = jg$，使得 $i \perp j$，此时有 $[u, v] = ijg$，连边即可．复杂度貌似是根号或者调和级数级别？

## 代码

~~太丑了，就不放了．~~ 还是放一下．

```cpp
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <utility>
#include <vector>

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

using i64 = long long;

const int N = 1e5;
const int M = 1e6;

int T, a, b, c;

int vis[N + 5], pri[N + 5], pct, mu[N + 5];
void pre() {
	mu[1] = 1;
	for (int i = 2; i <= N; i++) {
		if (!vis[i]) pri[++pct] = i, mu[i] = -1;
		for (int j = 1; j <= pct && i * pri[j] <= N; j++) {
			vis[i * pri[j]] = true;
			if (i % pri[j]) mu[i * pri[j]] = -mu[i];
			else break;
		}
	}
}

// f_a(x) = \sum\limits_{x | i}^a [a / i]
i64 fa[N + 5], fb[N + 5], fc[N + 5];
inline i64 calc(int i, int j, int k) { return 1ll * fa[i] * fb[j] * fc[k]; }

struct edge {
	int u, v, w;
} e[M + 5];
int ect, d[N + 5], flg[N + 5];

std::vector<std::pair<int, int>> g[N + 5];

signed main() {
	pre(), T = rd();
	while (T--) {
		a = rd(), b = rd(), c = rd();
		int n = std::max({a, b, c}), m = std::min({a, b, c});
		for (int i = 1; i <= a; i++)
			for (int j = 1; j * i <= a; j++) fa[i] += a / (j * i);
		for (int i = 1; i <= b; i++)
			for (int j = 1; j * i <= b; j++) fb[i] += b / (j * i);
		for (int i = 1; i <= c; i++)
			for (int j = 1; j * i <= c; j++) fc[i] += c / (j * i);

		i64 ans = 0;

		for (int i = 1; i <= m; i++) ans += 1ll * mu[i] * mu[i] * mu[i] * fa[i] * fb[i] * fc[i];

		for (i64 gcd = 1; gcd <= n; gcd++) {
			for (i64 i = 1; i * gcd <= n; i++) {
				if (!mu[i * gcd]) continue;
				for (i64 j = i + 1; j * gcd <= n && i * j * gcd <= n; j++) {
					if (!mu[j * gcd] || std::__gcd(i, j) != 1) continue;
					i64 u = i * gcd, v = j * gcd, w = i * j * gcd;
					e[++ect] = {u, v, w}, d[u]++, d[v]++;

					ans += 1ll * mu[u] * mu[u] * mu[v] * (calc(u, w, w) + calc(w, u, w) + calc(w, w, u));
					ans += 1ll * mu[v] * mu[v] * mu[u] * (calc(v, w, w) + calc(w, v, w) + calc(w, w, v));
				}
			}
		}
		for (int i = 1; i <= ect; i++) {
			auto [u, v, w] = e[i];
			if ((d[u] > d[v]) || (d[u] == d[v] && u > v)) std::swap(u, v);
			g[u].emplace_back(v, w);
		}
		for (int u = 1; u <= n; u++) {
			if (!mu[u]) continue;
			for (auto [v, w] : g[u]) flg[v] = w;
			for (auto [v, a] : g[u]) {
				if (!mu[v]) continue;
				for (auto [w, b] : g[v]) {
					if (!mu[w] || !flg[w]) continue;
					int c = flg[w];
					ans += 1ll * mu[u] * mu[v] * mu[w]
						 * (calc(a, b, c) + calc(a, c, b)
							+ calc(b, a, c) + calc(b, c, a)
							+ calc(c, a, b) + calc(c, b, a));
				}
			}
			for (auto [v, _] : g[u]) flg[v] = 0;
		}
		printf("%lld\n", ans % 1000000007);

		memset(fa, 0, sizeof(fa)), memset(fb, 0, sizeof(fb)), memset(fc, 0, sizeof(fc));
		memset(d, 0, sizeof(d)), memset(flg, 0, sizeof(flg));
		for (int i = 1; i <= n; i++) g[i].clear();
		ect = 0;
	}
	return 0;
}
```

## 参考

GKxx, [_题解 P4619_](https://www.luogu.com.cn/blog/gkxx-is-here/solution-p4619)
