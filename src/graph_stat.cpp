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

void countDegree(const std::string &name, int type, int vSize, int eSize)
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

	std::vector<int> degree(vSize);
	for (auto &v : adj)
	{
		for (int vp : v)
		{
			++degree[vp];
		}
	}

	using namespace matplot;
	auto fig{figure(true)};

	auto ax{gca()};
	ax->hist(degree, 50);

	fig->save(name);
}

void countBetweenness(const std::string &name, int vSize, int eSize)
{
	std::vector<std::vector<int>> adj{generateBaGraph(vSize, eSize)};
	std::vector<double> betweenness(getBetweenness(adj));
	using namespace matplot;
	auto fig{figure(true)};
	auto ax{gca()};
	ax->hist(betweenness, 50);
	fig->save(name);
}

void countPageRank(const std::string &name, int vSize, int eSize)
{
	std::vector<std::vector<int>> adj{generateBaGraph(vSize, eSize)};
	std::vector<double> pageRank(getPageRank(adj));
	using namespace matplot;
	auto fig{figure(true)};
	auto ax{gca()};
	ax->hist(pageRank, 50);
	fig->save(name);
}

void countKatz(const std::string &name, int vSize, int eSize)
{
	std::vector<std::vector<int>> adj{generateBaGraph(vSize, eSize)};
	std::vector<double> katz(getKatz(adj));
	using namespace matplot;
	auto fig{figure(true)};
	auto ax{gca()};
	ax->hist(katz, 50);
	fig->save(name);
}

void countLobby(const std::string &name, int vSize, int eSize)
{
	std::vector<std::vector<int>> adj{generateBaGraph(vSize, eSize)};
	std::vector<double> lobby(getLobby(adj));
	using namespace matplot;
	auto fig{figure(true)};
	auto ax{gca()};
	ax->hist(lobby, 50);
	fig->save(name);
}

void showBetweenness(const std::string &name, int vSize, int eSize)
{
	std::vector<std::vector<int>> adj{generateBaGraph(vSize, eSize)};
	std::vector<double> betweenness(getBetweenness(adj));
	std::vector<std::pair<size_t, size_t>> edge;
	for (int v{0}; v < static_cast<int>(adj.size()); ++v)
	{
		for (int vp : adj[v])
		{
			edge.push_back({v, vp});
		}
	}
	std::vector<std::string> labels;
	for (double cb : betweenness)
	{
		labels.push_back(std::to_string(cb));
	}

	using namespace matplot;
	auto fig{figure(true)};
	auto ax{gca()};
	auto g{ax->graph(edge)};
	g->layout_algorithm(network::layout::kawai);
	g->layout_iterations(100000000);
	g->marker_colors(betweenness);
	g->node_labels(labels);
	ax->colormap(palette::paired());
	fig->save(name);
}

void showPageRank(const std::string &name, int vSize, int eSize)
{
	std::vector<std::vector<int>> adj{generateBaGraph(vSize, eSize)};
	std::vector<double> pageRank(getPageRank(adj));
	std::vector<std::pair<size_t, size_t>> edge;
	for (int v{0}; v < static_cast<int>(adj.size()); ++v)
	{
		for (int vp : adj[v])
		{
			edge.push_back({v, vp});
		}
	}
	std::vector<std::string> labels;
	for (double pr : pageRank)
	{
		labels.push_back(std::to_string(pr));
	}

	using namespace matplot;
	auto fig{figure(true)};
	auto ax{gca()};
	auto g{ax->graph(edge)};
	g->layout_algorithm(network::layout::kawai);
	g->layout_iterations(100000000);
	g->marker_colors(pageRank);
	g->node_labels(labels);
	ax->colormap(palette::paired());
	fig->save(name);
}

void showKatz(const std::string &name, int vSize, int eSize)
{
	std::vector<std::vector<int>> adj{generateBaGraph(vSize, eSize)};
	std::vector<double> katz(getKatz(adj));
	std::vector<std::pair<size_t, size_t>> edge;
	for (int v{0}; v < static_cast<int>(adj.size()); ++v)
	{
		for (int vp : adj[v])
		{
			edge.push_back({v, vp});
		}
	}

	std::vector<std::string> labels;
	for (double c : katz)
	{
		labels.push_back(std::to_string(c));
	}

	using namespace matplot;
	auto fig{figure(true)};
	auto ax{gca()};
	auto g{ax->graph(edge)};
	g->layout_algorithm(network::layout::kawai);
	g->layout_iterations(100000000);
	g->marker_colors(katz);
	g->node_labels(labels);
	ax->colormap(palette::paired());
	fig->save(name);
}

void showLobby(const std::string &name, int vSize, int eSize)
{
	std::vector<std::vector<int>> adj{generateBaGraph(vSize, eSize)};
	std::vector<double> lobby(getLobby(adj));
	std::vector<std::pair<size_t, size_t>> edge;
	for (int v{0}; v < static_cast<int>(adj.size()); ++v)
	{
		for (int vp : adj[v])
		{
			edge.push_back({v, vp});
		}
	}

	std::vector<std::string> labels;
	for (double l : lobby)
	{
		labels.push_back(std::to_string(l));
	}

	using namespace matplot;
	auto fig{figure(true)};
	auto ax{gca()};
	auto g{ax->graph(edge)};
	g->layout_algorithm(network::layout::kawai);
	g->layout_iterations(100000000);
	g->marker_colors(lobby);
	g->node_labels(labels);
	ax->colormap(palette::paired());
	fig->save(name);
}

int main()
{
	countDegree("uniform_degree.eps", 0, 1000000, 10000000);
	countDegree("ba_degree.eps", 1, 1000000, 10);

	//countBetweenness("ba_betweenness.eps", 1000, 500);
	//showBetweenness("ba_betweenness_example.eps", 20, 10);
	//countPageRank("ba_PageRank.eps", 1000, 500);
	//showPageRank("ba_PageRank_example.eps", 20, 3);

	//countKatz("ba_katz.eps", 1000, 500);
	//showKatz("ba_katz_example.eps", 20, 5);

	//countLobby("ba_lobby.eps", 1000, 100);
	//showLobby("ba_lobby_example.eps", 20, 3);

	return 0;
}
