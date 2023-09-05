---
title: Codeforces 1796D Maximum Subarray 题解
date: 2023-09-05 13:25:49
categories: 题解
---

*2000 评绿是吧．

这就是模拟赛带走我的自信．

<!-- more -->

## 思路

贪不动，尝试 DP．

先将所有 $a_i$ 减去 $x$，问题转化为要将原序列中某些位置加上 $2x$，最大化最大子段和．

考虑设 $f_{i, j}$ 表示在 $i$ 处结束的子段，加了 $j$ 次的最大和．

转移需要考虑当前点改不改，由于我们可以在 $i$ 之后的某些位置操作，所以取答案时，需要对所有满足 $1 \le i \le n, \max\{k - (n - i)\} \le j \le k$ 的 $f_{i, j}$ 取 $\max$．

注意不要转移到非法状态．注意不要转移到非法状态．注意不要转移到非法状态．

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

const int N = 2e5;
const int K = 20;

int n, k, x, a[N + 5];
int f[N + 5][K + 5];

void solve() {
	n = rd(), k = rd(), x = rd(); for(int i = 1; i <= n; i++) a[i] = rd() - x;
	for(int i = 0; i <= n; i++) for(int j = 0; j <= k; j++) f[i][j] = 0;
	for(int i = 1; i <= n; i++) {
		f[i][0] = std::max(f[i][0], f[i - 1][0] + a[i]);
		for(int j = 1; j <= std::min(i, k); j++) {
			if(i == j) f[i][j] = std::max(f[i][j], f[i - 1][j - 1] + a[i] + 2 * x);
			else f[i][j] = std::max({f[i][j], f[i - 1][j] + a[i], f[i - 1][j - 1] + a[i] + 2 * x});
		}
	}
	int ans = 0;
	for(int i = 1; i <= n; i++) {
		for(int j = k; j >= std::max(0ll, k - (n - i)); j--) ans = std::max(ans, f[i][j]);
	}
	printf("%lld\n", ans);
}

signed main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
