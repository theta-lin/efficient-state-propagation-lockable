/*
 *This program is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "util.hpp"

std::mt19937& getRng()
{
	static std::random_device rd;
	static std::mt19937 rng{rd()};
	return rng;
}

int getRandom(int l, int r)
{
	std::uniform_int_distribution distr{l, r};
	return distr(getRng());
}

long long RandUnique::claim(long long node, long long tl, long long tr, long long pos)
{
	--nodes[node].cnt;
	if (tl == tr)
	{
		return tl;
	}
	else
	{
		long long mid{(tl + tr) / 2};
		if (!nodes[node].lch)
		{
			nodes[node].lch = nodes.size();
			nodes.emplace_back(Node{mid - tl + 1, 0, 0});
		}
		if (!nodes[node].rch)
		{
			nodes[node].rch = nodes.size();
			nodes.emplace_back(Node{tr - mid, 0, 0});
		}

		if (pos <= nodes[nodes[node].lch].cnt)
		{
			return claim(nodes[node].lch, tl, mid, pos);
		}
		else
		{
			return claim(nodes[node].rch, mid + 1, tr, pos - nodes[nodes[node].lch].cnt);
		}
	}
}

RandUnique::RandUnique(long long size) : size{size}, curSize{size}
{
}

long long RandUnique::claim()
{
	assert(curSize > 0);
	if (nodes.empty()) nodes.emplace_back(Node{size, 0, 0});
	long long result{claim(0, 1, size, getRandom(1, curSize))};
	--curSize;
	return result;
}

std::vector<std::vector<int>> generateGraph(int vSize, int eSize)
{
	assert(vSize - 1 <= eSize && eSize <= 1ll * vSize * (vSize - 1) / 2);

	std::vector<std::vector<int>> adj(vSize);

	/*
	 *if (std::log2(eSize) * eSize < 1.0 * vSize * (vSize - 1) / 2)
	 *{
	 *    RandUnique ru{1ll * vSize * (vSize - 1) / 2};
	 *    for (int i{0}; i < eSize; ++i)
	 *    {
	 *        long long id{ru.claim()};
	 *        int a{static_cast<int>((2.0 * vSize - std::sqrt(4.0 * vSize * vSize - 4.0 * vSize - 8.0 * id + 9.0) - 1.0)/ 2.0)};
	 *        int b{static_cast<int>(a + id - 1ll * a * (2 * vSize - a - 1) / 2)};
	 *        //std::cerr << "id:" << id << " a:" << a << " b:" << b << '\n';
	 *        adj[a].push_back(b);
	 *        adj[b].push_back(a);
	 *    }
	 *}
	 *else
	 */
	{
		std::set<std::pair<int, int>> visit;
		for (int i{0}; i < eSize; ++i)
		{
			int a, b;
			do
			{
				a = getRandom(0, vSize - 1);
				b = getRandom(0, vSize - 1);
				if (a > b) std::swap(a, b);
			}
			while (a == b || visit.find({a, b}) != visit.end());
			visit.insert({a, b});
			adj[a].push_back(b);
			adj[b].push_back(a);
		}
	}

	/*
	 *for (int v{0}; v < vSize - 1; ++v)
	 *{
	 *    int to{getRandom(v + 1, vSize - 1)};
	 *    visit.insert({v, to});
	 *    adj[v].push_back(to);
	 *    adj[to].push_back(v);
	 *}
	 *for (int i{0}; i < eSize - vSize + 1; ++i)
	 *{
	 *    int a, b;
	 *    do
	 *    {
	 *        a = getRandom(0, vSize - 1);
	 *        b = getRandom(0, vSize - 1);
	 *        if (a > b) std::swap(a, b);
	 *    }
	 *    while (a == b || visit.find({a, b}) != visit.end());
	 *    visit.insert({a, b});
	 *    adj[a].push_back(b);
	 *    adj[b].push_back(a);
	 *}
	 */
	return adj;
}

std::vector<std::vector<int>> generateBaGraph(int vSize, int eOut)
{
	assert(vSize > eOut);

	std::vector<std::vector<int>> adj(vSize);
	std::vector<int> vertRep;
	for (int v{0}; v < eOut; ++v)
	{
		vertRep.push_back(v);
	}
	for (int v{eOut}; v < vSize; ++v)
	{
		std::set<int> visit;
		for (int i{0}; i < eOut; ++i)
		{
			int to;
			do
			{
				to = vertRep[getRandom(0, int(vertRep.size()) - 1)];
			}
			while (v == to || visit.find(to) != visit.end());
			visit.insert(to);
			adj[v].push_back(to);
			adj[to].push_back(v);
			vertRep.push_back(v);
			vertRep.push_back(to);
		}
	}
	return adj;
}

std::vector<int> getDegree(const std::vector<std::vector<int>> &adj)
{
	std::vector<int> degree(adj.size());
	for (auto &v : adj)
	{
		for (int vp : v)
		{
			++degree[vp];
		}
	}
	return degree;
}

std::vector<double> getBetweenness(const std::vector<std::vector<int>> &adj)
{
	std::vector<int> sorted(adj.size());
	std::iota(sorted.begin(), sorted.end(), 0);
	std::shuffle(sorted.begin(), sorted.end(), getRng());

	const int maxSrc(std::min(100, static_cast<int>(adj.size())));

	std::vector<double> cb(adj.size());
	for (int i{0}; i < maxSrc; ++i)
	{
		int src{sorted[i]};

		std::stack<int> stack;
		std::vector<std::vector<int>> pred(adj.size());
		std::vector<double> cnt(adj.size());
		cnt[src] = 1;
		std::vector<int> dist(adj.size());
		std::fill(dist.begin(), dist.end(), -1);
		dist[src] = 0;

		std::queue<int> queue;
		queue.push(src);
		while (!queue.empty())
		{
			int v{queue.front()};
			queue.pop();
			stack.push(v);
			for (int vp: adj[v])
			{
				if (dist[vp] == -1)
				{
					dist[vp] = dist[v] + 1;
					queue.push(vp);
				}

				if (dist[vp] == dist[v] + 1)
				{
					cnt[vp] += cnt[v];
					pred[vp].push_back(v);
				}
			}
		}

		std::vector<double> sum(adj.size());
		while (!stack.empty())
		{
			int v{stack.top()};
			stack.pop();
			for (int p : pred[v])
			{
				sum[p] += cnt[p] / cnt[v] * (1 + sum[v]);
			}
			if (v != src)
			{
				cb[v] += sum[v];
			}
		}
	}
	return cb;
}

std::vector<double> getPageRank(const std::vector<std::vector<int>> &adj)
{
	std::vector<double> pr(adj.size(), 1.0 / adj.size());
	const int maxIt{100};
	const double d{0.85};
	for (int it{0}; it < maxIt; ++it)
	{
		std::vector<double> cur(adj.size(), (1.0 - d) / adj.size());
		for (int v{0}; v < static_cast<int>(adj.size()); ++v)
		{
			for (int vp : adj[v])
			{
				cur[v] += d * pr[vp] / adj[vp].size();
			}
		}
		std::swap(pr, cur);
	}
	return pr;
}

std::vector<double> getKatz(const std::vector<std::vector<int>> &adj)
{
	const int maxDepth{5};
	const double alpha{1e-6};

	std::vector<double> katz(adj.size());
	for (int src{0}; src < static_cast<int>(adj.size()); ++src)
	{
		std::vector<bool> visit(adj.size());
		visit[src] = true;
		std::vector<int> cur{src};
		double curAlpha{alpha};

		for (int depth{0}; depth < maxDepth && !cur.empty(); ++depth)
		{
			std::vector<int> nxt;
			for (int v : cur)
			{
				for (int vp : adj[v])
				{
					katz[vp] += curAlpha;
					if (!visit[vp])
					{
						nxt.push_back(vp);
						visit[vp] = true;
					}
				}
			}
			std::swap(cur, nxt);
			curAlpha *= alpha;
		}
	}
	return katz;
}

std::vector<double> getLobby(std::vector<std::vector<int>> adj)
{
	std::vector<double> lobby(adj.size());
	std::vector<int> degree(adj.size());
	for (auto &v : adj)
	{
		for (int vp : v)
		{
			++degree[vp];
		}
	}

	for (int v{0}; v < static_cast<int>(adj.size()); ++v)
	{
		std::sort(adj[v].begin(), adj[v].end(), [&degree](int a, int b){ return degree[a] > degree[b]; });
		for (int i{0}; i < static_cast<int>(adj[v].size()); ++i)
		{
			if (i + 1 <= degree[adj[v][i]])
			{
				lobby[v] = i + 1;
			}
			else
			{
				break;
			}
		}
	}

	return lobby;
}

std::vector<double> WeighterNon::operator()(const std::vector<std::vector<int>> &adj) const
{
	return std::vector<double>(adj.size(), 1.0);
}

WeighterPower::WeighterPower(double power) : power{power}
{
}

std::vector<double> WeighterPower::operator()(const std::vector<std::vector<int>> &adj) const
{
	auto degree{getDegree(adj)};
	std::vector<double> weight;
	for (int d : degree)
	{
		weight.push_back(std::pow(d, power));
	}
	return weight;
}

WeighterExp::WeighterExp(double base) : base{base}
{
}

std::vector<double> WeighterExp::operator()(const std::vector<std::vector<int>> &adj) const
{
	auto degree{getDegree(adj)};
	std::vector<double> weight;
	for (int d : degree)
	{
		weight.push_back(std::pow(base, d));
	}
	return weight;
}


std::vector<double> WeighterCb::operator()(const std::vector<std::vector<int>> &adj) const
{
	return getBetweenness(adj);
}

std::vector<double> WeighterPr::operator()(const std::vector<std::vector<int>> &adj) const
{
	return getPageRank(adj);
}

std::vector<double> WeighterKatz::operator()(const std::vector<std::vector<int>> &adj) const
{
	return getKatz(adj);
}

std::vector<double> WeighterL::operator()(const std::vector<std::vector<int>> &adj) const
{
	return getLobby(adj);
}

std::vector<std::array<int, 3>> generateOps(int vSize, int cnt, const std::array<double, 3> &weights)
{
	std::discrete_distribution<int> distr{weights.begin(), weights.end()};
	std::vector<std::array<int, 3>> ops(cnt);
	for (int i{0}; i < cnt; ++i)
	{
		ops[i][0] = distr(getRng());
		if (ops[i][0] == 0 || ops[i][0] == 1)
		{
			ops[i][1] = getRandom(0, vSize - 1);
			ops[i][2] = getRandom(0, 1);
		}
	}
	return ops;
}

std::vector<std::array<int, 3>> generateOps(int cnt, const std::array<double, 3> &queryWeights, const std::vector<double> &vWeights)
{
	std::discrete_distribution<int> opDistr{queryWeights.begin(), queryWeights.end()};
	std::discrete_distribution<int> vDistr{vWeights.begin(), vWeights.end()};
	std::vector<std::array<int, 3>> ops(cnt);
	for (int i{0}; i < cnt; ++i)
	{
		ops[i][0] = opDistr(getRng());
		if (ops[i][0] == 0 || ops[i][0] == 1)
		{
			ops[i][1] = vDistr(getRng());
			ops[i][2] = getRandom(0, 1);
		}
	}
	return ops;
}

std::vector<std::array<int, 3>> generateOps(int cnt, double lock, const std::array<double, 3> &queryWeights, const std::vector<double> &vWeights)
{
	std::discrete_distribution<int> sDistr{1.0 - lock, lock};
	std::discrete_distribution<int> opDistr{queryWeights.begin(), queryWeights.end()};
	std::discrete_distribution<int> vDistr{vWeights.begin(), vWeights.end()};
	std::vector<std::array<int, 3>> ops(vWeights.size() + cnt);
	for (int i{0}; i < static_cast<int>(vWeights.size()); ++i)
	{
		ops[i][0] = 1;
		ops[i][1] = i;
		ops[i][2] = sDistr(getRng());
	}
	for (int i{0}; i < cnt; ++i)
	{
		ops[vWeights.size() + i][0] = opDistr(getRng());
		if (ops[vWeights.size() + i][0] == 0 || ops[vWeights.size() + i][0] == 1)
		{
			ops[vWeights.size() + i][1] = vDistr(getRng());
			ops[vWeights.size() + i][2] = getRandom(0, 1);
		}
	}
	return ops;
}

void clearCache()
{
	const int size{1 << 24};
	int *mem{new int[size]};
	std::memset(mem, 0, size * sizeof(int));
	delete[] mem;
}
