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
#include <cmath>
#include <vector>

const int        maxN    (500000 + 10);
std::vector<int> edge    [maxN];
std::vector<int> large   [maxN];
int              degree  [maxN];

bool             infected[maxN];
bool             locked  [maxN];

int              src     [maxN];
std::vector<int> dst     [maxN];

int crit;

std::vector<int> queue;

void infect(int v)
{
	if (!locked[v] && !infected[v])
	{
		infected[v] = true;
		for (std::size_t e(0); e < large[v].size(); ++e)
		{
			int to(large[v][e]);
			++src[to];
		}
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
			for (std::size_t e(0); e < large[v].size(); ++e)
			{
				int to(large[v][e]);
				--src[to];
			}
		}
	}
	else
	{
		if (infected[v])
		{
			for (std::size_t e(0); e < large[v].size(); ++e)
			{
				int to(large[v][e]);
				++src[to];
			}
		}
		else
		{
			for (std::size_t e(0); e < large[v].size(); ++e)
			{
				int to(large[v][e]);
				dst[to].push_back(v);
			}
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
				if (degree[v] > crit)
				{
					for (std::size_t e(0); e < dst[v].size(); ++e)
					{
						toInfect.push_back(dst[v][e]);
					}
					dst[v].clear();
				}
				else
				{
					for (std::size_t e(0); e < edge[v].size(); ++e)
					{
						toInfect.push_back(edge[v][e]);
					}
				}
			}
			else
			{
				if (degree[v] > crit)
				{
					if (src[v])
					{
						toInfect.push_back(v);
					}
				}
				else
				{
					for (std::size_t e(0); e < edge[v].size(); ++e)
					{
						int to(edge[v][e]);
						if (!locked[to] && infected[to])
						{
							toInfect.push_back(v);
						}
					}
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
		edge[x].push_back(y);
		edge[y].push_back(x);
		++degree[x];
		++degree[y];
	}

	crit = std::sqrt(2 * (n - 1));
	for (int v(1); v <= n; ++v)
	{
		for (std::size_t e(0); e < edge[v].size(); ++e)
		{
			int to(edge[v][e]);
			if (degree[to] > crit)
			{
				large[v].push_back(to);
			}
		}
	}

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
			for (std::size_t e(0); e < large[v].size(); ++e)
			{
				int to(edge[v][e]);
				dst[to].push_back(v);
			}
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
