#include "Spectrogram.h"

void applyLowPassFilter(std::vector<double> &signal, int sampleRate, int cutoffFreq) {
    double rc = 1.0 / (2 * M_PI * cutoffFreq); // Time constant of analog RC low pass filter
    double dt = 1.0 / sampleRate; // Sampling period (time between samples)
    double alpha = dt / (rc + dt); // Filter coefficient
    double prevOutput = 0.0;

    for (size_t i = 0; i < signal.size(); i++) {
        if (i == 0) {
            // Scale sample by alpha when there is no previous output
            signal[i] *= alpha;
        } else {
            // Sum new part (current sample scaled by alpha) and old part (previous output scaled by alpha 
            // couterpart) to get the filtered output
            signal[i] = signal[i] * alpha + prevOutput * (1 - alpha);
        }
        prevOutput = signal[i];
    }
}

void downsample(std::vector<double> &signal, int sampleRate, int targetSampleRate) {
    if (sampleRate <= 0 || targetSampleRate <= 0) return; // Sample rates must be positive
    if (sampleRate > targetSampleRate) return; // We need a lower target than original rate to DOWNsample
    
    // Since we are not implementing upsampling, which would enable resampling, we must assume the 
    // target rate to be an integer division of the original. This means if our original to target 
    // rate ratio is 1 or less, there is no work to be done.
    int ratio = sampleRate / targetSampleRate;
    if (ratio <= 1) return;

    size_t n = signal.size(); 
    std::vector<double> resampledSignal;

    // To resample signal, we will process it in ratio (recall from earlier) sized chunks of samples
    for (size_t i = 0; i < n; i += ratio) {
        double sum = 0.0;
        size_t sampleEndIdx = std::min(i + ratio, n); // End of signal is hard end to any sample window

        // Use the average (to avoid aliasing) of the sample window to construct the resampled signal 
        for (size_t j = i; i < sampleEndIdx; j++) sum += signal[j];
        double sampleAvg = sum / (sampleEndIdx - i);
        resampledSignal.push_back(sampleAvg);
    }

    signal = resampledSignal;
}

spectrogram Spectrogram(std::vector<double> sample, int sampleRate) {

}