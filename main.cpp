#include <iostream>

#include <CImg.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: ./bill-bill-bill infile" << std::endl;
		return 1;
	}

	const std::string_view infile(argv[1]);
	cimg_library::CImg<unsigned char> image(infile.data());
	for (int i = 0; i < image.width(); i++) {
		for (int j = 0; j < image.height(); j++) {
			int total = 0;
			for (int c = 0; c < image.spectrum(); c++) {
			total += image(i, j, c);
			}

			double avg = total / image.spectrum();
			for (int c = 0; c < image.spectrum(); c++) {
				image(i, j, c) = avg;
			}
		}
	}
	image.display();
}
