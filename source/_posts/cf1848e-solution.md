---
title: Codeforces 1848E Vika and Stone Skipping 题解
date: 2023-09-07 15:53:10
categories: 题解
---

不是这场怎么连着 $3$ 个数学题啊．

和 RUS 人不共戴天．

<!-- more -->

## 思路

设 $x = f + (f - 1) + \cdots + (f - c + 1) = c(f + f - c + 1) / 2$，即以某个力 $f$ 丢出石子后，第 $c$ 次接触水面在 $x$ 处．

- 若 $c$ 是偶数，那么 $x = (c / 2)(2f - c + 1) = p(2k + 1)$，由于有 $f - c + 1 > 0$，所以 $p \le k$．
- 若 $c$ 是奇数，那么 $x = c(f - (c - 1) / 2) = (2k + 1)p$，此时有 $p > k$．

所以 $x$ 的每个奇因子 $2k + 1$ 都唯一对应一个 $p$，也唯一对应一个 $f$，对答案贡献恰好 $1$．那么问题转化为快速计算 $x$ 的奇因子数目．考虑 $x$ 的唯一分解 $x = \prod {p_i}^{a_i}$，那么答案就是 $\prod\limits_{p_i \not= 2} (a_i + 1)$．

由于直接维护会涉及逆元操作，而模数不是大质数，逆元不一定总存在，那么直接使用大炮打蚊子，拿个线段树维护．

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

const int N = 1e6;

int x, q; i64 P;

int mxp[N + 5], pri[N + 5], pid[N + 5], pct;
void pre() {
	for(int i = 2; i <= N; i++) {
		if(mxp[i]) continue;
		pri[++pct] = i, pid[i] = pct;
		for(int j = 1; j * i <= N; j++) {
			mxp[j * i] = i;
		}
	}
}

#define lch (p * 2)
#define rch (p * 2 + 1)
#define mid ((cl + cr) / 2)
i64 prod[N * 4 + 5];
void pushup(int p) { prod[p] = prod[lch] * prod[rch] % P; }
void build(int p = 1, int cl = 2, int cr = N) {
	if(cl == cr) return prod[p] = 1, void();
	build(lch, cl, mid), build(rch, mid + 1, cr);
	pushup(p);
}
void upd(int x, int v, int p = 1, int cl = 2, int cr = N) {
	if(cl == cr) return (prod[p] += v) %= P, void();
	(x <= mid) ? upd(x, v, lch, cl, mid) : upd(x, v, rch, mid + 1, cr);
	pushup(p);
}
#undef lch
#undef rch
#undef mid

int main() {
	pre();

	x = rd(), q = rd(), P = rd();
	build();
	for(int i = 1; i <= pct; i++) {
		int ct = 0;
		while(x % pri[i] == 0) {
			x /= pri[i];
			ct++;
		}
		if(i > 1 && ct) upd(i, ct);
	}
	int flg = 1;
	if(x != 1) flg++;
	for(int _ = 1; _ <= q; _++) {
		int y = rd();
		while(y != 1) {
			int t = mxp[y]; y /= t;
			if(pid[t] >= 2) upd(pid[t], 1);
		}
		printf("%lld\n", prod[1] * flg % P);
	}
	return 0;
}
```
