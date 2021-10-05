#include <CImg.h>

#include <cmath>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include "find.hpp"

/**
 * Morph the image into one that takes the specifier string, and uses substring positions to take portions of the image.
 * @tparam T The type of data stored in the CImg
 * @param specifier Ths specifier string that determines what parts of the string to splice
 * @return cimg_library::CImg<T> The morphed image
 */
template <typename T>
cimg_library::CImg<T> ImageProcessor<T>::morph_image(const std::string &specifier) const {
	auto ranges = this->get_base_string_ranges(specifier);

	return this->splice_image(ranges);
}

/**
 * Splice the image into parts based on the given ranges from the specified string
 * @tparam T The type to store in the output image
 * @param string_ranges The ranges to splice
 * @return cimg_library::CImg<T> The spliced image
 */
template <typename T>
cimg_library::CImg<T> ImageProcessor<T>::splice_image(const std::vector<std::pair<int, int>> &string_ranges) const {
	const auto total_length =
		std::accumulate(string_ranges.cbegin(), string_ranges.cend(), 0, [](int total, std::pair<int, int> range) {
			return total + (range.second - range.first);
		});

	const auto new_height = (this->base_image.height() * total_length) / this->base_string.length();
	auto res = cimg_library::CImg<T>(this->base_image.width(), new_height, 1, this->base_image.spectrum());

	auto write_height_cursor = 0ul;
	for (auto string_range : string_ranges) {
		auto read_range = std::make_pair(
			this->get_read_cursor_position(string_range.first),
			this->get_read_cursor_position(string_range.second));

		for (auto i = read_range.first; i < read_range.second; (i++, write_height_cursor++)) {
			// We can't write beyond the bounds of the new image. This can happen in cases where the ranges do not
			// evenly divide the final image.
			if (write_height_cursor >= new_height) {
				break;
			}

			for (auto j = 0; j < this->base_image.width(); j++) {
				for (auto c = 0; c < this->base_image.spectrum(); c++) {
					res(j, write_height_cursor, c) = this->base_image(j, i, c);
				}
			}
		}
	}

	return res;
}

template <typename T>
int ImageProcessor<T>::get_read_cursor_position(int position) const {
	return std::ceil(1.0 * this->base_image.height() * position / this->base_string.length());
}

/**
 * Sort the ranges based on the relative specified by the first pair element
 * @param ordered_ranges A vector of ranges that has a relative order as the first item
 * @return std::vector<Range> The ordered ranges
 */
static std::vector<StringIndexRange> sort_specifier_ranges(
	std::vector<std::pair<int, StringIndexRange>> &ordered_ranges) {
	std::sort(ordered_ranges.begin(), ordered_ranges.end());

	auto ranges = std::vector<StringIndexRange>{};
	ranges.reserve(ordered_ranges.size());
	std::transform(
		ordered_ranges.cbegin(),
		ordered_ranges.cend(),
		std::back_inserter(ranges),
		[](const auto &ordinal_pair) { return ordinal_pair.second; });

	return ranges;
}

/**
 * Set a substr of a target string to a given fill character
 * @param target The target string to output to
 * @param fill_char The character to fill
 * @param start_pos The position where the fill should start
 * @param n The number of characetrs to fill
 */
static void set_substr_to_char(std::string &target, char fill_char, int start_pos, int n) {
	for (auto i = 0; i < n; i++) {
		target.at(start_pos + i) = fill_char;
	}
}

/**
 * Get the ranges in which the specifier exists in the base string.
 * @tparam The type of the pixels in the base image
 * @param specifier The string that should be used to generate the image
 * @return std::vector<std::pair<int, int>> A vector of pairs representing the indices (start inclusive, end exclusive)
 * 										    in which a match occurred
 */
template <typename T>
std::vector<StringIndexRange> ImageProcessor<T>::get_base_string_ranges(const std::string &specifier) const {
	// The portions of the specifier string that have not been used, concatened together
	auto remaining_specifier_portions = std::string{specifier};
	// The portions of the specifier string, with chars zeroed out that have already been used
	auto unused_specifier_portions = std::string{specifier};
	auto range_orders = std::vector<std::pair<int, StringIndexRange>>{};
	while (remaining_specifier_portions.length()) {
		auto range = find_longest_common_substring(this->base_string, remaining_specifier_portions);
		if (!range.has_value()) {
			break;
		}

		const auto length = range->second - range->first;
		const auto base_string_substr = this->base_string.substr(range->first, length);

		auto pos_in_specifier = unused_specifier_portions.find(base_string_substr);
		if (pos_in_specifier == std::string::npos) {
			// Prioritize finding a string in the unused specifier portions, but if we can't, then just take it from the
			// specifier
			pos_in_specifier = specifier.find(base_string_substr);
		} else {
			set_substr_to_char(unused_specifier_portions, '\0', pos_in_specifier, length);
		}

		const auto pos_in_remaining = remaining_specifier_portions.find(base_string_substr);
		range_orders.push_back(std::make_pair(pos_in_specifier, *range));

		remaining_specifier_portions =
			remaining_specifier_portions.substr(0, pos_in_remaining) +
			remaining_specifier_portions.substr(pos_in_remaining + base_string_substr.length());
	}

	return sort_specifier_ranges(range_orders);
}
