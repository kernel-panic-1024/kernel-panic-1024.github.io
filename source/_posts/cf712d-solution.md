---
title: Codeforces 712D Memory and Scores 题解
date: 2023-09-12 20:47:05
categories: 题解
---

我是菜鸡．

<!-- more -->

## 思路

设 $f_{i, j}$ 表示进行了 $i$ 轮，分数差为 $j$ 时的局面数．

然而这样直接转移的话会带个诡异系数，不是很好优化，考虑直接玩 $2t$ 轮，每一轮只有一个人操作，那么有 $f_{i, j} \leftarrow f_{i - 1, p}$，其中 $p \in [j - k, j + k]$．

## 代码

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
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
const i64 P = 1e9 + 7;

const int N = 6e5;
const int B = 3e5;

int a, b, k, t;
int f[2][N + 5];

int main() {
	a = rd(), b = rd(), k = rd(), t = rd();
	int lim = 2 * k * t + abs(a - b);
	f[0][B + a - b] = 1;
	for(int i = -lim; i <= lim; i++) {
		(f[0][B + i] += f[0][B + i - 1]) %= P;
	}
	for(int i = 1; i <= t * 2; i++) {
		memset(f[i & 1], 0, sizeof(f[i & 1]));
		for(int j = -lim; j <= lim; j++) {
			int l = std::max(-lim, j - k), r = std::min(lim, j + k);
			f[i & 1][B + j] = (f[(i - 1) & 1][B + r] - f[(i - 1) & 1][B + l - 1] + P) % P;
		}
		for(int j = -lim; j <= lim; j++) {
			(f[i & 1][B + j] += f[i & 1][B + j - 1]) %= P;
		}
	}
	printf("%lld\n", (f[0][B + lim] - f[0][B] + P) % P);
	return 0;
}
```
