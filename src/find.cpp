#include "find.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <utility>
#include <vector>

/**
 * Make a table representing the length of substrings. A given index being > 0 indicates that that number of characters
 * is equal in both strings. Rows represent indices within a, columns represent within b
 *
 * @param a The first string to check
 * @param a The second string to check
 *
 * @return std::vector<std::vector<int>>
 */
static std::vector<std::vector<int>> make_substring_table(const std::string &a, const std::string &b) {
	std::vector<int> empty_row(b.length(), 0);
	std::vector<std::vector<int>> table(a.length(), empty_row);
	for (int i = 0; i < a.length(); i++) {
		for (int j = 0; j < b.length(); j++) {
			if (a.at(i) == b.at(j)) {
				table.at(i).at(j) = j > 0 && i > 0 ? table.at(i - 1).at(j - 1) + 1 : 1;
			}
		}
	}

	return table;
}

/**
 * Get the index of the largest value in a 2D vector
 *
 * @tparam T The type in the vector - must be comparable
 * @param table The 2D vector to check
 * @return std::optional<std::pair<int, int>> The index of the maximum element
 */
template <typename T>
static std::optional<std::pair<int, int>> get_largest_index(const std::vector<std::vector<T>> &table) {
	std::optional<std::pair<int, int>> max_index;
	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table.at(i).size(); j++) {
			// We won't take a value if there aren't any matching characters
			if (!max_index.has_value() && table.at(i).at(j) == 0) {
				continue;
			} else if (!max_index.has_value()) {
				// We will, however, take one, if there is one, and we haven't stored one yet
				max_index = std::pair<int, int>(i, j);
				continue;
			}

			int candidate = table.at(i).at(j);
			int current_max = table.at(max_index.value().first).at(max_index.value().second);
			if (candidate > current_max) {
				max_index = std::pair<int, int>(i, j);
			}
		}
	}

	return max_index;
}

/**
 * Find the longest common substring between the two given strings.
 * @param a The first string to check
 * @param b The second string to check
 * @return const std::pair<int, int> The range at which this substring exists, including the start, excluding the end.
 * 									 If no match is found, the range will be <0, 0>
 */
std::pair<int, int> find_longest_common_substring(const std::string &a, const std::string &b) {
	auto table = make_substring_table(a, b);
	auto max_index = get_largest_index(table);
	if (!max_index.has_value()) {
		return std::pair<int, int>(0, 0);
	}

	int a_end = max_index.value().first;
	int len = table.at(a_end).at(max_index.value().second);

	return std::pair<int, int>(a_end - (len - 1), a_end + 1);
}
