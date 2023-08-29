---
title: QOJ 687 Topological Ordering 题解
date: 2023-08-29 22:39:50
categories: 题解
tags: 动态规划
---

我是傻逼．

<!-- more -->

## 思路

考虑枚举 $u$，对其他所有 $v$ 计算 $v$ 在 $u$ 之后加入拓扑序的方案数．

设 $f_S$ 表示已经加入了集合 $S$ 中点的拓扑序数量，$g_S$ 表示从全集开始删到只剩 $S$ 中点的拓扑序数量．这个可以通过简单的 DP 得到．

计算 $\mathrm{ans}_{u, v}$ 时，枚举加入 $v$ 前的点集，我们有：

$$
\mathrm{ans}_{u, v} = \sum_{u \in S, v \not\in S} f_S \times g_{S \cup \{v\}}
$$

时间复杂度 $O(2^n n^2)$，卡卡常就过去了．

## 代码

先咕着．
