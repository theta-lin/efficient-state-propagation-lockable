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

#include <cstdio>
#include <vector>

const int        maxN    (500000 + 10);
int              to      [maxN * 2];
int              pre     [maxN * 2];
int              last    [maxN];
int              father  [maxN];

bool             infected[maxN];
bool             locked  [maxN];

int              src     [maxN];
std::vector<int> dst     [maxN];

void addEdge(int x, int y)
{
	static int lastEdge(0);
	++lastEdge;
	to[lastEdge] = y;
	pre[lastEdge] = last[x];
	last[x] = lastEdge;
}

void dfs(int v)
{
	for (int e(last[v]); e; e = pre[e])
	{
		if (to[e] != father[v])
		{
			father[to[e]] = v;
			dfs(to[e]);
		}
	}
}

std::vector<int> queue;

void infect(int v)
{
	if (!locked[v] && !infected[v])
	{
		infected[v] = true;
		++src[father[v]];
		queue.push_back(v);
	}
}

void toggle(int v)
{
	locked[v] = !locked[v];
	if (locked[v])
	{
		if (infected[v])
		{
			--src[father[v]];
		}
	}
	else
	{
		if (infected[v])
		{
			++src[father[v]];
		}
		else
		{
			dst[father[v]].push_back(v);
		}
		queue.push_back(v);
	}
}

void transmit()
{
	std::vector<int> cur;
	std::swap(cur, queue);
	std::vector<int> toInfect;
	for (std::size_t i(0); i < cur.size(); ++i)
	{
		int v(cur[i]);
		if (!locked[v])
		{
			if (infected[v])
			{
				toInfect.push_back(father[v]);
				for (std::size_t j(0); j < dst[v].size(); ++j)
				{
					toInfect.push_back(dst[v][j]);
				}
				dst[v].clear();
			}
			else
			{
				if (src[v] || (!locked[father[v]] && infected[father[v]]))
				{
					toInfect.push_back(v);
				}
			}
		}
	}

	for (std::size_t i(0); i < toInfect.size(); ++i)
	{
		infect(toInfect[i]);
	}
}

int main()
{
	int n, m, q;
	scanf("%d %d %d", &n, &m, &q);
	for (int e(1); e <= n - 1; ++e)
	{
		int x, y;
		scanf("%d %d", &x, &y);
		addEdge(x, y);
		addEdge(y, x);
	}

	dfs(1);

	for (int i(1); i <= m; ++i)
	{
		int a;
		scanf("%d", &a);
		infect(a);
	}
	for (int v(1); v <= n; ++v)
	{
		if (!infected[v])
		{
			dst[father[v]].push_back(v);
		}
	}

	for (int qqq(1); qqq <= q; ++qqq)
	{
		int opt;
		scanf("%d", &opt);
		switch (opt)
		{
		case 1:
		case 2:
		{
			int wwj;
			scanf("%d", &wwj);
			toggle(wwj);
			break;
		}

		case 3:
		{
			int wwj;
			scanf("%d", &wwj);
			printf(infected[wwj] ? "Y\n" : "N\n");
			break;
		}

		case 4:
		{
			transmit();
			break;
		}
		}
	}

	return 0;
}
