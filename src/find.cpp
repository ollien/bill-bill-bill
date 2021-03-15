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
 * Get the index of the largest value in a substring table, so long as its value != 0
 *
 * @tparam T The type in the vector - must be comparable
 * @param table The substring table to check.
 * @return std::optional<std::pair<int, int>> The index of the maximum element, or an empty optional if the value is
 * zero
 * @throws std::invalid_argument Thrown if any of the rows are of length 0.
 */
template <typename T>
static std::optional<std::pair<int, int>> get_largest_index(const std::vector<std::vector<T>> &table) {
	// We expect that all rows should be of equal size, so we don't have to check every row to get nullopt
	if (table.size() == 0 || table.at(0).size() == 0) {
		return std::nullopt;
	}

	auto max_row_iterator = std::max_element(
		table.cbegin(),
		table.cend(),
		[&table](const std::vector<T> &row1, const std::vector<T> &row2) {
			if (row1.empty() || row2.empty()) {
				throw std::invalid_argument("cannot find maximum of 0 length row");
			}

			return *std::max_element(row1.cbegin(), row1.cend()) < *std::max_element(row2.cbegin(), row2.cend());
		});

	auto max_row_idx = std::distance(table.cbegin(), max_row_iterator);
	auto &max_row = table.at(max_row_idx);
	auto max_col_iterator = std::max_element(max_row.cbegin(), max_row.cend());
	auto max_col_idx = std::distance(max_row.cbegin(), max_col_iterator);

	// If there's no substring length, return an empty optional.
	if (table.at(max_row_idx).at(max_col_idx) == 0) {
		return std::nullopt;
	}

	return std::make_pair(max_row_idx, max_col_idx);
}

/**
 * Find the longest common substring between the two given strings.
 * @param a The first string to check
 * @param b The second string to check
 * @return const std::pair<int, int> The range at which this substring exists, including the start, excluding the end.
 * 									 If no match is found, this will be an empty optional
 */
std::optional<StringIndexRange> find_longest_common_substring(const std::string &a, const std::string &b) {
	auto table = make_substring_table(a, b);
	auto max_index = get_largest_index(table);
	if (!max_index.has_value()) {
		return std::nullopt;
	}

	int end_pos_in_a = max_index->first;
	int len = table.at(end_pos_in_a).at(max_index->second);

	return std::make_pair(end_pos_in_a - (len - 1), end_pos_in_a + 1);
}
