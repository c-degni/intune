#include "Spectrogram.h"
#include <algorithm>

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
    if (sampleRate <= targetSampleRate) return; // We need a lower target than original rate to DOWNsample
    
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
        for (size_t j = i; j < sampleEndIdx; j++) sum += signal[j];
        double sampleAvg = sum / (sampleEndIdx - i);
        resampledSignal.push_back(sampleAvg);
    }

    signal = resampledSignal;
}

void applyWindowFunction(std::vector<double> &signal, WindowFunction window) {
    // TODO: May find it interesting to implement rectangle, triangle .. later 
    size_t n = signal.size();
    switch (window) {
        case Hamming:
            for (size_t i = 0; i < n; i++) {
                signal[i] *= (.54 - .46 * std::cos(2 * M_PI * i / (n - 1)));
            }
            break;
        case Hanning:
        case Triangle:
        case Rectangle:
        default:
            break;
    }
}

spectrogram Spectrogram(std::vector<double> signal, int sampleRate) {
    applyLowPassFilter(signal, sampleRate, sampleRate / DOWNSAMPLE_RATIO);
    downsample(signal, sampleRate, sampleRate / DOWNSAMPLE_RATIO);

    size_t n = signal.size();
    int numOfWindows = signal.size() / (FRAME_SIZE - HOP_SIZE);
    std::cerr << "Frames: " << numOfWindows << ", Frame size: " << FRAME_SIZE << "\n";
    // spectrogram sgram = spectrogram(numOfWindows);
    spectrogram sgram;
    sgram.reserve(numOfWindows);

    // Perform Short Time Fourier Transform (STFT) by first splitting the signal into short overlapping 
    // windows (frames) where each is a time slice of samples. 
    for (int i = 0; i < numOfWindows; i++) {
        int frameStart = i * HOP_SIZE;
        int frameEnd = std::min(int(n), frameStart + FRAME_SIZE);
        std::vector<double> frame(FRAME_SIZE);
        std::copy(signal.begin() + frameStart, signal.begin() + frameEnd, frame.begin());

        // After applying hamming window function to the time frame, use FFT to convert frame from time to 
        // frequency domain resulting in the corresponding frequency bins for the time frame. 
        applyWindowFunction(frame, Hamming);
        sgram.push_back(FFT(frame));
    } 

    // Spectrogram[time][freq]
    return sgram;
}

void transpose(spectrogram &s) {
    if (s.empty()) return; // Nothing to transpose
    size_t X = s.size();
    size_t Y = s[0].size();
    spectrogram transposed(Y, complex_vector(X));

    for (size_t x = 0; x < X; x++) {
        for (size_t y = 0; y < Y; y++) {
            transposed[y][x] = s[x][y];
        }
    }

    s = transposed;
}

void visualize(spectrogram &s) {
    if (s.empty()) {
        std::cerr << "Nothing to visualize.\n";
        return; // Nothing to visualize
    }
    size_t F = s.size();
    size_t T = s[0].size();

    // Convert amplitudes to decibels which is logarithmic (more distributed visual) and matches how we 
    // actually perceive loudness
    decibel_map dB(F, std::vector<double>(T));
    double minDB = 1e9;
    double maxDB = -1e9;
    for (size_t f = 0; f < F; f++) {
        for (size_t t = 0; t < T; t++) {
            double magnitude = std::abs(s[f][t]) / FRAME_SIZE; // Magnitude of complex amplitude (strength of frequency at time)
            magnitude = 20.0 * std::log10(std::max(magnitude, 1e-10)); // Complex amplitude to dB conversion
            dB[f][t] = magnitude;
            minDB = std::min(minDB, magnitude);
            maxDB = std::max(maxDB, magnitude);
        }
    }

    const double dynamicRange = 80.0;
    for (auto &row : dB) {
        for (auto &col : row) {
            col = std::max(col, maxDB - dynamicRange);
        }
    }

    // Represent decibel map as RGB pixels to create the "heat map" aspect
    cv::Mat img(F, T, CV_8UC1); // 8U = 8 bit pixel (0 - 255), C1 = grayscale (first channel)
    for (size_t f = 0; f < F; f++) {
        for (size_t t = 0; t < T; t++) {
            // Scale pixel brightness to be 0 <= b <= 1 for clear relative intensity
            double brightness = (dB[f][t] - minDB) / (maxDB - minDB);
            
            // Safeguards
            if (brightness > 1.0) brightness = 1.0; 
            if (brightness < 0.0) brightness = 0.0;

            // Transform pixel to RGB but flip image vertically to neutralize drawing rows backwards (an OpenCV thing)
            img.at<uint8_t>(F - 1 - f, t) = static_cast<uint8_t>(brightness * 255.0);
        }
    }

    // TODO: I may consider file output as an option; add output type as a specification parameter.
    // cv::rotate(img, img, cv::ROTATE_180_CLOCKWISE);
    cv::applyColorMap(img, img, cv::COLORMAP_MAGMA);
    cv::namedWindow("Spectrogram", cv::WINDOW_NORMAL);
    // cv::resizeWindow("Spectrogram", T * 2, F * 2);
    cv::imshow("Spectrogram", img);
    cv::waitKey(0); // Pauses program until any key press
}