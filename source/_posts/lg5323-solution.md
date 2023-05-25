---
title: 洛谷 5323 [BJOI2019] 光线 题解
date: 2023-05-25 14:18:58
categories: 题解
tags:
- 数学
---

为什么想不到！！！！！！！1111

## 思路

考虑逐层合并玻璃，并计算对合并后玻璃对从上方射入的光线的透过率．

设我们此前合并的玻璃对从上方射入的光线的透过率的透过率为 $a_1$，当前我们准备合并的玻璃的透过率为 $a_2$，发现由于光线会在两层玻璃之间反复横跳，只计算这一个量无法进行合并．

考虑再计算合并后玻璃对从下方射入的光线的反射率．

再设我们此前合并的玻璃对从下方射入的光线的反射率为 $b_1$，当前我们准备合并的玻璃的反射率为 $b_2$，新合并出的玻璃的向下透过率和向上反射率为 $a, b$，考察光线在玻璃间反射和透过的过程，我们可以得到：

$$
\begin{aligned}
  & a = a_1a_2 + a_1b_1b_2a_2 + a_1b_1b_2b_1b_2a_2 + \cdots = a_1a_2 \sum_{i = 0}^\infty (b_1b_2)^i = \frac{a_1a_2}{1 - b_1b_2} \\
  & b = b_2 + a_2b_1a_2 + a_2b_1b_2b_1a_2 + a_2b_1b_2b_1b_2b_1a_2 + \cdots = b_2 + b_1{a_2}^2 \sum_{i = 0}^\infty (b_1b_2)^i = b_2 + \frac{b_1{a_2}^2}{1 - b_1b_2}
\end{aligned}
$$

递推即可．

## 代码

```cpp
#include <cstdio>

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

const int N = 1e5;

using i64 = long long;
const i64 P = 1e9 + 7;
const i64 i100 = 570000004;

i64 fpow(i64 b, i64 p) {
	i64 res = 1;
	for(; p; b = b * b % P, p >>= 1) {
		if(p & 1) res = res * b % P;
	}
	return res;
}

int n;

int main() {
	n = rd();
	i64 a1 = rd() * i100 % P, b1 = rd() * i100 % P;
	for(int i = 2; i <= n; i++) {
		i64 a2 = rd() * i100 % P, b2 = rd() * i100 % P;
		i64 na = a1 * a2 % P * fpow(1 - b1 * b2 % P + P, P - 2) % P;
		i64 nb = (b2 + b1 * a2 % P * a2 % P * fpow(1 - b1 * b2 % P + P, P - 2)) % P;
		a1 = na, b1 = nb;
	}
	printf("%lld\n", a1);
	return 0;
}
```