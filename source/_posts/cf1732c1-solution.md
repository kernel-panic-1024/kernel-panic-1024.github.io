---
title: CF1732C1 Sheikh (Easy version) 题解
date: 2022-12-28 22:27:31
description: 
categories:
- 题解
tags:
- 位运算
- 二分
---
## 题目大意
给你 $1$ 个长度为 $n$ 的序列 $\{a_i\}$，我们定义 $1$ 个区间 $a_{l\cdots r}$ 的价值为 $\sum_{i=l}^{r}a_i-\bigoplus_{i=l}^{r}a_i$．

你需要选择 $1$ 个区间 $a_{l\cdots r}$，最大化价值的同时，最小化区间长度．

其中 $\oplus$ 代表按位异或运算．

<!-- more -->

## 思路
由于异或可以看作 $2$ 进制不进位加法，我们可以发现当区间长度增加 $1$ 时，其价值必然单调不减．

那么最后的最大价值必然是 $a_{1\cdots n}$ 的价值，所以我们只需要最小化区间长度．

只有 $1$ 组询问，可以想到枚举最终的区间．而答案具有单调性，所以我们可以枚举区间左端点，二分区间右端点，每次 `check` 检查当前区间的价值是否仍然等于 $a_{1\cdots n}$ 的价值即可．

预处理前缀和即可 $O(1)$ 进行每次的 `check`．

总时间复杂度 $O(n\log n)$．

## 代码
```cpp
#include <cstdint>
#include <iostream>

using i64 = int64_t;
constexpr size_t N = 1 << 20;

int T, n, q, L, R;
i64 ss[N], sx[N], a[N];

bool check(int l, int r, i64 ans) { return (ss[r] - ss[l - 1]) - (sx[r] ^ sx[l - 1]) == ans; }

int main() {
    std::ios::sync_with_stdio(false);
    std::cin >> T;
    while (T--) {
        std::cin >> n >> q;
        for (int i = 1; i <= n; i++) std::cin >> a[i];
        for (int i = 1; i <= n; i++) ss[i] = ss[i - 1] + a[i], sx[i] = sx[i - 1] ^ a[i];
        std::cin >> L >> R;
        i64 ans = (ss[R] - ss[L - 1]) - (sx[R] ^ sx[L - 1]);
        int cl = L, cr = R;
        for (int i = L; i <= R; i++) {
            int l = i, r = R, res = 0;
            while (l <= r) {
                int mid = (l + r) / 2;
                if (check(i, mid, ans)) r = mid - 1, res = mid;
                else l = mid + 1;
            }
            if (res == 0) continue;
            if (cr - cl + 1 > res - i + 1) cl = i, cr = res;
        }
        std::cout << cl << " " << cr << "\n";
    }
    return 0;
}
```
