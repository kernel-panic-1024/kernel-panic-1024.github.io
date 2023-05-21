---
title: 洛谷 3317 [SDOI2014] 重建 题解
date: 2023-05-20 13:12:14
categories: 题解
tags:
  - 矩阵树定理
---

/zhq．

<!-- more -->

## 思路

生成树相关计数，考虑矩阵树定理．

矩阵树定理是用于求解形如下式的问题的：

$$
\sum_{T} \prod_{e \in T} w_e
$$

其中 $T$ 取遍图中所有生成树的边集，$w_e$ 是 $e$ 这条边的边权．

设 $p_{e}$ 为 $e$ 这条边存在的概率，对于这道题，我们要求的是：

$$
\sum_{T} \left(\prod_{e \in T} p_e\right) \times \left(\prod_{e \not\in T} 1 - p_e\right)
$$

发现右边的 $\prod$ 很烦人，它阻止我们直接使用矩阵树定理．考虑怎么让它与 $\sum$ 无关．提一个 $\prod\limits_e 1 - p_e$ 出来，式子可化为：

$$
\left(\prod_e 1 - p_e\right) \times \sum_{T} \prod_{e \in T} \frac{p_e}{1 - p_e}
$$

左边的 $\prod$ 可以直接算，右边的 $\sum$ 使用矩阵树定理．

注意到存在 $p_e = 1$ 的情况，直接做除法会除出来 inf，给分母加个微小扰动即可．

## 代码

```cpp
#include <cstdio>
#include <cmath>
#include <algorithm>

using f64 = double;

const int N = 50;
const f64 eps = 1e-9;

int n; f64 a[N + 5][N + 5];
int ct[N + 5];

int d;
f64 det() {
	int sgn = 1;
	for(int i = 1; i <= d; i++) {
		int p = i;
		for(int j = i + 1; j <= d; j++) {
			if(a[j][i] > a[p][i]) p = j;
		}
		if(p != i) sgn *= -1, std::swap(a[i], a[p]);
		for(int j = 1; j <= d; j++) {
			if(i == j) continue;
			f64 x = a[j][i] / a[i][i];
			for(int k = 1; k <= d; k++) {
				a[j][k] -= x * a[i][k];
			}
		}
	}
	f64 res = 1;
	for(int i = 1; i <= d; i++) res *= a[i][i];
	return res * sgn;
}

int main() {
	scanf("%d", &n);
	f64 rev = 1;
	for(int i = 1; i <= n; i++) {
		for(int j = 1; j <= n; j++) {
			scanf("%lf", &a[i][j]);
		}
	}

	for(int i = 1; i <= n; i++) {
		for(int j = i + 1; j <= n; j++) {
			rev *= (1 - a[i][j] + eps);
		}
	}
	for(int i = 1; i <= n; i++) {
		for(int j = 1; j <= n; j++) {
			a[i][j] /= (1 - a[i][j] + eps);
		}
	}
	for(int i = 1; i <= n; i++) {
		for(int j = 1; j <= n; j++) {
			if(i == j) continue;
			a[i][i] += a[i][j];
		}
	}
	for(int i = 1; i <= n; i++) {
		for(int j = 1; j <= n; j++) {
			if(i == j) continue;
			a[i][j] = -a[i][j];
		}
	}
	for(int i = 1; i <= n; i++) {
		for(int j = 1; j <= n; j++) {
			a[i][j] = a[i + 1][j + 1];
		}
	}

	d = n - 1;
	printf("%.15lf\n", det() * rev);
	return 0;
}
```
