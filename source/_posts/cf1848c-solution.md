---
title: Codeforces 1848C Vika and Price Tags 题解
date: 2023-09-07 15:12:54
categories: 题解
---

乐乐乐乐乐．

<!-- more -->

## 思路

考虑 $n = 1$ 的情况，不妨设 $(x, y) = (a_1, b_1)$，然后就可以观察到如果已经达到了目标状态，即 $x = 0$，那么接下来进行的操作就会形如

$$
(0, y) \to (y, y) \to (y, 0) \to (0, y)
$$

周期为 $3$．

那么只需要算出每个位置上的数对达到目标状态的次数，然后判定 $\bmod\ 3$ 下是否相等即可．

考虑快速计算操作次数，不妨设 $a < b$，$b = ta + r$，状态变化形如

$$
(a, ta + r) \to (ta + r, (t - 1)a + r) \to ((t - 1)a + r, a) \to (a, (t - 2)a + r)
$$

周期也为 $3$，且每轮中 $t$ 会减 $2$．由于我们只关心操作次数 $\bmod\ 3$ 后的值，所以模拟操作时可以直接将 $k$ 模 $2$．然后讨论一下

1. $t \bmod 2 = 1$ 时，$(a, a + r) \to (a + r, r) \to (r, a)$，使用 $2$ 步操作转化为子问题，递归解决 $(r, a)$．
2. $t \bmod 2 = 0$ 时，直接递归解决 $(a, r)$．

$a > b$ 时类似．每次子问题规模至少减半，那么复杂度 $\Theta(n \log V)$，其中 $V$ 是值域．

## 代码

```cpp
#include <cstdio>
#define debug(...) fprintf(stderr, __VA_ARGS__)

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

const int N = 1e5;

int n, a[N + 5], b[N + 5];
int calc(int a, int b) {
	if(!a || !b) return bool(a);
	if(a < b) {
		int d = b / a, r = b % a;
		if(d & 1) return calc(r, a) + 2;
		else return calc(a, r);
	} else {
		int d = a / b, r = a % b;
		if(d & 1) return calc(b, r) + 1;
		else return calc(r, b);
	}
}
void solve() {
	n = rd();
	for(int i = 1; i <= n; i++) a[i] = rd();
	for(int i = 1; i <= n; i++) b[i] = rd();
	int ct[3]; ct[0] = ct[1] = ct[2] = 0;
	int tot = 0;
	for(int i = 1; i <= n; i++) {
		if(a[i] || b[i]) {
			tot++;
			ct[calc(a[i], b[i]) % 3]++;
		}
	}
	for(int i = 0; i < 3; i++) if(ct[i] == tot) return puts("YES"), void();
	puts("NO");
}

int main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```

## 参考

bykem, [_CF1848C Vika and Price Tags 题解_](https://www.luogu.com.cn/blog/bykem/solution-cf1848c)
