#include "find.h"

#include <cmath>
#include <execution>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

template <typename T>
cimg_library::CImg<T> ImageProcessor<T>::morph_image(const std::string &specifier) const {
	auto ranges = this->get_base_string_ranges(specifier);
	auto total_length = std::accumulate(ranges.cbegin(), ranges.cend(), 0, [](int total, std::pair<int, int> range) {
		return total + (range.second - range.first);
	});

	auto new_height = this->base_image.height() * total_length / this->base_string.length();
	cimg_library::CImg<T> res(this->base_image.width(), new_height, 1, this->base_image.spectrum());
	auto write_height_cursor = 0;
	std::cout << this->base_image.width() << " x " << new_height << std::endl;
	for (auto range : ranges) {
		std::cout << range.first << " " << range.second << std::endl;
		std::pair<int, int> read_range(
		    // these futzings need to be adjusted ... off by ones are annoying as hell
		    // Maybe I just have to break prematurely???
		    std::ceil(1.0 * (this->base_image.height() - 1) * range.first / this->base_string.length()),
		    std::floor(1.0 * (this->base_image.height() - 1) * range.second / this->base_string.length()));

		std::cout << read_range.first << " " << read_range.second << std::endl;
		std::cout << write_height_cursor << std::endl;
		for (int i = read_range.first; i < read_range.second; (i++, write_height_cursor++)) {
			for (int j = 0; j < this->base_image.width(); j++) {
				// std::cout << j << ", " << i << "(cursor=" << write_height_cursor << ")" << std::endl;
				for (int c = 0; c < this->base_image.spectrum(); c++) {
					res(j, write_height_cursor, c) = this->base_image(j, i, c);
				}
			}
		}
		/*
		for (int i = 0; i < this->base_image.width(); (i++, write_height_cursor++)) {
		    std::pair<int, int> read_range(this->base_image.height() * range.first / this->base_string.length(),
		                                   this->base_image.height() * range.second / this->base_string.length());

		    for (int j = read_range.first; j < read_range.second; j++) {
		        for (int c = 0; c < this->base_image.spectrum(); c++) {
		            res(i, write_height_cursor, c) = this->base_image(i, j, c);
		        }
		    }
		}
		*/
		std::cout << write_height_cursor << std::endl << std::endl;
	}

	return res;
}

/**
 * Sort the items in the range vector such that they are in the order they appaeared in the original string
 * @param ranges The vector of ranges to sort
 */
static void sort_range_vector(std::vector<std::pair<int, int>> &ranges) {
	std::sort(ranges.begin(), ranges.end(),
	          [](std::pair<int, int> a, std::pair<int, int> b) { return a.first < b.first; });
}

/**
 * Get the ranges in which the specifier exists in the base string.
 * @tparam The type of the pixels in the base image
 * @param specifier The string that should be used to generate the image
 * @return std::vector<std::pair<int, int>> A vector of pairs representing the indices (start inclusive, end exclusive)
 * 										    in which a match occured
 */
template <typename T>
std::vector<std::pair<int, int>> ImageProcessor<T>::get_base_string_ranges(const std::string &specifier) const {
	// I would love for this to be a string_view but we can't concat string_views :(
	std::string remaining(specifier);
	std::vector<std::pair<int, int>> ranges;
	while (remaining.length()) {
		std::pair<int, int> range = find_longest_common_substring(this->base_string, remaining);
		// TODO: Maybe this should be optional?
		if (range == std::pair<int, int>(0, 0)) {
			break;
		}

		ranges.push_back(range);
		// TODO: This should have a test because it should probably be -1 on the len? not sure
		auto base_string_substr = this->base_string.substr(range.first, range.second - range.first);
		auto pos_in_remaining = remaining.find(base_string_substr);
		remaining =
		    remaining.substr(0, pos_in_remaining) + remaining.substr(pos_in_remaining + base_string_substr.length());
	}

	sort_range_vector(ranges);

	return ranges;
}
