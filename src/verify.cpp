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
#include "propagation_lockable.hpp"
#include "bruteforce.hpp"

bool verify(bool logOn)
{
	const int vSize{1000}, eSize{200000}, q{200000};
	auto adj{generateGraph(vSize, eSize)};

	Bruteforce bf{adj};
	PropagationLockable pl{adj};
	auto ops{generateOps(vSize, q, {1.0 / 3, 1.0 / 3, 1.0 / 3})};
	std::vector<std::string> log;
	for (int i{0}; i < q; ++i)
	{
		switch (ops[i][0])
		{
		case 0:
		{
			if (logOn) log.emplace_back("set(" + std::to_string(ops[i][1]) + ',' + std::to_string(ops[i][2]) + ")\n");
			bf.set(ops[i][1], ops[i][2]);
			pl.set(ops[i][1], ops[i][2]);
			break;
		}

		case 1:
		{
			if (logOn) log.emplace_back("setLock(" + std::to_string(ops[i][1]) + ',' + std::to_string(ops[i][2]) + ")\n");
			bf.setLock(ops[i][1], ops[i][2]);
			pl.setLock(ops[i][1], ops[i][2]);
			break;
		}

		case 2:
		{
			if (logOn) log.emplace_back("propagate()\n");
			bf.propagate();
			pl.propagate();
			break;
		}
		}

		for (int v{0}; v < vSize; ++v)
		{
			if (bf.get(v) != pl.get(v) || bf.getLocked(v) != pl.getLocked(v))
			{
				std::cout << "FAILED\n";
				std::cout << "graph:\n";
				for (int v{0}; v < static_cast<int>(adj.size()); ++v)
				{
					for (int vp : adj[v])
					{
						std::cout << v << " -> " << vp << '\n';
					}
				}
				std::cout << "log:\n";
				for (auto &str : log)
				{
					std::cout << str;
				}
				std::cout << "status bf,pl:\n";
				for (int vv{0}; vv < vSize; ++vv)
				{
					std::cout << vv << ":\ts: " << bf.get(vv) << ',' << pl.get(vv) << "\tlocked: " << bf.getLocked(vv) << ',' << pl.getLocked(vv) << '\n';
				}
				std::cin.get();
				return false;
			}
		}
	}

	std::cout << "passed\n";
	return true;
}

int main()
{
	for (int i{1}; i <= 5; ++i)
	{
		std::cout << "# " << i << '\n';
		verify(false);
	}

	return 0;
}
