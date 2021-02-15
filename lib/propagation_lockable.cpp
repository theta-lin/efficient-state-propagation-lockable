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

#include "propagation_lockable.hpp"

PropagationLockable::PropagationLockable(const std::vector<std::vector<int>> &adj, double critCoeff) : adj{adj}, degree(adj.size()), large(adj.size()), s(adj.size()), locked(adj.size()), sources(adj.size()), targets(adj.size())
{
	int edgeCnt{0};
	for (auto &v : adj)
	{
		for (int vp : v)
		{
			++edgeCnt;
			++degree[vp];
		}
	}
	crit = static_cast<int>(critCoeff * std::sqrt(edgeCnt / 2));

	for (int v{0}; v < static_cast<int>(adj.size()); ++v)
	{
		for (int vp : adj[v])
		{
			if (degree[vp] > crit)
			{
				large[v].push_back(vp);
				targets[vp].push_back(v);
			}
		}
	}
}

bool PropagationLockable::get(int v)
{
	return s[v];
}

bool PropagationLockable::getLocked(int v)
{
	return locked[v];
}

int PropagationLockable::countS()
{
	int cnt{0};
	for (bool sss : s)
	{
		cnt += sss;
	}
	return cnt;
}

std::vector<std::pair<size_t, size_t>> PropagationLockable::getEdges()
{
	std::vector<std::pair<size_t, size_t>> result;
	for (int v{0}; v < static_cast<int>(adj.size()); ++v)
	{
		for (int vp : adj[v])
		{
			result.push_back({v, vp});
		}
	}
	return result;
}

std::vector<float> PropagationLockable::getSizes()
{
	std::vector<float> sizes;
	for (int d : degree)
	{
		sizes.push_back(d > crit ? 3.0 : 1.5);
	}
	return sizes;
}

std::vector<double> PropagationLockable::getColors()
{
	std::vector<double> colors;
	for (bool sss : s)
	{
		colors.push_back(sss ? 1.0 : 0.0);
	}
	return colors;
}

std::vector<std::string> PropagationLockable::getLabels()
{
	std::vector<std::string> labels(adj.size());
	for (int v{0}; v < static_cast<int>(adj.size()); ++v)
	{
		labels[v] = std::to_string(v);
		if (locked[v])
		{
			labels[v] += ", locked";
		}
		if (sources[v])
		{
			labels[v] += ", sources = " + std::to_string(sources[v]);
		}
		if (!targets[v].empty())
		{
			auto tmp{targets[v]};
			std::sort(tmp.begin(), tmp.end());
			tmp.erase(std::unique(tmp.begin(), tmp.end()), tmp.end());
			int cnt{0};
			for (int v : tmp)
			{
				if (!s[v] && !locked[v])
				{
					++cnt;
				}
			}
			if (cnt)
			{
				labels[v] += ", targets = " + std::to_string(cnt);
			}
		}
	}
	return labels;
}

void PropagationLockable::set(int v, bool sp)
{
	if (s[v] == sp || locked[v]) return;
	s[v] = sp;

	if (s[v])
	{
		for (int vp : large[v])
		{
			++sources[vp];
		}
	}
	else
	{
		for (int vp : large[v])
		{
			--sources[vp];
			targets[vp].push_back(v);
		}
	}
	queue.push_back(v);
}

void PropagationLockable::setLock(int v, bool lockedp)
{
	if (locked[v] == lockedp) return;
	locked[v] = lockedp;

	if (locked[v])
	{
		if (s[v])
		{
			for (int vp : large[v])
			{
				--sources[vp];
			}
		}
	}
	else
	{
		if (s[v])
		{
			for (int vp : large[v])
			{
				++sources[vp];
			}
		}
		else
		{
			for (int vp : large[v])
			{
				targets[vp].push_back(v);
			}
		}
		queue.push_back(v);
	}
}

void PropagationLockable::propagate()
{
	std::vector<int> temp, pending;
	std::swap(queue, temp);

	for (int v : temp)
	{
		if (locked[v]) continue;

		if (s[v])
		{
			if (degree[v] > crit)
			{
				for (int vp : targets[v])
				{
					pending.push_back(vp);
				}
				targets[v].clear();
			}
			else
			{
				for (int vp : adj[v])
				{
					pending.push_back(vp);
				}
			}
		}
		else
		{
			if (degree[v] > crit)
			{
				if (sources[v] > 0)
				{
					pending.push_back(v);
				}
			}
			else
			{
				for (int vp : adj[v])
				{
					if (s[vp] && !locked[vp])
					{
						pending.push_back(v);
						break;
					}
				}
			}
		}
	}

	for (int v : pending)
	{
		set(v, true);
	}
}
