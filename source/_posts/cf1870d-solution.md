---
title: Codeforces 1870D Prefix Purchase 题解
date: 2023-09-23 14:48:19
categories: 题解
tags:
  - 贪心
  - 调整
---

线性！线性！

<!-- more -->

## 思路

考虑选择序列中价格最小的位置 $i$，然后买 $\lfloor k / c_i \rfloor$ 次，这样显然给出了操作完后 $a_1$ 可能的最大值．

接下来考虑在不动 $a_1$ 的情况下调整使得字典序最大．设上次调整完后剩余的钱数为 $r$，上次买的位置是 $p$，我们找到 $[p + 1, n]$ 中的最小值 $c_q$，然后卖掉 $\lfloor r / (c_q - c_p) \rfloor$ 个 $p$ 后加钱买个数相等的 $q$，这样显然不会改变 $a_1$ 的值，由于 $q$ 在 $p$ 后边，所以会增大字典序．由于每次选的是后缀最小值，所以调整过程一定能结束．

此时已经可以利用优先队列做到 $O(n \log n)$，但是注意到每次选的是最靠后的后缀最小值，可以直接预处理做到 $O(n)$．

## 代码

```cpp
#include <cstdio>
#include <queue>
#define debug(...) fprintf(stderr, __VA_ARGS__)

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

const int N = 2e5;

int n, k, a[N + 5];

struct node { int v, p; } suf[N + 5];
bool operator<(node a, node b) {
	return a.v != b.v ? a.v < b.v : a.p > b.p;
}

int b[N + 5];

void solve() {
	n = rd(); for(int i = 1; i <= n; i++) a[i] = rd();
	k = rd();

	for(int i = 1; i <= n; i++) suf[i] = {a[i], i};
	for(int i = n - 1; i >= 1; i--) suf[i] = std::min(suf[i], suf[i + 1]);
	int pre = 0;
	while(k && pre + 1 <= n) {
		auto [v, p] = suf[pre + 1];
		if(k < v - a[pre]) break;
		int used = k / (v - a[pre]);
		if(pre) used = std::min(b[pre], used);
		b[p] += used, b[pre] -= used, k -= (v - a[pre]) * used;
		pre = p;
	}
	for(int i = n; i >= 1; i--) b[i] += b[i + 1];
	for(int i = 1; i <= n; i++) printf("%d%c", b[i], " \n"[i == n]);

	for(int i = 1; i <= n; i++) b[i] = 0;
}

int main() {
	int T = rd();
	while(T--) solve();
	return 0;
}
```
