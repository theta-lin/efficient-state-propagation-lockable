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
#include "propagation_lockable.hpp"
#include "util.hpp"

void test(const std::string &name, const Weighter &weighter, int type, int vSize, int eSize, double lock, int qLim, int gLim, int opLim, int dist)
{
	std::cout << name << ": " << (type == 0 ? "uniform" : "BA") << " |V| = " << vSize << ", |E| = " << eSize << '\n';

	std::vector<double> x(qLim / dist);
	for (int i{0}; i < static_cast<int>(x.size()); ++i)
	{
		x[i] = (i + 1) * dist;
	}

	std::vector<double> y(qLim / dist);
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

				if (i >= vSize && (i - vSize + 1) % dist == 0) y[(i - vSize + 1) / dist - 1] += pl.countS();
			}

			std::cout << "\trun ops: " << o + 1 << '/' << opLim << '\n';
		}
	}
	for (double &i : y)
	{
		i /= gLim * opLim;
	}

	using namespace matplot;
	auto fig{figure(true)};
	auto ax{gca()};
    auto l{ax->plot(x, y, "-o")};
   	l->marker_face(true);
	ax->xlabel("q");
	ax->ylabel("num of v.s = true");
    fig->save(name);
}

int main()
{
	test("uniform_coverage_1000000_10000000_0.eps", WeighterNon{}, 0, 1000000, 10000000, 0, 100, 5, 5, 1);
	test("ba_coverage_1000000_10_0.eps", WeighterNon{}, 1, 1000000, 10, 0, 100, 5, 5, 1);
	return 0;

	test("uniform_coverage_1000000_10000000_0.1.eps", WeighterNon{}, 0, 1000000, 10000000, 0.1, 1000000, 5, 5, 1000);

	test("uniform_coverage_1000000_10000000_0.1_power_1.eps", WeighterPower{1}, 0, 1000000, 10000000, 0.1, 1000000, 5, 5, 1000);
	test("uniform_coverage_1000000_10000000_0.1_power_2.eps", WeighterPower{2}, 0, 1000000, 10000000, 0.1, 1000000, 5, 5, 1000);
	test("uniform_coverage_1000000_10000000_0.1_power_3.eps", WeighterPower{3}, 0, 1000000, 10000000, 0.1, 1000000, 5, 5, 1000);

	test("uniform_coverage_1000000_10000000_0.1_exp_2.eps", WeighterExp{2}, 0, 1000000, 10000000, 0.1, 1000000, 5, 5, 1000);
	test("uniform_coverage_1000000_10000000_0.1_exp_3.eps", WeighterExp{3}, 0, 1000000, 10000000, 0.1, 1000000, 5, 5, 1000);

	test("ba_coverage_1000000_10_0.1.eps", WeighterNon{}, 1, 1000000, 10, 0.1, 1000000, 5, 5, 1000);

	test("ba_coverage_1000000_10_0.1_power_1.eps", WeighterPower{1}, 1, 1000000, 10, 0.1, 1000000, 5, 5, 1000);
	test("ba_coverage_1000000_10_0.1_power_2.eps", WeighterPower{2}, 1, 1000000, 10, 0.1, 1000000, 5, 5, 1000);
	test("ba_coverage_1000000_10_0.1_power_3.eps", WeighterPower{3}, 1, 1000000, 10, 0.1, 1000000, 5, 5, 1000);

	test("ba_coverage_1000000_10_0.1_exp_2.eps", WeighterExp{2}, 1, 1000000, 10, 0.1, 1000000, 5, 5, 1000);
	test("ba_coverage_1000000_10_0.1_exp_3.eps", WeighterExp{3}, 1, 1000000, 10, 0.1, 1000000, 5, 5, 1000);

	return 0;
}
