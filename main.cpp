#include "signal_processor/Wav.h"
#include <iostream>
#include "signal_processor/Spectrogram.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "argc != 2";
        return 1;
    }
    
    WavFile file = WavFile(argv[1]);
    if (!file) return 1;

    auto signal = file.extractSignal();
    spectrogram s = Spectrogram(signal, file.header.sampleRate);
    // transpose(s);
    visualize(s);
}