---
title: Codrforces 1375G Tree Modification 题解
date: 2023-05-19 15:33:00
categories:
---

喵喵题．

<!-- more -->

## 思路

由于树是一个二分图，考虑将这个二分图的两个部分染成黑白两色．如果我们对 $a, b, c$ 进行一次操作，不妨假设操作前 $a, c$ 是白色，$b$ 是黑色，那么由于操作后 $a$ 连到了 $c$ 上，其颜色改变，其他节点颜色不变．

考虑对菊花图染色，发现其中有一种颜色只有一个节点，设原树上白色节点数为 $w$，黑色节点数目为 $b$，答案即为 $\min\{w, b\} - 1$．

~~以后遇到树上诡异操作可以尝试染色后观察性质．~~

## 代码
```cpp
#include <cstdio>
#include <vector>
#include <algorithm>

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

const int N = 2e5;

int n;
std::vector<int> g[N + 5];

int ct[2];
void dfs(int u, int fa, int col) {
	ct[col]++;
	for(int v : g[u]) {
		if(v == fa) continue;
		dfs(v, u, col ^ 1);
	}
}

int main() {
	n = rd();
	for(int i = 1; i <= n - 1; i++) {
		int u = rd(), v = rd();
		g[u].push_back(v); g[v].push_back(u);
	}
	dfs(1, 0, 0);
	printf("%d\n", std::min(ct[0], ct[1]) - 1);
	return 0;
}
```