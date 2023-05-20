---
title: 洛谷 3312 [SDOI2014] 数表 题解
date: 2023-05-20 13:49:04
categories: 题解
tags:
- 数学
- 数论分块
- 树状数组
---

非常好反演，爱来自 Mobius．

## 题面

### 题目描述

$q$ 次询问，每次给定 $n, m, a$，求：

$$
\sum_{i = 1}^n \sum_{j = 1}^m [\sigma(\gcd(i, j)) \le a] \sigma(\gcd(i, j))
$$

### 数据范围

$1 \le n, m \le {10}^5$，$1 \le q \le 2 \times {10}^4$．

<!-- more -->

## 思路

令 $\sigma^\prime(x) = [\sigma(x) \le a] \sigma(x)$．

不失一般性，令 $n \le m$，大力推式：

$$
\def\flr#1{\left\lfloor #1 \right\rfloor}
\begin{aligned}
  \sum_{i = 1}^n \sum_{j = 1}^m \sigma^\prime(\gcd(i, j))
  &= \sum_{g = 1}^n \sigma^\prime(g) \sum_{i = 1}^n \sum_{j = 1}^m [\gcd(i, j) = g] \\
  &= \sum_{g = 1}^n \sigma^\prime(g) \sum_{x = 1}^{\flr{n / g}} \sum_{y = 1}^{\flr{m / g}} [\gcd(x, y) = 1] \\
  &= \sum_{g = 1}^n \sigma^\prime(g) \sum_{x = 1}^{\flr{n / g}} \sum_{y = 1}^{\flr{m / g}} \sum_{d | \gcd(x, y)} \mu(d) \\
  &= \sum_{g = 1}^n \sigma^\prime(g) \sum_{d = 1}^{\flr{n / g}} \mu(d) \flr{\frac{n}{gd}} \flr{\frac{m}{gd}} \\
  &= \sum_{T = 1}^n \flr{\frac{n}{T}} \flr{\frac{m}{T}} \sum_{p | T} \sigma^\prime(p) \mu\left(\frac{T}{p}\right)
\end{aligned}
$$

令 $g(x) = \sum\limits_{d | x} \sigma^\prime(d) \mu\left(\frac{x}{d}\right)$，答案即为 $\sum\limits_{i = 1}^n \left\lfloor\frac{n}{i}\right\rfloor \left\lfloor\frac{m}{i}\right\rfloor g(i)$．若能处理 $g(x)$ 的前缀和，就能数论分块 $O(\sqrt{n})$ 回答每次询问．

~~在推式推错的过程中~~ 发现这个 $\sigma^\prime(x)$ 的前缀和有点像二维数点，于是考虑将询问离线，按照 $a$ 升序回答每个询问．

考虑 $a - 1 \rightarrow a$ 的过程对 $g(x)$ 造成的影响，发现满足 $\sigma(x) = a$ 的 $x$ 对满足 $x | y$ 的 $g(y)$ 都造成了 $\sigma(x)\mu\left(\frac{y}{x}\right)$ 的贡献．用树状数组维护 $g(x)$ 即可．

在整个过程中，满足 $x \le n, \sigma(x) \le \max a$ 的所有 $x$ 都会造成 $O(\log n)$ 次贡献，而这样的 $x$ 的个数是 $O(n)$ 的，于是总时间复杂度 $O(q\sqrt{n}\log n + n \log^2 n)$．

## 代码
```cpp
#include <cstdio>
#include <algorithm>
#include <utility>

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

#define int long long

using pii = std::pair<int, int>;
const int N = 1e5;
const int P = 1ll << 31;

int T;
struct query { int n, m, a, id; } q[N + 5];
int nd[N + 5];
int ans[N + 5];

int vis[N + 5], pct, pri[N + 5], mu[N + 5], d[N + 5];
void pre() {
	vis[1] = 1, mu[1] = 1;
	for(int i = 2; i <= N; i++) {
		if(!vis[i]) pri[++pct] = i, mu[i] = -1;
		for(int j = 1; j <= pct && i * pri[j] <= N; j++) {
			vis[i * pri[j]] = 1;
			if(i % pri[j]) mu[i * pri[j]] = -mu[i];
			else break;
		}
	}
	for(int i = 1; i <= N; i++) {
		for(int j = 1; j * i <= N; j++) {
			d[j * i] += i;
		}
	}
	for(int i = 1; i <= N; i++) nd[i] = i;
	std::sort(nd + 1, nd + 1 + N, [&](int a, int b) { return d[a] < d[b]; });
}

int t[N + 5];
int lowbit(int x) { return x & (-x); }
void upd(int x, int d) { for(int i = x; i <= N; i += lowbit(i)) (t[i] += d) %= P; }
int que(int x) { int res = 0; for(int i = x; i >= 1; i -= lowbit(i)) (res += t[i]) %= P; return res; }

int calc(int n, int m) {
	if(n > m) std::swap(n, m);
	int ans = 0;
	for(int i = 1, j; i <= n; i = j + 1) {
		j = std::min(n / (n / i), m / (m / i));
		(ans += (n / i) * (m / i) % P * (que(j) - que(i - 1) + P) % P) %= P;
	}
	return ans;
}

signed main() {
	pre(), T = rd();
	for(int i = 1; i <= T; i++) q[i] = {rd(), rd(), rd(), i};
	std::sort(q + 1, q + 1 + T, [](query a, query b) { return a.a < b.a; });

	int cur = 1;
	for(int _ = 1; _ <= T; _++) {
		auto [n, m, a, id] = q[_];
		for(; d[nd[cur]] <= a; cur++) {
			for(int i = 1; i * nd[cur] <= N; i++) {
				upd(i * nd[cur], d[nd[cur]] * mu[i] % P);
			}
		}
		ans[id] = calc(n, m);
	}
	for(int i = 1; i <= T; i++) {
		printf("%lld\n", ans[i]);
	}
	return 0;
}
```