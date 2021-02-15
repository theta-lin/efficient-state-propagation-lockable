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

#include "bruteforce.hpp"

Bruteforce::Bruteforce(const std::vector<std::vector<int>> &adj) : adj{adj}, s(adj.size()), locked(adj.size())
{
}

bool Bruteforce::get(int v)
{
	return s[v];
}

bool Bruteforce::getLocked(int v)
{
	return locked[v];
}

void Bruteforce::set(int v, bool sp)
{
	if (s[v] == sp || locked[v]) return;
	s[v] = sp;
	queue.push_back(v);
}

void Bruteforce::setLock(int v, bool lockedp)
{
	if (locked[v] == lockedp) return;
	locked[v] = lockedp;
	queue.push_back(v);
}

void Bruteforce::propagate()
{
	std::vector<int> temp, pending;
	std::swap(queue, temp);
	for (int v : temp)
	{
		if (locked[v]) continue;

		if (s[v])
		{
			for (int vp : adj[v])
			{
				if (!s[vp] && !locked[vp])
				{
					pending.push_back(vp);
				}
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

	for (int v : pending)
	{
		set(v, true);
	}
}
