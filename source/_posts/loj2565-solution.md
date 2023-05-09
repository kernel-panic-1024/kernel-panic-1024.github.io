---
title: LOJ 2565 「SDOI2018」旧试题 题解
date: 2023-05-09 22:58:43
categories: 题解
tags:
  - Mobius 反演
  - 三元环计数
---

大受震撼．

## 思路

首先我们有 $\sigma_0(xyz) = \sum\limits_{i | x} \sum\limits_{j | y} \sum\limits_{k | z} [i \perp j][j \perp k][i \perp k]$．

然后开始大力推柿子：

$$
\def\flr#1{\left\lfloor #1 \right\rfloor}
\begin{aligned}
  \sum_{i = 1}^a \sum_{j = 1}^b \sum_{k = 1}^c \sigma_0(ijk)
  &= \sum_{i = 1}^a \sum_{j = 1}^b \sum_{k = 1}^c \sum_{x | i} \sum_{y | j} \sum_{z | k} [x \perp y][y \perp z][x \perp z] \\
  &= \sum_{x = 1}^a \sum_{y = 1}^b \sum_{z = 1}^c [x \perp y][y \perp z][x \perp z] \flr{a / x} \flr{b / y} \flr{c / z} \\
  &= \sum_{x = 1}^a \sum_{y = 1}^b \sum_{z = 1}^c \sum_{r | (x, y)} \mu(r) \sum_{s | (y, z)} \mu(s) \sum_{t | (x, z)} \mu(t) \flr{a / x} \flr{b / y} \flr{c / z} \\
  &= \sum_{r = 1}^{\min\{a, b\}} \mu(r) \sum_{s = 1}^{\min\{b, c\}} \mu(s) \sum_{t = 1}^{\min\{a, c\}} \mu(t) \sum_{[r, t] | x} \flr{a / x} \sum_{[r, s] | y} \flr{b / y} \sum_{[s, t] | z} \flr{c / z} \\
\end{aligned}
$$

定义 $f_a(x) = \sum\limits_{x | i} \lfloor a / i \rfloor$，显然可以预处理 $f_a$、$f_b$、$f_c$．那么我们要求的就是：

$$
\sum_{r = 1}^{\min\{a, b\}} \sum_{s = 1}^{\min\{b, c\}} \sum_{t = 1}^{\min\{a, c\}} \mu(r)\mu(s)\mu(t)f_a([r, t])f_b([r, s])f_c([s, t])
$$

然后你会发现，我们反演了一通，计算复杂度没变．~~止步于此．~~

咋办？考虑直接枚举对答案贡献不为 $0$ 的三元组 $(r, s, t)$．由于 $\mu(x)$ 很可能为 $0$，而 $f_a(x)$ 在 $x > a$ 时也为 $0$，直觉告诉我们这样的三元组不会很多．

有一种枚举这样的三元组的方式：考虑一张无向图 $G = (V, E)$，对于所有的 $u, v$，若 $\mu(u) \not= 0, \mu(v) \not= 0, [u, v] \le \max\{a, b, c\}$，那么 $(u, v) \in E$，这样的 $G$ 中的每个三元环都对应一个贡献不为 $0$ 且元素互异的三元组．跑一遍三元环计数即可获得所有元素互异的三元组造成的贡献．对于相同元素造成的贡献，拉出来单独统计即可．

实现上，为了避免多次求 $[u, v]$ 的开销，我们枚举一个数 $g$ 充当 $(u, v)$，再枚举 $u = ig, v = jg$，使得 $i \perp j$，此时有 $[u, v] = ijg$．

## 代码

太丑了，就不放了．

## 参考

[GKxx, _题解 P4619_](https://www.luogu.com.cn/blog/gkxx-is-here/solution-p4619)
