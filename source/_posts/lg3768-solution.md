---
title: 洛谷 3768 简单的数学题 题解
date: 2023-06-23 11:19:17
categories: 题解
tags:
  - Mobius 反演
  - 杜教筛
---

筛．/oh

<!-- more -->

## 思路

瞎反演一通可以得到：

$$
\begin{aligned}
  \text{原式}
  &= \sum_{g = 1}^n g^3 \sum_{d = 1}^{\lfloor n / g \rfloor} \mu(d) d^2 \left(\sum_{i = 1}^{\lfloor n / gd \rfloor} i\right)^2 \\
  &= \sum_{T = 1}^n \sum_{g | T} g^3 \left(\frac{T}{g}\right)^2 \mu\left(\frac{T}{g}\right) \left(\sum_{i = 1}^{\lfloor n / T \rfloor} i\right)^2 \\
  &= \sum_{T = 1}^n \left(\sum_{i = 1}^{\lfloor n / T \rfloor} i\right)^2 T^2 \sum_{g | T} g\mu\left(\frac{T}{g}\right) \\
  &= \sum_{T = 1}^n \left(\sum_{i = 1}^{\lfloor n / T \rfloor} i\right)^2 T^2 (1 * \mu)(T) \\
  &= \sum_{T = 1}^n \left(\sum_{i = 1}^{\lfloor n / T \rfloor} i\right)^2 T^2 \varphi(T) \\
\end{aligned}
$$

考虑数论分块，现在我们需要快速计算 $\sum\limits_{i = 1}^n i^2 \varphi(i)$．

考虑杜教筛．令 $f(n) = n^2 \varphi(n)$，$g(n) = n^2$，有：

$$
\begin{aligned}
  \sum_{i = 1}^n (f * g)(i)
  &= \sum_{i = 1}^n \sum_{d | i} d^2 \varphi(d) \frac{i^2}{d^2} \\
  &= \sum_{i = 1}^n i^2 \sum_{d | i} \varphi(d) \\
  &= \sum_{i = 1}^n i^3 = \frac{n^2 (n + 1)^2}{4}
\end{aligned}
$$

令 $S(n) = \sum\limits_{i = 1}^n f(i)$，有：

$$
S(n) = g(1)S(n) = \sum_{i = 1}^n (f * g)(i) - \sum_{i = 2}^n (f * g)(i) = \frac{n^2 (n + 1)^2}{4} - \sum_{i = 2}^n g(d)S(\lfloor n / d \rfloor)
$$

杜教筛即可．

不是很懂数论分块里面套杜教筛的复杂度，看题解说貌似还是 $O(n^{2 / 3})$ 的？

用 Euler 反演推貌似更简单．

## 代码

```cpp
#include <cstdio>
#include <unordered_map>

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

int P, n;

int fpow(int b, int p) {
	int res = 1;
	for (; p; b = b * b % P, p >>= 1) {
		if (p & 1) res = res * b % P;
	}
	return res;
}

int i4, i6;
int f(int n) {
	n %= P;
	return n * (n + 1) % P * (2 * n + 1) % P * i6 % P;
}
int g(int n) {
	n %= P;
	return (1 + n) * (1 + n) % P * n % P * n % P * i4 % P;
}

bool vis[N + 5];
int pri[N + 5], pct;
int sphi[N + 5];
void pre() {
	i4 = fpow(4, P - 2), i6 = fpow(6, P - 2);
	sphi[1] = 1;
	for (int i = 2; i <= N; i++) {
		if (!vis[i]) pri[++pct] = i, sphi[i] = i - 1;
		for (int j = 1; j <= pct && i * pri[j] <= N; j++) {
			vis[i * pri[j]] = true;
			if (i % pri[j]) sphi[i * pri[j]] = sphi[i] * (pri[j] - 1) % P;
			else sphi[i * pri[j]] = sphi[i] * pri[j] % P;
		}
	}
	for (int i = 1; i <= N; i++) (sphi[i] *= i * i % P) %= P;
	for (int i = 1; i <= N; i++) (sphi[i] += sphi[i - 1]) %= P;
}

std::unordered_map<int, int> msphi;
int getsphi(int n) {
	if (n <= N) return sphi[n];
	if (msphi.count(n)) return msphi[n];
	int res = g(n);
	for (int i = 2, j; i <= n; i = j + 1) {
		j = n / (n / i);
		(res += P - (f(j) - f(i - 1)) % P * getsphi(n / i) % P) %= P;
	}
	return msphi[n] = res;
}

signed main() {
	P = rd(), n = rd(), pre();
	int ans = 0;
	for (int i = 1, j; i <= n; i = j + 1) {
		j = n / (n / i);
		(ans += (getsphi(j) - getsphi(i - 1)) % P * g(n / i) % P) %= P;
	}
	printf("%lld\n", (ans + P) % P);
	return 0;
}
```
