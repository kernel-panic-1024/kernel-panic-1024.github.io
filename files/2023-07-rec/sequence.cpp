#include <cstdio>
#include <random>
#include <algorithm>
#include <vector>

inline int rd() {
	int x = 0, f = 1, c = getchar();
	while (((c - '0') | ('9' - c)) < 0)
		f = c != '-', c = getchar();
	while (((c - '0') | ('9' - c)) > 0)
		x = x * 10 + c - '0', c = getchar();
	return f ? x : -x;
}

#define int long long

std::mt19937 rng(19260817);

const int N = 1e5;

#define lc(p) (t[p].ch[0])
#define rc(p) (t[p].ch[1])
struct dat {
	int s[2]; // +, -
};
dat operator+(dat a, dat b) {
	return {a.s[0] + b.s[0], a.s[1] + b.s[1]};
}
dat operator+(dat a, int v) {
	if(v > 0) return {a.s[0] + v, a.s[1]};
	else return {a.s[0], a.s[1] + v};
}

struct node {
	int ch[2], siz, key;
	int v[2]; dat s[2]; // i - (i - 1), i - (i + 1)
	int r;
} t[N + 5]; int alct;
int newnode(int v0, int v1) {
	int p = ++alct;
	t[p].siz = 1, t[p].key = rng();
	t[p].v[0] = v0, t[p].s[0] = t[p].s[0] + v0;
	t[p].v[1] = v1, t[p].s[1] = t[p].s[1] + v1;
	return p;
}
void rev(int p) {
	if(!p) return;
	std::swap(lc(p), rc(p));
	std::swap(t[p].v[0], t[p].v[1]);
	std::swap(t[p].s[0], t[p].s[1]);
	t[p].r ^= 1;
}
// ensure single point
void _add(int p, int id, int d) {
	if(!p) return;
	t[p].v[id] += d;
	t[p].s[id] = {};
	t[p].s[id] = t[p].s[id] + t[p].v[id];
}
// ensure single point
void cov(int p, int id, int nv) {
	if(!p) return;
	t[p].v[id] = nv;
	t[p].s[id] = {};
	t[p].s[id] = t[p].s[id] + nv;
}
void pushdown(int p) {
	if(!t[p].r) return;
	rev(lc(p)), rev(rc(p));
	t[p].r = 0;
}
void pushup(int p) {
	t[p].siz = t[lc(p)].siz + t[rc(p)].siz + 1;
	t[p].s[0] = t[lc(p)].s[0] + t[rc(p)].s[0] + t[p].v[0];
	t[p].s[1] = t[lc(p)].s[1] + t[rc(p)].s[1] + t[p].v[1];
}
void splt(int p, int &x, int &y, int k) {
	if(!p) return x = y = 0, void();
	pushdown(p);
	if(t[lc(p)].siz < k) x = p, splt(rc(p), rc(x), y, k - t[lc(p)].siz - 1);
	else y = p, splt(lc(p), x, lc(y), k);
	pushup(p);
}
int mrg(int p, int q) {
	if(!p || !q) return p + q;
	if(t[p].key < t[q].key) {
		pushdown(p);
		rc(p) = mrg(rc(p), q);
		pushup(p);
		return p;
	} else {
		pushdown(q);
		lc(q) = mrg(p, lc(q));
		pushup(q);
		return q;
	}
}
#undef lc
#undef rc

int n, m, a[N + 5];
int rt;

int getv(int x) {
	int a, b;
	splt(rt, a, b, x);
	int ans = t[a].s[0].s[0] + t[a].s[0].s[1];
	rt = mrg(a, b);
	return ans;
}

void add(int p, int id, int d) {
	int x, y, z;
	splt(rt, x, z, p);
	splt(x, x, y, p - 1);
	_add(y, id, d);
	rt = mrg(mrg(x, y), z);
}

int opt001(int l, int r) {
	int a = getv(l) - 0, b = 0 - getv(r);
	int ans = 0;
	if(a > 0) ans += a;
	if(b > 0) ans += b;
	if(l < r) {
		int x, y, z;
		splt(rt, x, z, r);
		splt(x, x, y, l);
		ans += t[y].s[0].s[0];
		rt = mrg(mrg(x, y), z);
	}
	return ans;
}

void opt010(int l, int r, int delta) {
	add(l, 0, delta), add(r + 1, 0, -delta);
	add(r, 1, delta), add(l - 1, 1, -delta);
}

void opt011(int l, int r) {
	int v1 = getv(l - 1), v2 = getv(l), v3 = getv(r), v4 = getv(r + 1);
	if(l == r) return;
	else if(l + 1 == r) {
		/*
			a: [1, l - 2]
			b: l - 1
			c: l
			d: r
			e: r + 1
			f: [r + 2, n]
		*/
		int a, b, c, d, e, f;
		splt(rt, a, f, r + 1);
		splt(a, a, e, r);
		splt(a, a, d, r - 1);
		splt(a, a, c, l - 1);
		splt(a, a, b, l - 2);

		// process l - 1
		cov(b, 1, v1 - v3);

		// process l (new r)
		std::swap(t[c].v[0], t[c].v[1]);
		std::swap(t[c].s[0], t[c].s[1]);
		cov(c, 1, v2 - v4);

		// process r (new l)
		std::swap(t[d].v[0], t[d].v[1]);
		std::swap(t[d].s[0], t[d].s[1]);
		cov(d, 0, v3 - v1);

		// process r + 1
		cov(e, 0, v4 - v2);

		rt = mrg(mrg(mrg(mrg(mrg(a, b), d), c), e), f);
	} else {
		/*
			a: [1, l - 2]
			b: l - 1
			c: l
			d: [l + 1, r - 1]
			e: r
			f: r + 1
			g: [r + 2, n]
		*/
		int a, b, c, d, e, f, g;
		splt(rt, a, g, r + 1);
		splt(a, a, f, r);
		splt(a, a, e, r - 1);
		splt(a, a, d, l);
		splt(a, a, c, l - 1);
		splt(a, a, b, l - 2);

		// process l - 1
		cov(b, 1, v1 - v3);

		// process l (new r)
		std::swap(t[c].v[0], t[c].v[1]);
		std::swap(t[c].s[0], t[c].s[1]);
		cov(c, 1, v2 - v4);

		// process [l + 1, r - 1]
		rev(d);

		// process r (new l)
		std::swap(t[e].v[0], t[e].v[1]);
		std::swap(t[e].s[0], t[e].s[1]);
		cov(e, 0, v3 - v1);

		// process r + 1
		cov(f, 0, v4 - v2);

		rt = mrg(mrg(mrg(mrg(mrg(mrg(a, b), e), d), c), f), g);
	}
}

struct option {
	int opt, l, r, x;
} op[N + 5];

int fa[N + 5];
std::vector<int> g[N + 5];
int ans[N + 5];
void dfs(int u) {
	auto [opt, l, r, x] = op[u];

	if(opt == 1) ans[u] = opt001(l, r);

	if(opt == 10) opt010(l, r, x);
	if(opt == 11) opt011(l, r);

	for(int v : g[u]) dfs(v);

	if(opt == 10) opt010(l, r, -x);
	if(opt == 11) opt011(l, r);
}

signed main() {
	n = rd(), m = rd();
	for(int i = 1; i <= n; i++) a[i] = rd();

	for(int i = 1; i <= n; i++) {
		rt = mrg(rt, newnode(a[i] - a[i - 1], a[i] - a[i + 1]));
	}

	for(int i = 1; i <= m; i++) {
		fa[i] = i - 1;
		int opt = rd();
		if(opt == 1) {
			int l = rd(), r = rd();
			op[i] = {opt, l, r, 0};
		} else if(opt == 10) {
			int l = rd(), r = rd(), x = rd();
			op[i] = {opt, l, r, x};
		} else if(opt == 11) {
			int l = rd(), r = rd();
			op[i] = {opt, l, r, 0};
		} else {
			int k = rd(); fa[i] = i - k - 1;
		}
	}

	for(int i = 1; i <= m; i++) {
		g[fa[i]].push_back(i);
	}
	dfs(0);
	for(int i = 1; i <= m; i++) {
		if(op[i].opt == 1) printf("%lld\n", ans[i]);
	}
	return 0;
}
