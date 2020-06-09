#include <bits/stdc++.h>
#define ll long long int
#define N 10000
using namespace std;

void printBin(ll x) {
  if (x == 0) return;
  printBin(x / 2);
  cout << (x & 1) << " ";
}

void buildTree(ll x, ll init, unordered_map<ll, ll> &nodes,
               vector<set<pair<ll, ll>>> &e) {
  ll i = 1, n = x, y;
  nodes[x] = 2;

  cout << x << " -> ";
  printBin(x);
  cout << "\n";

  while (n > 0) {
    if (!(n & 1)) {
      y = (x >> i) | init;

      if (!nodes.count(y)) nodes[y] = 1;

      e[x].insert({y, i});

      if (nodes[y] == 1) buildTree(y, init, nodes, e);
    }
    i++;
    n /= 2;
  }

  e[x].insert({init, i});
}

void printEdges(ll x, unordered_map<ll, ll> &nodes,
                vector<set<pair<ll, ll>>> &e) {
  nodes[x] = 2;

  cout << x << ": ";
  for (auto it : e[x]) cout << it.first << ", ";
  cout << "\n";

  for (auto it : e[x]) {
    if (nodes[it.first] == 1) printEdges(it.first, nodes, e);
  }
}

void greedyCycles(ll x, unordered_map<ll, ll> &nodes,
                  vector<set<pair<ll, ll>>> &e, vector<vector<ll>> &greedy,
                  vector<ll> v, double &mal, ll sum) {
  nodes[x] = 2;
  v.push_back(x);

  ll mn = INT_MAX, y = x;
  for (auto it : e[x]) {
    if (it.second < mn) {
      mn = it.second;
      y = it.first;
    }
  }

  ll minn = INT_MAX, idx;
  for (ll i = 0; i < v.size(); i++)
    if (v[i] < minn) minn = v[i], idx = i;

  bool t = false, w;
  for (auto it : greedy) {
    w = true;

    for (ll i = 0; i < v.size(); i++)
      if (v[(idx + i) % v.size()] != it[i]) w = false;

    if (w) t = true;
  }

  if (!t && v[0] == y) {
    vector<ll> newCycle;
    for (ll i = 0; i < v.size(); i++)
      newCycle.push_back(v[(idx + i) % v.size()]);
    greedy.push_back(newCycle);

    if (double(sum + mn) / v.size() < mal) mal = double(sum + mn) / v.size();
  }

  if (nodes[y] == 1) greedyCycles(y, nodes, e, greedy, v, mal, sum + mn);

  v.pop_back();
}

void simpleCycles(ll x, unordered_map<ll, ll> &nodes,
                  vector<set<pair<ll, ll>>> &e, vector<vector<ll>> &simple,
                  vector<ll> v) {
  nodes[x] = 2;
  v.push_back(x);

  for (auto it : e[x]) {
    ll minn = INT_MAX, idx;
    for (ll i = 0; i < v.size(); i++)
      if (v[i] < minn) minn = v[i], idx = i;

    bool t = false, w;
    for (auto it : simple) {
      w = true;

      for (ll i = 0; i < v.size(); i++)
        if (v[(idx + i) % v.size()] != it[i]) w = false;

      if (w && v.size() == it.size()) t = true;
    }

    if (nodes[it.first] == 2 && !t && (v.size() == 1 || it.first != x)) {
      vector<ll> newCycle;
      for (ll i = 0; i < v.size(); i++)
        newCycle.push_back(v[(idx + i) % v.size()]);

      simple.push_back(newCycle);
    }

    if (nodes[it.first] == 1) simpleCycles(it.first, nodes, e, simple, v);
  }

  nodes[x] = 1;
  v.pop_back();
}

int main() {
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  ll r, c;
  cin >> r >> c;

  vector<vector<char>> a(r, vector<char>(c));
  for (ll i = 0; i < r; i++)
    for (ll j = 0; j < c; j++) cin >> a[i][j];

  ll initial = 0;
  set<ll> s;
  for (ll i = 0; i < r; i++) {
    for (ll j = 0; j < c; j++) {
      for (ll k = j + 1; k < c; k++) {
        if (a[i][j] == a[i][k] && a[i][j] == 'X') {
          initial |= (1 << (k - j - 1));
          s.insert(k - j);
        }
      }
    }
  }

  cout << "Forbidden Latencies: \n";
  ll mx = INT_MIN;
  for (auto it : s) {
    cout << it << " ";
    mx = max(mx, it);
  }

  cout << "\n\nPermissible Latencies: \n";
  for (ll i = 1; i < mx; i++)
    if (s.find(i) == s.end()) cout << i << " ";

  cout << "\n\nCollision Vector: \n";
  for (ll i = mx; i >= 1; i--) cout << (s.find(i) != s.end()) << " ";

  vector<set<pair<ll, ll>>> e(N);
  unordered_map<ll, ll> nodes;
  nodes[initial] = 1;

  cout << "\n\nNodes:\n";
  buildTree(initial, initial, nodes, e);
  for (auto it : nodes) nodes[it.first] = 1;

  cout << "\nEdges:\n";
  printEdges(initial, nodes, e);
  for (auto it : nodes) nodes[it.first] = 1;
  cout << "\n";

  vector<ll> v;
  double mal = INT_MAX;
  vector<vector<ll>> greedy;

  cout << "Greedy Cycles: \n";
  for (auto it : nodes) {
    greedyCycles(it.first, nodes, e, greedy, v, mal, 0);
    for (auto it2 : nodes) nodes[it2.first] = 1;
  }

  for (auto it : greedy) {
    for (auto x : it) cout << x << " -> ";
    cout << it[0] << "\n";
  }

  cout << "\nMAL = " << mal << "\n";

  vector<vector<ll>> simple;
  cout << "\nSimple Cycles: \n";
  for (auto it : nodes) {
    simpleCycles(it.first, nodes, e, simple, v);
    for (auto it2 : nodes) nodes[it2.first] = 1;
  }

  for (auto it : simple) {
    for (auto x : it) cout << x << " -> ";
    cout << it[0] << "\n";
  }
  return 0;
}