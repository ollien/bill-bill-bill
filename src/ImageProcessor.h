#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <string>

#include <CImg.h>

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

	std::vector<std::pair<int, int>> get_base_string_ranges(const std::string &specifier) const;
	cimg_library::CImg<T> splice_image(const std::string &specifier) const;
};

#include "ImageProcessor.tpp"

#endif
