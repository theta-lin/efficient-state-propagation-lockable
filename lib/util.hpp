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

#ifndef UTIL_HPP
#define UTIL_HPP

#include <cassert>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <queue>
#include <stack>
#include <random>
#include <string>
#include <utility>
#include <set>
#include <chrono>
#include <map>

std::mt19937& getRng();
int getRandom(int l, int r);

class RandUnique
{
private:
	const long long size;
	long long curSize;

	struct Node
	{
		long long cnt;
		long long lch;
		long long rch;
	};

	std::vector<Node> nodes;

	long long claim(long long node, long long tl, long long tr, long long pos);

public:
	RandUnique(long long size);
	long long claim();
};

std::vector<std::vector<int>> generateGraph(int vSize, int eSize);
std::vector<std::vector<int>> generateBaGraph(int vSize, int eOut);

std::vector<int> getDegree(const std::vector<std::vector<int>> &adj);
std::vector<double> getBetweenness(const std::vector<std::vector<int>> &adj);
std::vector<double> getPageRank(const std::vector<std::vector<int>> &adj);
std::vector<double> getKatz(const std::vector<std::vector<int>> &adj);
std::vector<double> getLobby(std::vector<std::vector<int>> adj);

class Weighter
{

public:
	virtual std::vector<double> operator()(const std::vector<std::vector<int>> &adj) const = 0;
};

class WeighterNon : public Weighter
{
public:
	std::vector<double> operator()(const std::vector<std::vector<int>> &adj) const override final;
};

class WeighterPower : public Weighter
{
private:
	const double power;

public:
	WeighterPower(double power);
	std::vector<double> operator()(const std::vector<std::vector<int>> &adj) const override final;
};

class WeighterExp : public Weighter
{
private:
	const double base;

public:
	WeighterExp(double base);
	std::vector<double> operator()(const std::vector<std::vector<int>> &adj) const override final;
};

class WeighterCb : public Weighter
{
public:
	std::vector<double> operator()(const std::vector<std::vector<int>> &adj) const override final;
};

class WeighterPr : public Weighter
{
public:
	std::vector<double> operator()(const std::vector<std::vector<int>> &adj) const override final;
};


class WeighterKatz : public Weighter
{
public:
	std::vector<double> operator()(const std::vector<std::vector<int>> &adj) const override final;
};


class WeighterL : public Weighter
{
public:
	std::vector<double> operator()(const std::vector<std::vector<int>> &adj) const override final;
};

std::vector<std::array<int, 3>> generateOps(int vSize, int cnt, const std::array<double, 3> &weights);
std::vector<std::array<int, 3>> generateOps(int cnt, const std::array<double, 3> &queryWeights, const std::vector<double> &vWeights);
std::vector<std::array<int, 3>> generateOps(int cnt, double lock, const std::array<double, 3> &queryWeights, const std::vector<double> &vWeights);

void clearCache();

class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<clock_t> begin;

public:
	Timer() : begin(clock_t::now())
	{
	}

	void reset()
	{
		begin = clock_t::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - begin).count();
	}
};

#endif
