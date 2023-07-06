#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "histogram.h"


using namespace std;

const size_t SCREEN_WIDTH = 40;
const size_t MAX_ASTERISK = SCREEN_WIDTH - 5 - 2;


vector<double> input_numbers(size_t count) {
	vector<double> result(count);
	for (size_t i = 0; i < count; i++) {
		std::cin >> result[i];
	}
	return result;
}

vector<size_t> make_histogram(const vector<double>& numbers, size_t bin_count, vector<double>& captions_for_bins) {
	double min, max;
	vector<size_t> bins(bin_count);

	find_minmax(numbers, min, max);

	double bin_size = (max - min) / bin_count;

	for (size_t i = 0; i < numbers.size(); i++) {
		bool found = false;
		for (size_t j = 0; (j < bin_count - 1) && !found; j++) {
			auto lo = min + j * bin_size;
			auto hi = min + (j + 1) * bin_size;
			if ((lo <= numbers[i]) && (numbers[i] < hi)) {
				bins[j]++;
				found = true;
			}
		}
		if (!found) {
			bins[bin_count - 1]++;
		}
	}
	return bins;
}

void caption_for_histogram(double width, double top, string text) {
	const auto ONE_SIMBOL = 7.5;
	double word_lengs = text.length() * ONE_SIMBOL;
	double x_poin = (width - word_lengs) / 2;
	svg_text(x_poin, top, text);
}


void show_histogram_svg(size_t bin_count, const vector<size_t>& bins, string& histogram_color){
	const auto IMAGE_WIDTH = 400;
	const auto IMAGE_HEIGHT = 700; 
	const auto TEXT_LEFT = 20;
	const auto TEXT_BASELINE = 20;
	const auto TEXT_WIDTH = 50;

	size_t BIN_HEIGHT;
	cerr << "Enter the image height:"; 
	cin >> BIN_HEIGHT;

	if(IMAGE_HEIGHT < BIN_HEIGHT)
	{
		BIN_HEIGHT = (IMAGE_HEIGHT / bin_count);
	}


	const auto BLOCK_WIDTH = 10;
	const auto SHIFT_CAPT = 20;

	size_t max_count = 0;
	bool flag_overflow = false;

	for (size_t i = 0; i < bins.size(); i++) {
		if (bins[i] > MAX_ASTERISK && bins[i] > max_count) {
			max_count = bins[i];
			flag_overflow = true;
		}
	}

	svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);

	double top = 0;
	caption_for_histogram(IMAGE_WIDTH, top + TEXT_BASELINE, "Histogram");
	top = TEXT_BASELINE * 2;

	size_t number_of_stars;
	for (size_t i = 0; i < bins.size(); i++) {
		number_of_stars = bins[i];
		if (flag_overflow) {
			number_of_stars = (int)MAX_ASTERISK * (static_cast<double>(bins[i]) / max_count);
		}
		const double bin_width = BLOCK_WIDTH * number_of_stars;
		svg_text(TEXT_LEFT + SHIFT_CAPT, top + TEXT_BASELINE, to_string(bins[i]));
		svg_rect(TEXT_WIDTH + SHIFT_CAPT, top, bin_width, BIN_HEIGHT, "blue", histogram_color);
		top += BIN_HEIGHT;

	}
	svg_end();
}


int main() {
	size_t number_count;
	cerr << "Enter number count:";
	cin >> number_count;

	cerr << "Enter numbers:";
	const auto numbers = input_numbers(number_count);

	size_t bin_count;
	cerr << "Enter bin count:";
	cin >> bin_count;

	string histogram_color = input_hist_color();

	vector<double> captions_for_bins(bin_count - 1);
	const auto bins = make_histogram(numbers, bin_count, captions_for_bins);

	show_histogram_svg(bin_count, bins, histogram_color);
}