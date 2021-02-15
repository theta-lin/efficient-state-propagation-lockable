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

void plot(int id, PropagationLockable &pl)
{
	using namespace matplot;
	auto fig{figure(true)};
	auto ax{gca()};
	auto g{ax->graph(pl.getEdges())};

	g->layout_algorithm(network::layout::kawai);
	//g->layout_iterations(100000000);
	g->marker_sizes(pl.getSizes());
	g->marker_colors(pl.getColors());
	g->node_labels(pl.getLabels());
	ax->colormap(palette::paired());
	fig->save(std::to_string(id) + ".eps");
}

void small()
{
	//const int vSize{8}, eSize{3}
	const int q{6};
	//std::vector<std::vector<int> > adj{generateBaGraph(vSize, eSize)};
	std::vector<std::vector<int> > adj{
									  {3, 4, 5, 6, 7},
									  {3, 4, 6},
									  {3, 5},
									  {0, 1, 2, 4, 7},
									  {0, 1, 3, 5, 6},
									  {0, 2, 4},
									  {0, 1, 4, 7},
									  {0, 3, 6}
	                                  };
	for (int v{0}; v < static_cast<int>(adj.size()); ++v)
	{
		for (int vp : adj[v])
		{
			std::cout << v << "->" << vp << '\n';
		}
	}

	PropagationLockable pl{adj};

	//auto ops{generateOps(vSize, q, {1.0 / 3, 1.0 / 3, 1.0 / 3})};
	std::vector<std::array<int, 3>> ops{
		                               {0, 0, 1},
		                               {1, 5, 1},
									   {2, 0, 0},
									   {1, 5, 0},
									   {1, 3, 1},
									   {2, 0, 0}
	                                   };

	for (int i{0}; i < q; ++i)
	{
		switch (ops[i][0])
		{
		case 0:
		{
			pl.set(ops[i][1], ops[i][2]);
			std::cout << i << ": set(" + std::to_string(ops[i][1]) + ',' + std::to_string(ops[i][2]) + ")\n";
			break;
		}

		case 1:
		{
			pl.setLock(ops[i][1], ops[i][2]);
			std::cout << i << ": setLock(" + std::to_string(ops[i][1]) + ',' + std::to_string(ops[i][2]) + ")\n";
			break;
		}

		case 2:
		{
			std::cout << i << ": propagate()\n";
			pl.propagate();
			break;
		}
		}
		plot(i, pl);
	}

}

void large()
{
	const int vSize{100}, eSize{3};
	const int q{30};
	std::vector<std::vector<int> > adj{generateBaGraph(vSize, eSize)};

	PropagationLockable pl{adj};

	auto ops{generateOps(vSize, q, {1.0 / 3, 1.0 / 3, 1.0 / 3})};
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
	std::cout << "done" << std::endl;
	plot(100, pl);
}

int main()
{
	//small();
	large();
	return 0;
}
