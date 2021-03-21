#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <CImg.h>

#include <string>

#include "find.hpp"

/**
 * Process an image for morphing
 * @tparam T The type of pixels stored in the image
 */
template <typename T>
class ImageProcessor {
 public:
	/**
	 * Construct a new ImageProcessor with an image to process and a base string that represents its contents
	 *
	 * @param base_image The image to copy
	 * @param base_string The string that represents the image. For a string of length n, each character represents a
	 * 					  portion equal to image_height/n pixels.
	 */
	ImageProcessor<T>(const cimg_library::CImg<T> &base_image, std::string base_string)
		: base_image(base_image),  // This cannot be a move (or a by-value pass), as we can't make assumptions about
								   // CImg's move construction (see Effective Modern C++ Item 28).
		  base_string(std::move(base_string)) {
	}

	cimg_library::CImg<T> morph_image(const std::string &specifier) const;

 private:
	cimg_library::CImg<T> base_image;
	std::string base_string;

	std::vector<StringIndexRange> get_base_string_ranges(const std::string &specifier) const;
	cimg_library::CImg<T> splice_image(const std::vector<std::pair<int, int>> &string_ranges) const;
	int get_read_cursor_position(int position) const;
};

#include "ImageProcessor.tpp"

#endif
