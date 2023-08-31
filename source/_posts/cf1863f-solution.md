---
title: Codeforces 1863F Divide, XOR, and Conquer 题解
date: 2023-08-31 22:13:58
categories: 题解
---

这么厉害？？？？？？？？？？

<!-- more -->

## 思路

考察一个区间 $[l, r]$ 在何时可以被划分出并保留 $[l, k]$．

记 $s_{l, r} = \bigoplus\limits_{i = l}^r a_i$．对于一个划分点 $k$，记 $x = s_{l, k}, y = s_{k + 1, r}$．

1. 若 $s_{l, r} = 0$，那么对于任意的划分点 $k$，一定有 $x = y$，也就是说可以随便划分．
2. 若 $s_{l, r} \not= 0$ 时，考虑 $s_{l, r}$ 的最高位 $p$，对于一个划分点 $k$，$x$ 和 $y$ 的第 $p$ 位一定不同，且高于 $p$ 的位数一定相同．也就是说，第 $p$ 位决定了 $x$ 和 $y$ 的大小关系．

那么我们按照区间长度降序遍历所有区间，记 $\mathrm{st}_i$ 表示考察 $i$ 开始的合法区间，每个合法区间的异或和的最高位在 $\mathrm{st}_i$ 中的对应位都是 $1$．也就是说，对于 $\mathrm{st}_i$ 中的每一位 $p$，若该位上为 $1$，那么存在一个合法区间 $[i, x]$，使得 $s_{i, x}$ 的最高位为 $p$．

当我们枚举一个区间 $[l, k]$ 时，我们只需判断 $s_{l, k} \operatorname{and} \mathrm{st}_l$ 是否有值，若有值则 $[l, k]$ 可以被某个区间划分出来．

保留右半边的情况类似．

## 代码

```cpp
#include <cstdio>
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


const int N = 1e4;
const int U = (1ll << 61) - 1;

int n, a[N + 5], s[N + 5];

int stb[N + 5], edb[N + 5];

int highbit(int x) {
	if(!x) return U;
	return 1ll << (63 - __builtin_clzll(x));
}

void solve() {
	n = rd(); for(int i = 1; i <= n; i++) a[i] = rd();
	if(n == 1) return puts("1"), void();
	for(int i = 1; i <= n; i++) s[i] = s[i - 1] ^ a[i];
	stb[1] = edb[n] = highbit(s[n]);
	for(int len = n; len >= 1; len--) {
		for(int l = 1; l + len - 1 <= n; l++) {
			int r = l + len - 1;
			int sum = s[r] ^ s[l - 1];
			int msk = (stb[l] & sum) | (edb[r] & sum);
			int flg = 0;
			if(msk || stb[l] == U || edb[r] == U) {
				stb[l] |= highbit(sum), edb[r] |= highbit(sum);
				flg = 1;
			}
			if(len == 1) printf("%lld", flg);
		}
	}
	puts("");

	for(int i = 1; i <= n; i++) stb[i] = edb[i] = 0;
}

signed main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
