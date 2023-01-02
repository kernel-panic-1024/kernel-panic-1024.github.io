---
title: CF1458C Latin Square 题解
date: 2022-10-26 19:05:43
categories:
- 题解
sticky: 1
---
模拟赛考了这题，赛时想出了一个用矩阵乘法的屑做法，常数巨大，没想到还跑的挺快。

<!--more-->

## 思路
首先为了方便操作，我们让矩阵的行列编号都从 $0$ 开始，并且让所有元素都减 $1$。

前 $4$ 个操作显然可以用矩阵表示。我们把 $1$ 个元素 $a_{i,j}$ 在矩阵中的位置表示成向量 $\begin{bmatrix} i & j & 1 \end{bmatrix}$，变换矩阵如下：
$$
R=
\begin{bmatrix}
1 & 0 & 0 \\
0 & 1 & 0 \\
0 & 1 & 1 \\
\end{bmatrix}
,
L=
\begin{bmatrix}
1 & 0 & 0 \\
0 & 1 & 0 \\
0 & -1 & 1 \\
\end{bmatrix}
,
D=
\begin{bmatrix}
1 & 0 & 0 \\
0 & 1 & 0 \\
1 & 0 & 1 \\
\end{bmatrix}
,
U=
\begin{bmatrix}
1 & 0 & 0 \\
0 & 1 & 0 \\
-1 & 0 & 1 \\
\end{bmatrix}
$$

考虑后 $2$ 个操作，不难发现对于 $1$ 个元素 $a_{i,j}$，对其施加 $\texttt{I}$ 变换的本质是将其列编号和值互换。$\texttt{C}$ 变换同理。

所以我们最终将 $a_{i,j}$ 表示成向量 $\begin{bmatrix} a_{i,j} & i & j & 1 \end{bmatrix}$，容易写出所有的变换矩阵：
$$
\begin{aligned}
  & R=
  \begin{bmatrix}
  1 & 0 & 0 & 0 \\
  0 & 1 & 0 & 0 \\
  0 & 0 & 1 & 0 \\
  0 & 0 & 1 & 1 \\
  \end{bmatrix}
  ,
  L=
  \begin{bmatrix}
  1 & 0 & 0 & 0 \\
  0 & 1 & 0 & 0 \\
  0 & 0 & 1 & 0 \\
  0 & 0 & -1 & 1 \\
  \end{bmatrix}
  ,
  D=
  \begin{bmatrix}
  1 & 0 & 0 & 0 \\
  0 & 1 & 0 & 0 \\
  0 & 0 & 1 & 0 \\
  0 & 1 & 0 & 1 \\
  \end{bmatrix}
  ,
  U=
  \begin{bmatrix}
  1 & 0 & 0 & 0 \\
  0 & 1 & 0 & 0 \\
  0 & 0 & 1 & 0 \\
  0 & -1 & 0 & 1 \\
  \end{bmatrix}
  , \\
  &I=
  \begin{bmatrix}
  0 & 0 & 1 & 0 \\
  0 & 1 & 0 & 0 \\
  1 & 0 & 0 & 0 \\
  0 & 0 & 0 & 1 \\
  \end{bmatrix}
  ,
  C=
  \begin{bmatrix}
  0 & 1 & 0 & 0 \\
  1 & 0 & 0 & 0 \\
  0 & 0 & 1 & 0 \\
  0 & 0 & 0 & 1 \\
  \end{bmatrix}
\end{aligned}
$$

对于操作序列，我们构造出矩阵后直接累乘。乘法在模 $n$ 意义下进行。

设变换完的矩阵为 $b$。对于 $a_{i,j}$，我们构造向量 $\begin{bmatrix} a_{i,j} & i & j & 1 \end{bmatrix}$，设乘上变换矩阵后的向量为  $\begin{bmatrix} c & x & y & 1 \end{bmatrix}$，让 $b_{x,y}\leftarrow c$ 即可。

输出时记得加 $1$。

时间复杂度 $O(nm)$。

## 代码
```cpp
#include <cstdint>
#include <iostream>

using i64 = int64_t;
constexpr size_t N = 1 << 10;
constexpr size_t M = 1 << 20;

int n, m;

struct mat {
    i64 _m[4][4];
    constexpr mat() : _m{} {}
    constexpr i64 &operator()(int i, int j) { return _m[i - 1][j - 1]; }
    constexpr const i64 &operator()(int i, int j) const { return _m[i - 1][j - 1]; }
    friend mat operator*(const mat &A, const mat &B) {
        mat R;
        for (int i = 1; i <= 4; i++) {
            for (int k = 1; k <= 4; k++) {
                for (int j = 1; j <= 4; j++) { R(i, j) = (R(i, j) + A(i, k) * B(k, j) % n + n) % n; }
            }
        }
        return R;
    }
    constexpr static mat R() {
        mat R;
        R(1, 1) = 1;
        R(2, 2) = 1;
        R(3, 3) = 1;
        R(4, 3) = R(4, 4) = 1;
        return R;
    }
    constexpr static mat L() {
        mat R;
        R(1, 1) = 1;
        R(2, 2) = 1;
        R(3, 3) = 1;
        R(4, 3) = -1, R(4, 4) = 1;
        return R;
    }
    constexpr static mat D() {
        mat R;
        R(1, 1) = 1;
        R(2, 2) = 1;
        R(3, 3) = 1;
        R(4, 2) = R(4, 4) = 1;
        return R;
    }
    constexpr static mat U() {
        mat R;
        R(1, 1) = 1;
        R(2, 2) = 1;
        R(3, 3) = 1;
        R(4, 2) = -1, R(4, 4) = 1;
        return R;
    }
    constexpr static mat I() {
        mat R;
        R(1, 3) = 1;
        R(2, 2) = 1;
        R(3, 1) = 1;
        R(4, 4) = 1;
        return R;
    }
    constexpr static mat C() {
        mat R;
        R(1, 2) = 1;
        R(2, 1) = 1;
        R(3, 3) = 1;
        R(4, 4) = 1;
        return R;
    }
    constexpr static mat E() {
        mat R;
        for (int i = 1; i <= 4; i++) R(i, i) = 1;
        return R;
    }
};

int a[N][N], b[N][N];
mat R = mat::R();
mat L = mat::L();
mat D = mat::D();
mat U = mat::U();
mat I = mat::I();
mat C = mat::C();

char opt[M];

int T;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin >> T;
    while (T--) {
        mat A = mat::E();
        std::cin >> n >> m;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                std::cin >> a[i][j];
                a[i][j] -= 1;
            }
        }
        std::cin >> opt;
        for (int i = 0; i < m; i++) {
            if (opt[i] == 'R') A = A * R;
            if (opt[i] == 'L') A = A * L;
            if (opt[i] == 'D') A = A * D;
            if (opt[i] == 'U') A = A * U;
            if (opt[i] == 'I') A = A * I;
            if (opt[i] == 'C') A = A * C;
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int a = ((::a[i][j] * A(1, 1) + i * A(2, 1) + j * A(3, 1) + A(4, 1)) % n + n) % n;
                int x = ((::a[i][j] * A(1, 2) + i * A(2, 2) + j * A(3, 2) + A(4, 2)) % n + n) % n;
                int y = ((::a[i][j] * A(1, 3) + i * A(2, 3) + j * A(3, 3) + A(4, 3)) % n + n) % n;
                b[x][y] = a;
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) { std::cout << b[i][j] + 1 << " \n"[j == n - 1]; }
        }
        std::cout << "\n";
    }
    return 0;
}
```
