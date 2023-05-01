---
title: 洛谷 1446 [HNOI2008] Cards 题解
date: 2023-05-02 06:41:30
categories: 题解
tag:
  - 群论
  - Burnside 引理
  - 动态规划
---

## 题面

### 题目描述

小春现在很清闲，面对书桌上的 $n$ 张牌，他决定给每张牌染色，目前小春拥有 $3$ 种颜色：红色，蓝色，绿色．他询问 Sun 有多少种染色方案，Sun 很快就给出了答案．

进一步，小春要求染出 $S_r$ 张红色，$S_b$ 张蓝色，$S_g$ 张绿色．他又询问有多少种方案，Sun 想了一下，又给出了正确答案．最后小春发明了 $m$ 种不同的洗牌法，这里他又问 Sun 有多少种不同的染色方案．两种染色方法相同当且仅当其中一种可以通过任意的洗牌法（即可以使用多种洗牌法，而每种方法可以使用多次）洗成另一种．

Sun 发现这个问题有点难度，决定交给你，由于答案可能很大，你只需要求出答案对于 $P$ 取模的结果． 保证 $P$ 为一个质数．

输入数据保证任意多次洗牌都可用这 $m$ 种洗牌法中的一种代替，且对每种洗牌法，都存在一种洗牌法使得能回到原状态．

### 数据范围

$1 \le m \le 60$，$m + 1 < P < 100$，$\max\{S_r, S_b, S_g\} \le 20$．

<!-- more -->

## 思路

注意到输入对洗牌法的限制，我们如果将每一种洗牌法看作一个置换，那么当加入单位置换 $\begin{pmatrix} 1 & 2 & \cdots & n \\ 1 & 2 & \cdots & n \end{pmatrix}$ 后，这 $m + 1$ 种置换就对置换的复合构成群．

又能通过洗牌法洗成一样的两个牌堆相同，将置换作用在牌堆上看作是一种群作用，问题转化成了对置换群的轨道计数．

考虑使用 Burnside 引理，我们只需要计算出每个置换下不变的染色方案数目．考虑一个置换中的所有轮换，一个轮换中的元素必定只能被染成一种颜色．那么我们可以使用 DP 来计算方案数．

对于一个置换，枚举其中的轮换，记当前轮换的长度为 $s$，设 $f_{r, b, g}$ 为三种颜色各使用 $r$、$b$、$g$ 个时的方案数，转移方程如下：

$$
f_{r, b, g} = f_{r - s, b, g} + f_{r, b - s, g} + f_{r, b, g - s}
$$

对于所有排列，计算出不动点数目后求和，最后除以群的大小 $m + 1$．

## 代码

```cpp
#include <cstdio>
#include <cstring>

static int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

const int N = 100;

int sr, sb, sg, n, m, P;

int fpow(int b, int p) {
	int res = 1;
	for (; p; b = b * b % P, p >>= 1) {
		if (p & 1) res = res * b % P;
	}
	return res;
}

int p[N + 5], ct, siz[N + 5], vis[N + 5];
int f[N + 5][N + 5][N + 5];
void clear() {
	memset(siz, 0, sizeof(siz));
	memset(vis, 0, sizeof(vis));
	memset(f, 0, sizeof(f));
	ct = 0;
}
int calc() {
	clear();
	for (int i = 1; i <= n; i++) {
		if (vis[i]) continue;
		ct++;
		for (int cur = i; !vis[cur]; cur = p[cur]) {
			siz[ct]++, vis[cur] = 1;
		}
	}
	f[0][0][0] = 1;
	for (int i = 1; i <= ct; i++) {
		for (int r = sr; r >= 0; r--) {
			for (int b = sb; b >= 0; b--) {
				for (int g = sg; g >= 0; g--) {
					if (r >= siz[i]) (f[r][b][g] += f[r - siz[i]][b][g]) %= P;
					if (b >= siz[i]) (f[r][b][g] += f[r][b - siz[i]][g]) %= P;
					if (g >= siz[i]) (f[r][b][g] += f[r][b][g - siz[i]]) %= P;
				}
			}
		}
	}
	return f[sr][sb][sg];
}

int main() {
	sr = rd(), sb = rd(), sg = rd();
	n = sr + sg + sb;
	m = rd(), P = rd();

	for (int i = 1; i <= n; i++) p[i] = i;
	int ans = calc();
	for (int i = 1; i <= m; i++) {
		for (int i = 1; i <= n; i++) p[i] = rd();
		ans = (ans + calc()) % P;
	}

	printf("%d\n", ans * fpow(m + 1, P - 2) % P);

	return 0;
}
```
