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

#ifndef PROPAGATION_LOCKABLE_HPP
#define PROPAGATION_LOCKABLE_HPP

#include <cmath>
#include <algorithm>
#include <vector>
#include <string>

class PropagationLockable
{
private:
	const std::vector<std::vector<int>> adj;

	int crit;

	std::vector<int> degree;
	std::vector<std::vector<int>> large;
	std::vector<int> queue;

	std::vector<bool> s;
	std::vector<bool> locked;
	std::vector<int> sources;
	std::vector<std::vector<int>> targets;

public:
	PropagationLockable(const std::vector<std::vector<int>> &adj, double critCoeff = 1.0);

	bool get(int v);
	bool getLocked(int v);

	int countS();
	std::vector<std::pair<size_t, size_t>> getEdges();
	std::vector<float> getSizes();
	std::vector<double> getColors();
	std::vector<std::string> getLabels();

	void set(int v, bool sp);
	void setLock(int v, bool lockedp);
	void propagate();
};

#endif
