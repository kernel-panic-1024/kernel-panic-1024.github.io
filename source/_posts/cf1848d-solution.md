---
title: Codeforces 1848D Vika and Bonuses 题解
date: 2023-09-07 15:40:58
categories: 题解
---

烂．

<!-- more -->

## 思路

观察可以发现，个位数的变化情况形如

![变化情况](/images/cf1848d-solution/1.png)

容易发现 $2 \to 4 \to 8 \to 6$ 是一轮循环，且经过一轮循环之后 $s$ 会增加 $20$．

考虑对一个 $s(s \in \{2, 4, 6, 8\})$ 求解．设循环轮数为 $x$，由于我们一定先给 $s$ 增再累计，所以操作后的值 $w = (s + 20x)(k - 4x) = -80x^2 + (20k - 4s)x + sk$，是一个关于 $x$ 的二次函数，容易求出极值．然后对每个可能的 $s$ 做一遍就行了．

记得判掉 $0$ 和 $5$．

## 代码

```cpp
#include <cstdio>
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

int calc(int s, int k, int x) { return -80 * x * x + (20 * k - 4 * s) * x + s * k; }

int solve(int s, int k) {
	int x = std::max(std::min((5 * k - s) / 40, k / 4), 0ll);
	return std::max({s * k, calc(s, k, x), calc(s, k, x + 1)});
}

int s, k;
void solve() {
	s = rd(), k = rd();
	if(s % 10 == 0) return printf("%lld\n", s * k), void();
	if(s % 10 == 5) return printf("%lld\n", std::max(s * k, (s + 5) * (k - 1))), void();
	int ans = s * k;
	if(s % 2) s += s % 10, k--;
	for(int i = 1; i <= 4; i++) {
		if(k <= 0) break;
		ans = std::max(ans, solve(s, k)), s += s % 10, k--;
	}
	printf("%lld\n", ans);
}

signed main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
