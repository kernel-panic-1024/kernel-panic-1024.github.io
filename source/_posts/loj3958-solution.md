---
title: LOJ 3958 「联合省选 2023」过河卒 题解
date: 2023-05-07 20:27:31
categories: 题解
tags:
  - 博弈论
---

遇到这题之前根本没做过有向图博弈．寄大了．

<!-- more -->

## 思路

发现 $n, m \le 10$，这说明总状态数量不超过 ${10}^6$，允许我们枚举所有状态．建出博弈图即可得到答案．

具体地，将每个状态与它能移动到的所有状态建有向边，我们定义 $\text{P}$ 态为必输态，$\text{N}$ 态为必胜态．博弈图有以下性质：

1. 结束状态是 $\text{P}$ 态．
2. 可以移动到 $\text{P}$ 态的状态都是 $\text{N}$ 态．
3. 只能移动到 $\text{N}$ 态的状态是 $\text{P}$ 态．

那么，我们用 $6$ 元组 $(a, b, c, d, e, f)$ 来表示红子在 $(a, b)$、$(c, d)$，黑子在 $(e, f)$ 的状态．枚举所有可能的状态和该状态的所有转移，反向建边之后拓扑排序，按照定义递推输赢状态和对应步数，即可获得答案．时间复杂度 $O(n^3m^3)$．

实现时有以下注意事项：

1. 多测．
2. 两个红子碰撞不是合法状态．
3. 黑子到达底线或与红子碰撞是结束状态．
4. 可以将 $6$ 元组状压成一个不超过 $24$ 位的二进制数，用来作为储存编号到 $6$ 元组映射的数组的下标．
5. 可以发现，对于一个状态，棋子距其初始状态的 Manhattan 距离之和即为走到该状态的步数，这样就无需储存当前执棋状态．
6. 注意卡常．不想卡常的话，容易发现两个红子本质相同，那么可以钦定 $a \le c$，状态数减少一半．

## 代码

```cpp
#include <algorithm>
#include <cstdio>
#include <cstring>

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

const int N = 10;
const int M = 1e6;

struct pos {
	int x, y;
};
struct state {
	pos r1, r2, b;
};

const pos d[4] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};

int T, n, m;
char mp[N + 5][N + 5];

int alct;
state sta[M + 5];
int num[1 << 24];

struct edge {
	int v, nxt;
} g[M * 4 + 5];
int head[M + 5], ect, ind[M + 5];
void add(int u, int v) { g[++ect] = {v, head[u]}, head[u] = ect; }

inline bool operator==(const pos a, const pos b) { return a.x == b.x && a.y == b.y; }
inline pos operator+(const pos a, const pos b) { return {a.x + b.x, a.y + b.y}; }
inline int get(const pos a) { return (a.x << 4) | a.y; }
inline int get(const state &a) { return (get(a.r1) << 16) | (get(a.r2) << 8) | get(a.b); }
inline bool check(const pos a) { return (((n - a.x) | (a.x - 1) | (m - a.y) | (a.y - 1)) >= 0) && mp[a.x][a.y] != '#'; }
inline bool check(const state &a) { return check(a.r1) && check(a.r2) && check(a.b) && !(a.r1 == a.r2); }
inline int dist(const pos a, const pos b) { return std::abs(a.x - b.x) + std::abs(a.y - b.y); }
inline int dist(const state &a, const state &b) { return dist(a.r1, b.r1) + dist(a.r2, b.r2) + dist(a.b, b.b); }

int win[M + 5], dis[M + 5];
int q[M + 5], hd, tl;
inline void solve() {
	hd = 1, tl = 0;
	for (int i = 1; i <= alct; i++) {
		if (ind[i]) continue;
		win[i] = -1, q[++tl] = i;
	}
	while (tl >= hd) {
		int u = q[hd++];
		for (int e = head[u]; e; e = g[e].nxt) {
			int v = g[e].v;
			if (win[v]) continue;
			if (win[u] == -1) win[v] = 1, dis[v] = dis[u] + 1, q[++tl] = v;
			else if (!--ind[v]) win[v] = -1, dis[v] = dis[u] + 1, q[++tl] = v;
		}
	}
}

inline void clear() {
	memset(head, 0, sizeof(head)), ect = 0;
	memset(ind, 0, sizeof(ind));
	memset(win, 0, sizeof(win));
	memset(dis, 0, sizeof(dis));
	alct = 0;
}

int main() {
	rd(), T = rd();
	while (T--) {
		clear();
		n = rd(), m = rd();
		for (int i = 1; i <= n; i++) { scanf("%s", mp[i] + 1); }

		state st = {};
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= m; j++) {
				if (mp[i][j] == 'O') st.r1.x ? st.r2 = {i, j} : st.r1 = {i, j};
				if (mp[i][j] == 'X') st.b = {i, j};
			}
		}

		for (int a = 1; a <= n; a++) {
			for (int b = 1; b <= m; b++) {
				for (int c = 1; c <= n; c++) {
					for (int d = 1; d <= m; d++) {
						for (int e = 1; e <= n; e++) {
							for (int f = 1; f <= m; f++) {
								state cur = {{a, b}, {c, d}, {e, f}};
								if (!check(cur)) continue;
								alct++, sta[alct] = cur, num[get(cur)] = alct;
							}
						}
					}
				}
			}
		}

		for (int i = 1; i <= alct; i++) {
			auto [r1, r2, b] = sta[i];
			if (r1 == b || r2 == b || b.x == 1) continue;
			if (dist(sta[i], st) % 2) {
				for (int _ = 0; _ < 3; _++) {
					state ns = {r1, r2, b + d[_]};
					if (check(ns)) add(num[get(ns)], i), ind[i]++;
				}
			} else {
				for (int _ = 0; _ < 4; _++) {
					state ns = {r1 + d[_], r2, b};
					if (check(ns)) add(num[get(ns)], i), ind[i]++;
				}
				for (int _ = 0; _ < 4; _++) {
					state ns = {r1, r2 + d[_], b};
					if (check(ns)) add(num[get(ns)], i), ind[i]++;
				}
			}
		}

		solve();

		int id = num[get(st)];
		if (!win[id]) puts("Tie");
		else if (win[id] == 1) printf("Red %d\n", dis[id]);
		else if (win[id]) printf("Black %d\n", dis[id]);
	}
	return 0;
}
```
