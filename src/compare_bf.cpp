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

#include <matplot/matplot.h>
#include "util.hpp"
#include "propagation_lockable.hpp"
#include "bruteforce.hpp"

void test(const std::string &name, const Weighter &weighter, int type, int vSize, int eSize, double lock, int qLim, int gLim, int opLim, int dist)
{
	std::cout << name << ": " << (type == 0 ? "uniform" : "BA") << " |V| = " << vSize << ", |E| = " << eSize << '\n';

	std::vector<double> x(qLim / dist);
	for (int i{0}; i < static_cast<int>(x.size()); ++i)
	{
		x[i] = (i + 1) * dist;
	}

	std::vector<double> yPl(qLim / dist);
	std::vector<double> yBf(qLim / dist);

	for (int g{0}; g < gLim; ++g)
	{
		std::vector<std::vector<int>> adj;
		if (type == 0)
		{
			adj = generateGraph(vSize, eSize);
		}
		else
		{
			adj = generateBaGraph(vSize, eSize);
		}

		auto weight{weighter(adj)};

		std::cout << "graph generated: " << g + 1 << '/' << gLim << '\n';

		for (int o{0}; o < opLim; ++o)
		{
			auto ops{generateOps(qLim, lock, {1.0 / 3, 1.0 / 3, 1.0 / 3}, weight)};

			Timer timer;

			clearCache();
			timer.reset();
			PropagationLockable pl{adj};
			for (int i{0}; i < vSize + qLim; ++i)
			{
				switch (ops[i][0])
				{
				case 0:
				{
					pl.set(ops[i][1], ops[i][2]);
					break;
				}

				case 1:
				{
					pl.setLock(ops[i][1], ops[i][2]);
					break;
				}

				case 2:
				{
					pl.propagate();
					break;
				}
				}

				if (i >= vSize && (i - vSize + 1) % dist == 0) yPl[(i - vSize + 1) / dist - 1] += timer.elapsed();
			}

			clearCache();
			timer.reset();
			Bruteforce bf{adj};
			for (int i{0}; i < vSize + qLim; ++i)
			{
				switch (ops[i][0])
				{
				case 0:
				{
					bf.set(ops[i][1], ops[i][2]);
					break;
				}

				case 1:
				{
					bf.setLock(ops[i][1], ops[i][2]);
					break;
				}

				case 2:
				{
					bf.propagate();
					break;
				}
				}

				if (i >= vSize && (i - vSize + 1) % dist == 0) yBf[(i - vSize + 1) / dist - 1] += timer.elapsed();
			}

			std::cout << "\trun ops: " << o + 1 << '/' << opLim << '\n';
		}
	}

	for (double &i : yPl)
	{
		i /= gLim * opLim;
	}

	for (double &i : yBf)
	{
		i /= gLim * opLim;
	}

	using namespace matplot;
	auto fig{figure(true)};
	auto ax{gca()};
    auto l{ax->plot(x, yPl, "-o")};
   	l->marker_face(true);
    ax->hold(on);
    l = ax->plot(x, yBf, "-o");
   	l->marker_face(true);
	ax->xlabel("q");
	ax->ylabel("t/s");
	auto lgd{legend("novel", "naive")};
    fig->save(name);
}

int main()
{
	test("uniform_q_1000000_10000000_0.1.eps", WeighterNon{}, 0, 1000000, 10000000, 0.1, 1000000, 5, 5, 1000);

	test("uniform_q_1000000_10000000_0.1_power_1.eps", WeighterPower{1}, 0, 1000000, 10000000, 0.1, 1000000, 5, 5, 1000);
	test("uniform_q_1000000_10000000_0.1_power_2.eps", WeighterPower{2}, 0, 1000000, 10000000, 0.1, 1000000, 5, 5, 1000);
	test("uniform_q_1000000_10000000_0.1_power_3.eps", WeighterPower{3}, 0, 1000000, 10000000, 0.1, 1000000, 5, 5, 1000);

	test("uniform_q_1000000_10000000_0.1_exp_2.eps", WeighterExp{2}, 0, 1000000, 10000000, 0.1, 1000000, 5, 5, 1000);
	test("uniform_q_1000000_10000000_0.1_exp_3.eps", WeighterExp{3}, 0, 1000000, 10000000, 0.1, 1000000, 5, 5, 1000);

	test("ba_q_1000000_10_0.1.eps", WeighterNon{}, 1, 1000000, 10, 0.1, 1000000, 5, 5, 1000);

	test("ba_q_1000000_10_0.1_power_1.eps", WeighterPower{1}, 1, 1000000, 10, 0.1, 1000000, 5, 5, 1000);
	test("ba_q_1000000_10_0.1_power_2.eps", WeighterPower{2}, 1, 1000000, 10, 0.1, 1000000, 5, 5, 1000);
	test("ba_q_1000000_10_0.1_power_3.eps", WeighterPower{3}, 1, 1000000, 10, 0.1, 1000000, 5, 5, 1000);

	test("ba_q_1000000_10_0.1_exp_2.eps", WeighterExp{2}, 1, 1000000, 10, 0.1, 1000000, 5, 5, 1000);
	test("ba_q_1000000_10_0.1_exp_3.eps", WeighterExp{3}, 1, 1000000, 10, 0.1, 1000000, 5, 5, 1000);

	return 0;
}
