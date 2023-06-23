---
title: 洛谷 3362 Cool loves shaxian 题解
date: 2023-06-23 11:18:32
categories: 题解
tags:
  - 线性筛
---

/oh．

<!-- more -->

## 思路

题意即计算 $\sum\limits_{i = l}^r \sigma_d(n)$．

由于我们知道 $\sigma_d$ 是积性函数，故考虑线性筛．

质数处的 $\sigma_d$ 是容易计算的．当一个数 $i$ 被其最小质因子 $p$ 筛到时，有以下两种情况：

- $p \perp i / p$ 时，由 $\sigma_d$ 的积性可以得到 $\sigma_d(i) = \sigma_d(p) \sigma_d(i / p)$．
- $p \not\perp i / p$ 时，我们可以将 $i$ 中的 $p$ 这个质因子除尽．设 $i$ 的唯一分解中 $p$ 的次数为 $k$，显然 $i / p^k \perp p^{k + 1}$．那么 $\sigma_d(i) = \sigma_d(i / p^k) \sigma_d(p^{k + 1})$．
  对于每个 $i$ 都计算一遍 $k$ 显然会超时，但是这个 $k$ 也是可以线性筛的．具体的，记 $i$ 的 $k$ 为 $g_i$，当 $i$ 被 $p$ 筛到时，有：

  $$
  g_i =
  \begin{cases}
    p & i \bmod p \not = 0 \\
    g_{i / p} \times p & i \bmod p = 0 \\
  \end{cases}
  $$

那么预处理一下质数次幂处的 $\sigma_d$，就可以线性筛出答案了．

复杂度 $O\left(n + \dfrac{n}{\ln n}\log n\right) = O(n)$．

事实上在质数次幂处能快速计算的积性函数都可以使用类似的方式筛出，详见 [OI Wiki](https://oi-wiki.org/math/number-theory/sieve/#%E4%B8%80%E8%88%AC%E7%9A%84%E7%A7%AF%E6%80%A7%E5%87%BD%E6%95%B0)．

## 代码

```cpp
#include <cstdio>

#define int long long

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

const int N = 1e7;
const int P = 1e9 + 7;

int n, d, q;

int fpow(int b, int p) {
	int res = 1;
	for(; p; b = b * b % P, p >>= 1) {
		if(p & 1) res = res * b % P;
	}
	return res;
}

int pri[N + 5], pct, vis[N + 5];
int f[N + 5], s[N + 5];

void pre() {
	s[1] = 1;
	for(int i = 2; i <= N; i++) {
		if(!vis[i]) {
			pri[++pct] = i;
			int k = fpow(i, d);
			s[i] = (k + 1) % P;
			f[i] = i;
			for(int cur = i * i, curk = k * k % P; cur <= N; cur *= i, (curk *= k) %= P) {
				s[cur] = (s[cur / i] + curk) % P;
			}
		}
		for(int j = 1; j <= pct && i * pri[j] <= N; j++) {
			vis[i * pri[j]] = true;
			if(i % pri[j]) {
				s[i * pri[j]] = s[i] * s[pri[j]] % P;
				f[i * pri[j]] = pri[j];
			} else {
				s[i * pri[j]] = s[i / f[i]] * s[pri[j] * f[i]] % P;
				f[i * pri[j]] = f[i] * pri[j];
				break;
			}
		}
	}
	for(int i = 1; i <= N; i++) (s[i] += s[i - 1]) %= P;
}

signed main() {
	n = rd(), d = rd(), q = rd();
	pre();
	for(int i = 1; i <= q; i++) {
		int l = rd(), r = rd();
		printf("%lld\n", (s[r] - s[l - 1] + P) % P);
	}
	return 0;
}
```
