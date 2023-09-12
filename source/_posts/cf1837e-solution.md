---
title: Codeforces 1837E Playoff Fixing 题解
date: 2023-09-12 21:54:39
categories: 题解
---

IMSB．

<!-- more -->

## 思路

考虑从最上面开始往下推．设最上面为第一层．

考虑安排第 $i$ 层，此时第 $i - 1$ 层已经安排完毕，故只有新增的 $2^{i - 1}$ 个人的位置需要确定，若未钦定任何位置，那么答案需要乘上 $(2^{i - 1})! \times 2^{2^{i - 1}}$，前面那个因子是在考虑新加入的人的排列，后面那个因子是在考虑新加入的人在某场比赛中的先后．

现在某些位置已经确定了，也就是确定了某些人的排列和某些场次的先后，那么减去即可．

## 代码

```cpp
#include <cstdio>
#include <cstring>
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

const int N = 1 << 20;

int k, n, a[N];

i64 fac[N], p2[N];

int flg[N];

int main() {
	memset(a, -1, sizeof(a));

	fac[0] = 1; for(int i = 1; i < N; i++) fac[i] = fac[i - 1] * i % P;
	p2[0] = 1; for(int i = 1; i < N; i++) p2[i] = p2[i - 1] * 2 % P;

	k = rd(), n = 1 << k;
	for(int i = 0; i < n; i++) {
		int x = rd();
		if(x != -1) a[x] = i;
	}
	i64 ans = 1;
	for(int i = 1; i <= k; i++) {
		int f1 = 0, f2 = 0;
		memset(flg, 0, sizeof(flg));
		for(int j = (1 << (i - 1)) + 1; j <= 1 << i; j++) {
			if(a[j] != -1) {
				if(flg[a[j] >> (k - i + 1)] == 1) return puts("0"), 0;
				flg[a[j] >> (k - i + 1)] = 1, f1++;
			}
		}
		for(int j = 1; j <= 1 << (i - 1); j++) {
			if(a[j] != -1) {
				if(flg[a[j] >> (k - i + 1)] == 2) return puts("0"), 0;
				flg[a[j] >> (k - i + 1)] = 2;
			}
		}
		for(int j = 0; j < 1 << (i - 1); j++) f2 += bool(flg[j]);
		(ans *= fac[(1 << (i - 1)) - f1] * p2[(1 << (i - 1)) - f2] % P) %= P;
	}
	printf("%lld\n", ans);
	return 0;
}
```

## 参考

stntn, [_CF1837E Playoff Fixing 题解_](https://www.luogu.com.cn/blog/stntn/solution-cf1837e)
