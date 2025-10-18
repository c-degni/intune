#include <vector>
#include "Fft.h"
#include <opencv2/opencv.hpp>

using spectrogram = std::vector<std::vector<complex_number>>;
using decibel_map = std::vector<std::vector<double>>;

#define MAX_FREQUENCY 5000 // 5 kHz
#define DOWNSAMPLE_RATIO 4 // Reduce sample to 1/4 of its original sample rate
#define FRAME_SIZE 1024 // Number of samples in each (STFT) window
#define HOP_SIZE FRAME_SIZE / 32 // Sample overlap between adjacent windows

// Types of different window functions used in signal windowing
enum WindowFunction {
    Rectangle,
    Triangle,
    Hanning,
    Hamming,
};

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
 * Multiply a signal by a specified window function to avoid spectral leakage and taper a signal to
 * zero at the edges. If not specified, a rectangle window function will be applied to the signal.
 * 
 * @param signal The signal to be windowed.
 * @param windowType The window function to be applied on signal.
 */
void applyWindowFunction(std::vector<double> &signal, WindowFunction window = Rectangle);

/**
 * This creates a spectrogram; a visual representation of a signal as a function of time, frequency, and
 * amplitude.
 * 
 * @param signal The signal the spectrogram will derive from.
 * @param sampleRate The sample rate of signal.
 * @return A spectrogram where each row corresponds to a time frame and each column within that row is a
 * frequency bin: spectrogram[time = i][frequency = k] = complex amplitude representing the magnitude and phase 
 * of frequency bin k at time i in the signal.
 * 
 * @note For computation and analysis sake, it makes sense for the spectrogram to be indexed [time][frequency].
 * However, when graphing or plotting the spectrogram, make sure to transpose it to be indexed [frequency][time]
 * as it is conventionally oriented.
 */
spectrogram Spectrogram(std::vector<double> signal, int sampleRate);

/**
 * Transposing a spectrogram results in the indexing switching from [time][frequency] to [frequency][time] or
 * vice versa depending on the structure of s initially. The values at indices are still respective complex
 * amplitudes.
 * 
 * @param s The spectrogram to be tranposed.
 * 
 * @note Transposing a matrix twice returns it to its original configuration. The same holds for spectrograms!
 */
void transpose(spectrogram &s);

/**
 * Visualize a spectrogram as a frequency over time "heat map" plot.
 * 
 * @param s The spectrogram to be visualized.
 * 
 * @note It is assumed the spectrogram is conventionally indexed: [frequency][time].
 */
void visualize(spectrogram &s);