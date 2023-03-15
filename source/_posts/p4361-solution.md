---
title: P4361 [SHOI2015] 激光发生器 题解
date: 2022-12-10 23:32:54
categories:
- 题解
tags:
- 计算几何
---
## 题目大意
光线追踪，但是对于第 $i$ 面镜子，光线的入射角为 $\alpha$ 时，反射角 $\beta=\lambda_i\alpha$，其中 $\lambda_i$ 是给定的常数．

<!--more-->
## 思路
要求出光线的路径上遇到的所有偏转装置的编号，直接递归模拟反射过程即可，难点在于如何计算反射光线．

我们使用 $\mathbf{R}(x)=\mathbf{A}+x\mathbf{b}$ 来表示一条直线，其中 $\mathbf{A}$ 是源点，$\mathbf{b}$ 是方向向量，$x$ 是参数．

对于两条直线 $\mathbf{A}(x)=\mathbf{P}+x\mathbf{u},\mathbf{B}(x)=\mathbf{Q}+x\mathbf{v}$，我们考虑如何求它们间的交点．

若它们之间有交点，则有两实数 $s,t$，满足 $\mathbf{A}(s)=\mathbf{B}(t)$．只要我们计算出 $s$ 或 $t$，我们就知道了交点．以计算 $s$ 为例，我们有：
$$
\mathbf{P}+s\mathbf{u}=\mathbf{Q}+t\mathbf{v}
$$

等式两边点乘上 $\mathbf{v}$ 的法向量 $\mathbf{n}$，得到：
$$
\mathbf{P} \cdot \mathbf{n} + s\mathbf{u} \cdot \mathbf{n} =\mathbf{Q} \cdot \mathbf{n} + \mathbf{0}
$$

整理可得：
$$
s=\frac{\mathbf{Q} \cdot \mathbf{n} - \mathbf{P} \cdot \mathbf{n}}{\mathbf{u} \cdot \mathbf{n}}
$$

$t$ 的计算类似．

记当前入射光线为 $\mathbf{R}(x)=\mathbf{A}+x\mathbf{u}$，偏转装置为 $\mathbf{M}(x)=\mathbf{B}+x\mathbf{v}$．

光线是射线，所以对于 $\mathbf{R}(x)$，应当有 $x>0$．

我们不妨让表示偏转装置的直线的方向向量由线段的一个端点指向另一个端点．这样对于 $\mathbf{M}(x)$，应当有 $0 \le x \le 	1$．

知道了如何对光线和偏转装置求交，我们再考虑如何计算反射光线．

为了计算方便，不妨让反射过程中的法线 $\mathbf{N}(x)=\mathbf{H}+x\mathbf{w}$ 和 $\mathbf{R}(x)$ 在同一侧．如下图所示：

![](/images/p4361-solution/1.png)

这样我们只需要计算出 $-\mathbf{u}$ 到 $\mathbf{w}$ 的转角 $\theta$，反射光线即为 $\mathbf{R}^\prime(x)=\mathbf{H}+x\mathbf{c}$，其中 $\mathbf{c}$ 是 $\mathbf{w}$ 旋转 $\lambda\theta$ 角之后的向量．如下图所示：

![](/images/p4361-solution/2.png)

其中 $\mathbf{a}$ 到 $\mathbf{b}$ 的转角 $\displaystyle\theta=\arcsin\frac{\mathbf{a}\times\mathbf{b}}{|\mathbf{a}||\mathbf{b}|}$．

在模拟光线反射的过程中，对于能够相交的偏转装置，设交点为 $\mathbf{R}(s)$，我们选取最小的 $s$ 所对应的反射光线作为下一次反射的入射光线，这样就能保证这一次反射在离源点最近的反射装置处发生．

## 代码
```cpp
#include <cmath>
#include <iostream>

using f64 = double;
constexpr size_t N = 1 << 7;
constexpr f64 eps = 1e-5;

struct vec {
	f64 x, y;
	vec(f64 x = 0, f64 y = 0) : x(x), y(y) {}
	f64 len() const { return std::sqrt(x * x + y * y); }
};
using point = vec;

vec operator-(const vec &a) { return {-a.x, -a.y}; }
vec operator+(const vec &a, const vec &b) { return {a.x + b.x, a.y + b.y}; }
vec operator-(const vec &a, const vec &b) { return {a.x - b.x, a.y - b.y}; }
vec operator*(const f64 a, const vec &b) { return {a * b.x, a * b.y}; }
vec norm(const vec &a) { return {-a.y, a.x}; }
f64 dot(const vec &a, const vec &b) { return a.x * b.x + a.y * b.y; }
f64 cross(const vec &a, const vec &b) { return a.x * b.y - a.y * b.x; }
f64 angle(const vec &a, const vec &b) { return std::asin(cross(a, b) / (a.len() * b.len())); }
vec rotate(const vec &a, f64 x) {
	return {a.x * std::cos(x) - a.y * std::sin(x), a.x * std::sin(x) + a.y * std::cos(x)};
}
std::istream &operator>>(std::istream &is, vec &v) {
	f64 x, y;
	is >> x >> y, v = {x, y};
	return is;
}

struct ray {
	point ori;
	vec dire;
	ray() : ori(), dire() {}
	ray(const point &ori, const vec &dire) : ori(ori), dire(dire) {}
	point at(f64 x) { return ori + x * dire; }
};

struct refl {
	ray r;
	f64 k;
	refl() : r(), k() {}
	refl(const point &s, const point &t, const f64 k) : r(s, t - s), k(k) {}
	bool hit(const ray &inr, ray &otr, f64 &x) {
		vec m = norm(inr.dire);
		f64 s = dot(m, inr.ori - r.ori) / dot(m, r.dire);
		if (s < eps || s > 1) return false;
		vec n = norm(r.dire);
		if (dot(-inr.dire, n) < 0) n = -n;
		f64 t = dot(n, r.ori - inr.ori) / dot(n, inr.dire);
		if (t < eps) return false;
		x = t, otr = {r.at(s), rotate(n, k * angle(-inr.dire, n))};
		return true;
	}
};

int n;
refl a[N];
point o, d;

int ans[N], tp;
void dfs(const ray &r, int dep) {
	if (dep <= 0) return;
	int id = 0;
	ray rf, tr;
	f64 mn = 1e300, tl;
	for (int i = 1; i <= n; i++) {
		if (a[i].hit(r, tr, tl)) {
			if (tl < mn) id = i, rf = tr, mn = tl;
		}
	}
	if (id) {
		ans[++tp] = id;
		dfs(rf, dep - 1);
	}
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::cin >> o >> d >> n;
	for (int i = 1, x, y; i <= n; i++) {
		point s, t;
		std::cin >> s >> t >> x >> y;
		a[i] = {s, t, f64(x) / y};
	}

	dfs({o, d}, 10);

	if (tp) {
		for (int i = 1; i <= tp; i++) {
			std::cout << ans[i] << " \n"[i == tp];
		}
	} else std::cout << "NONE\n";

	return 0;
}
```
