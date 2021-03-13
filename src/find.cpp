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
 * Given a 2D vector, get the index of the maximum element in each row. Each row must have a length of > 0.
 *
 * @tparam T The type in the vector.
 * @param table The substring table
 * @return std::vector<int> A vector of the maximum index in each row
 * @throws std::invalid_argument Thrown if any of the rows are of length 0.
 */
template <typename T>
static std::vector<int> get_max_index_in_rows(const std::vector<std::vector<T>> &table) {
	std::vector<int> max_column_in_rows;
	max_column_in_rows.reserve(table.size());
	std::transform(table.cbegin(), table.cend(), std::back_inserter(max_column_in_rows), [](const std::vector<T> &row) {
		auto max_iterator = std::max_element(row.cbegin(), row.cend());
		if (max_iterator == row.cend()) {
			throw std::invalid_argument("cannot find maximum index if of zero length row");
		}

		return std::distance(row.cbegin(), max_iterator);
	});

	return max_column_in_rows;
}

/**
 * Get the index of the largest value in a substring table, so long as its value != 0
 *
 * @tparam T The type in the vector - must be comparable
 * @param table The 2D vector to check
 * @return std::optional<std::pair<int, int>> The index of the maximum element, or an empty optional if the value is
 * zero
 */
template <typename T>
static std::optional<std::pair<int, int>> get_largest_index(const std::vector<std::vector<T>> &table) {
	if (table.size() == 0 || (table.size() > 0 && table.at(0).size() == 0)) {
		return std::nullopt;
	}

	auto max_column_in_row = get_max_index_in_rows(table);
	auto max_row_iterator = std::max_element(
		max_column_in_row.cbegin(),
		max_column_in_row.cend(),
		[&table, row_index = 0](int max_column_index_1, int max_column_index_2) mutable {
			auto res = table.at(row_index).at(max_column_index_1) < table.at(row_index + 1).at(max_column_index_2);
			row_index++;
			return res;
		});

	auto max_row = std::distance(max_column_in_row.cbegin(), max_row_iterator);
	auto max_col = max_column_in_row.at(max_row);
	// If there's no substring length, return an empty optional.
	if (table.at(max_row).at(max_col) == 0) {
		return std::nullopt;
	}

	return std::make_pair(max_row, max_col);
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
