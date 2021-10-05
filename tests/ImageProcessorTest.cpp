#include <CImg.h>

#include <optional>
#include <utility>

#include "../src/ImageProcessor.hpp"
// This is a _HACK_. Both LibX11 (depended on by CImg) and Gtest define None and Bool, so we must undefine them.
#undef None
#undef Bool

#include <gtest/gtest.h>

class ImageProcessorTest : public ::testing::Test {
 public:
	static constexpr char base_string[] = "Hello World";

 protected:
	std::optional<cimg_library::CImg<unsigned char>> image;
	std::optional<ImageProcessor<unsigned char>> imageProcessor;

	void SetUp() override {
		this->image = cimg_library::CImg<unsigned char>(4, 4);
		fillImage(*(this->image));
		this->imageProcessor = ImageProcessor<unsigned char>(*(this->image), this->base_string);
	}

 private:
	void fillImage(cimg_library::CImg<unsigned char> &image) const {
		for (auto i = 0; i < image.width(); i++) {
			for (auto j = 0; j < image.height(); j++) {
				for (auto c = 0; c < image.spectrum(); c++) {
					image(i, j, c) = (i * j + j) % 255;
				}
			}
		}
	}
};

class ImageProcessorStringParamTest : public ImageProcessorTest, public ::testing::WithParamInterface<std::string> {
 protected:
	std::string param;
};

TEST_F(ImageProcessorTest, FullStringCopiesFullImage) {
	cimg_library::CImg<unsigned char> res = imageProcessor->morph_image(base_string);
	ASSERT_EQ(res, *image);
}

TEST_F(ImageProcessorTest, EmptyStringGivesEmptyImage) {
	const auto res = imageProcessor->morph_image("");
	const auto expected = cimg_library::CImg<unsigned char>(image->width(), static_cast<int>(0));

	ASSERT_EQ(res, expected);
}

TEST_P(ImageProcessorStringParamTest, StringPortionHasExpectedSize) {
	const auto specifier = GetParam();
	const auto expected_height = image->height() * specifier.length() / std::char_traits<char>::length(base_string);
	cimg_library::CImg<unsigned char> res = imageProcessor->morph_image(specifier);

	ASSERT_EQ(res.height(), expected_height);
}

std::string test_strings[] = {"Hello", "Hellold"};

INSTANTIATE_TEST_CASE_P(, ImageProcessorStringParamTest, ::testing::ValuesIn(test_strings));
