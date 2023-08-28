---
title: Codeforces 1864E Guess Game 题解
date: 2023-08-28 14:51:44
categories: 题解
tags: Trie
---

傻逼没法模拟绝顶聪明的决策，就像 32 位计算机没有 __int128．

<!-- more -->

## 思路

考虑模拟决策过程.

由于二人没有头发，他们会从最高位开始考虑，那么轮数就是 $a$ 和 $b$ 二进制表示的 LCP 中 $1$ 的个数乘 $2$ 加 $1 + [a > b]$．

求期望直接求所有情况的轮数和，然后除 $n^2$．

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

using i64 = long long;
const i64 P = 998244353;

i64 fpow(i64 b, i64 p) {
	i64 res = 1;
	for(; p; b = b * b % P, p >>= 1) {
		if(p & 1) res = res * b % P;
	}
	return res;
}

const int N = 2e5;
const int LOGV = 30;

i64 n;
int ch[N * LOGV + 5][2], alct;
i64 siz[N * LOGV + 5];
void ins(int x) {
	int p = 0;
	for(int i = LOGV; i >= 0; i--) {
		int t = (x >> i) & 1;
		if(!ch[p][t]) ch[p][t] = ++alct;
		p = ch[p][t], siz[p]++;
	}
}

#define lch (ch[u][0])
#define rch (ch[u][1])
i64 ans;
void dfs(int u, int ct) {
	if(lch) dfs(lch, ct);
	if(rch) dfs(rch, ct + 1);
	if(lch && rch) {
		(ans += siz[lch] * siz[rch] % P * (ct + 1) % P) %= P;
		(ans += siz[rch] * siz[lch] % P * (ct + 2) % P) %= P;
	} else if(!lch && !rch) {
		(ans += siz[u] * siz[u] % P * (ct + 1) % P) %= P;
	}
}

void solve() {
	n = rd(); for(int i = 1; i <= n; i++) ins(rd());
	ans = 0, dfs(0, 0);
	printf("%lld\n", ans * fpow(n * n % P, P - 2) % P);

	for(int i = 0; i <= alct; i++) {
		ch[i][0] = ch[i][1] = siz[i] = 0;
	} alct = 0;
}

int main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
