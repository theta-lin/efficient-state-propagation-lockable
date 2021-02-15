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

double run(const std::vector<std::vector<int>> &adj, const std::vector<std::array<int, 3>> &ops, int q, double critRatio)
{
	clearCache();
	Timer timer;
	PropagationLockable pl{adj, critRatio};
	for (int i{0}; i < q; ++i)
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
	}

	return timer.elapsed();
}

void benchCrit(const std::string &name, int type, double rMax, double aMax, int rSample, int aSample, int gLim, int opLim)
{
	std::cout << name << ": " << (type == 0 ? "power" : "exp") << " rMax = " << rMax << ", aMax = " << aMax << '\n';

	const int vSize{1000000};
	const int eSize{10};
	const int q{1000000};
	const double lock{0.1};

	std::vector<std::vector<double>> X(aSample);
	for (auto &x : X)
	{
		x.resize(rSample);
	}

	std::vector<std::vector<double>> Y(aSample);
	for (auto &y : Y)
	{
		y.resize(rSample);
	}

	std::vector<std::vector<double>> Z(aSample);
	for (auto &z : Z)
	{
		z.resize(rSample);
	}

	for (int g{0}; g < gLim; ++g)
	{
		auto adj{generateBaGraph(vSize, eSize)};
		std::cout << "graph generated: " << g + 1 << '/' << gLim << '\n';

		for (int aId{0}; aId < aSample; ++aId)
		{
			double a{(aId + 1) * aMax / aSample};
			std::vector<double> weight;
			if (type == 0)
			{
				weight = WeighterPower{a}(adj);
			}
			else
			{
				weight = WeighterExp{a}(adj);
			}

			for (int o{0}; o < opLim; ++o)
			{
				auto ops{generateOps(q, lock, {1.0 / 3, 1.0 / 3, 1.0 / 3}, weight)};

				for (int rId{0}; rId < rSample; ++rId)
				{
					double r{(rId + 1) * rMax / rSample};
					X[aId][rId] = r;
					Y[aId][rId] = a;
					Z[aId][rId] += run(adj, ops, q, r);
					std::cout << "\t run ops: " << o + 1 << ' ' << a << ' ' << r << '\n';
				}
			}
		}
	}

	for (auto &z : Z)
	{
		for (double &zz : z)
		{
			zz /= gLim * opLim;
		}
	}

	using namespace matplot;
	auto fig{figure(true)};
	auto ax{gca()};
	auto l{plot3(X, Y, Z, "-o")};
	for (auto &ll : l)
	{
		ll->marker_face(true);
	}
	ax->xlabel("r");
	ax->ylabel("a");
	ax->zlabel("t/s");
	fig->save(name);
}

int main()
{
	benchCrit("ba_crit_power_1.eps", 0, 100, 5, 20, 5, 7, 7);

	return 0;
}
