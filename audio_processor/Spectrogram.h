#include <vector>
#include "Fft.h"

using spectrogram = std::vector<std::vector<complex_number>>;

/**
 * This first order low pass filter attentuates frequencies of a signal above the specified 
 * cutoff frequency.
 * 
 * @param signal The signal on which to apply the low pass filter.
 * @param sampleRate The sample rate of signal.
 * @param cutoffFreq The frequency at which to attentuate higher frequencies.
 */
void applyLowPassFilter(std::vector<double> &signal, int sampleRate, int cutoffFreq);

/**
 * Downsampling (decimation) will reduce the sample rate of a signal by keeping a fraction of the 
 * original samples to achieve a target sample rate.
 * 
 * @param signal The signal to downsample.
 * @param sampleRate The original sample rate of signal.
 * @param targetSampleRate The desired sample rate of signal.
 */
void downsample(std::vector<double> &signal, int sampleRate, int targetSampleRate);

/**
 * This creates a spectrogram; a visual representation of a signal where rows represent frequency,
 * columns represent time, and values represent respective amplitude.
 * 
 * @param signal The signal the spectrogram will derive from.
 * @param sampleRate The sample rate of signal.
 */
spectrogram Spectrogram(std::vector<double> sample, int sampleRate);