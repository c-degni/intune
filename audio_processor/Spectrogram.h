#include <vector>
#include "Fft.h"

using spectrogram = std::vector<std::vector<complex_number>>;

void applyLowPassFilter(std::vector<double> &sample, int sampleRate, int cutoffFreq);

void downsample();

spectrogram Spectrogram(std::vector<double> sample, int sampleRate);