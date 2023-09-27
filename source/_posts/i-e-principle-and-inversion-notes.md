---
title: 容斥原理与反演学习笔记
date: 2023-09-26 20:06:16
categories: 学习笔记
---

往简中互联网塞点垃圾．

鉴定为民科．

WIP．

<!-- more-->

## 容斥原理

省流：小学数学．

来看一道水题！给你拥有 A，B，C，AB，BC，AC，ABC 属性的物体个数，求总物品数．

小学老师会告诉你，我们加上有一个属性的，减去有两个属性的，再加上有三个属性的就是答案．容易验证这是正确的！

尝试扩展一下．我们现在有属性全集 $U$，给你拥有集合 $S$ 中属性的物品数量 $f_S$，求总物品数．

延续上面的思路，我们可以猜测答案是

$$
\sum_{S \subseteq U} (-1)^{|S| + 1} f_S
$$

容易验证这也是对的，但是正确性并不直观．延续之前的思路按图索骥能够猜出系数是 $(-1)^{|S| + 1}$，但是这对解决一般的问题并无益处，考虑如何算出这个系数．

设 $c_i$ 表示集合大小为 $i$ 时的容斥系数．我们希望对于所有 $x$，属性集合大小为 $x$ 的元素只会被计算一次，即：

$$
\forall x \le |U|, \sum_{i = 1}^x \binom{x}{i} c_i = 1
$$

此时带入 $(-1)^{i + 1}$ 的正确性就可以通过二项式定理轻易得出了！但是这一系数的得出仍然不自然，难以扩展到其他限制．若系数能够通过简单的递推求出就皆大欢喜，但不能的话就需要一些其他工具了．

## 反演

### 本质

反演指的是改变表示的方向，即 $f$ 表示 $g$ $\to$ $g$ 表示 $f$．

假设我们已知

$$
g_n = \sum_{i = 1}^n a_{n, i} f_i
$$

且求出了一组系数 $b_{i, j}$ 使得

$$
f_n = \sum_{i = 1}^n b_{n, i} g_i
$$

就能在知道 $f, g$ 其中一个的情况下求出另外一个．这一过程称作反演，上述两式互为反演公式．

### 第一反演公式

## 一些经典的反演 / 容斥系数

### 二项式反演

### 子集反演

### Mobius 反演

### Euler 反演

### Min-Max 容斥

### 单位根反演

### Stirling 反演

## 参考

《具体数学》5.1, 6.1

Alex_Wei, [_组合数学相关_](https://www.cnblogs.com/alex-wei/p/Combinatorial_Mathematics.html)

Alex_Wei, [_反演与狄利克雷卷积_](https://www.cnblogs.com/alex-wei/p/Dirichlet.html)

VFleaKing, [_炫酷反演魔术_](https://vfleaking.blog.uoj.ac/slide/87)

command_block, [_炫酷反演魔术_](https://www.luogu.com.cn/blog/command-block/xuan-ku-fan-yan-mo-shu)

RenaMoe, [_容斥原理学习笔记_](https://renamoe.gitee.io/2021/04/08/容斥原理学习笔记/)

RenaMoe, [_笔记 各类反演_](https://renamoe.gitee.io/2021/03/11/笔记-各类反演/)

Deadecho, [_容斥原理，容斥系数_](https://www.cnblogs.com/gzy-cjoier/p/9686787.html)
