#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <CImg.h>

#include <string>

template <typename T>
class ImageProcessor {
 public:
	ImageProcessor<T>(const cimg_library::CImg<T> &base_image, const std::string &base_string)
		: base_image(base_image), base_string(base_string) {
	}

	cimg_library::CImg<T> morph_image(const std::string &specifier) const;

 private:
	std::string base_string;
	cimg_library::CImg<T> base_image;

	using Range = std::pair<int, int>;
	std::vector<Range> get_base_string_ranges(const std::string &specifier) const;
	cimg_library::CImg<T> splice_image(const std::vector<std::pair<int, int>> &string_ranges) const;
	int get_read_cursor_position(int position) const;
};

#include "ImageProcessor.tpp"

#endif
