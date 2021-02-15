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

#ifndef BRUTEFORCE_HPP
#define BRUTEFORCE_HPP

#include <algorithm>
#include <vector>

class Bruteforce
{
private:
	const std::vector<std::vector<int>> adj;

	std::vector<int> queue;

	std::vector<bool> s;
	std::vector<bool> locked;

public:
	Bruteforce(const std::vector<std::vector<int>> &adj);

	bool get(int v);
	bool getLocked(int v);
	void set(int v, bool sp);
	void setLock(int v, bool lockedp);
	void propagate();
};

#endif
