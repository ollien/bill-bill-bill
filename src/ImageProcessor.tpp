#include <cmath>
#include <execution>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "find.h"

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
	auto total_length =
		std::accumulate(string_ranges.cbegin(), string_ranges.cend(), 0, [](int total, std::pair<int, int> range) {
			return total + (range.second - range.first);
		});

	int new_height = (this->base_image.height() * total_length) / this->base_string.length();
	cimg_library::CImg<T> res(this->base_image.width(), new_height, 1, this->base_image.spectrum());

	auto write_height_cursor = 0;
	for (auto string_range : string_ranges) {
		std::pair<int, int> read_range(
			this->get_read_cursor_position(string_range.first),
			this->get_read_cursor_position(string_range.second));

		for (int i = read_range.first; i < read_range.second; (i++, write_height_cursor++)) {
			// We can't write beyond the bounds of the new image. This can happen in cases where the ranges do not
			// evenly divide the final image.
			if (write_height_cursor >= new_height) {
				break;
			}

			for (int j = 0; j < this->base_image.width(); j++) {
				for (int c = 0; c < this->base_image.spectrum(); c++) {
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
 * Get the ranges in which the specifier exists in the base string.
 * @tparam The type of the pixels in the base image
 * @param specifier The string that should be used to generate the image
 * @return std::vector<std::pair<int, int>> A vector of pairs representing the indices (start inclusive, end exclusive)
 * 										    in which a match occurred
 */
template <typename T>
std::vector<std::pair<int, int>> ImageProcessor<T>::get_base_string_ranges(const std::string &specifier) const {
	// I would love for this to be a string_view but we can't concat string_views :(
	std::string remaining(specifier);
	std::vector<std::pair<int, int>> ranges;
	while (remaining.length()) {
		auto range = find_longest_common_substring(this->base_string, remaining);
		if (!range.has_value()) {
			break;
		}

		ranges.push_back(*range);
		auto base_string_substr = this->base_string.substr(range->first, range->second - range->first);
		auto pos_in_remaining = remaining.find(base_string_substr);
		remaining =
			remaining.substr(0, pos_in_remaining) + remaining.substr(pos_in_remaining + base_string_substr.length());
	}

	std::sort(ranges.begin(), ranges.end());

	return ranges;
}
