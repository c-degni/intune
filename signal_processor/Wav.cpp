#include "Wav.h"
#include <iostream>
#include <fstream>

WavFile::WavFile(std::string file) {
    valid = load(file);
}

/**
 * Generate the fields of a WavFile from a specified .wav file.
 * 
 * @param filePath The file path of the .wav file to load into the WavFile.
 * @return True if and only if all WavFile fields have been properly generated from the given .wav.
 */
bool WavFile::load(std::string filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filePath << "\n";
        return false;
    }

    file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));
    if (std::strncmp(header.chunkId, "RIFF", FOURCC_LENGTH) != 0 && std::strncmp(header.format, "WAVE", FOURCC_LENGTH)) {
        std::cerr << "Not a valid .wav file.\n";
        return false;
    }

    // Parse over non standard (but valid) wav formats including LIST + INFO ...
    if (std::strncmp(header.subchunk2Id, "data", FOURCC_LENGTH) != 0) {
        file.seekg(START_OF_FMT_DATA + header.subchunk1Size); // After fmt chunk
        char chunkId[FOURCC_LENGTH];
        uint32_t chunkSize;

        while (file.read(chunkId, FOURCC_LENGTH)) {
            file.read(reinterpret_cast<char*>(&chunkSize), 4); // Possible chunk size field
            if (std::strncmp(chunkId, "data", FOURCC_LENGTH) == 0) { // Confirm and update fields
                std::memcpy(header.subchunk2Id, chunkId, FOURCC_LENGTH);
                header.subchunk2Size = chunkSize;
                break;
            }
            file.seekg(chunkSize, std::ios::cur); // File ptr set to current + chunkSize
        }
    }

    // Rest of file is actual audio, so read it!
    audioData.resize(header.subchunk2Size);
    file.read(reinterpret_cast<char*>(audioData.data()), header.subchunk2Size);
    return true;
}   

/**
 * Extract normalized PCM signal from WavFile audio data
 * 
 * @param convertToMono True if we would like to handle all number of channels as a singular channel (mono).
 * @return The normalized PCM signal [-1, 1] of the respective WavFile audio data
 */
std::vector<double> WavFile::extractSignal(bool convertToMono) {
    size_t bytesPerSample = header.bitsPerSample / 8;
    size_t numSamples = header.subchunk2Size / (header.numChannels * bytesPerSample);
    size_t outputSize;
    if (convertToMono) outputSize = numSamples;
    else outputSize = numSamples * header.numChannels;
    std::vector<double> signal(outputSize);

    for (size_t i = 0; i < numSamples; i++) {
        double monoSum = 0;
        for (int ch = 0; ch < header.numChannels; ch++) {
            size_t byteIdx = (i * header.numChannels + ch) * bytesPerSample;
            double sample = 0;

            switch (header.bitsPerSample) {
                case 8: {
                    uint8_t val = audioData[byteIdx];
                    sample = (val - 128) / 128.0; // 2^8 / 2
                    break;
                }
                case 12: {
                    int16_t val;
                    std::memcpy(&val, &audioData[byteIdx], 2); // 16 / 8
                    sample = val / 32768.0; // 2^16 / 2
                    break;
                }
                case 24: {
                    int32_t val = 0;
                    std::memcpy(&val, &audioData[byteIdx], 3); // 24 / 8
                    if (val & 0x800000) val |= 0xFF000000;
                    sample = val / 8388608.0; // 2^24 / 2
                    break;
                }
                case 32: {
                    if (header.audioFormat == 3) { // IEEE float
                        float val;
                        std::memcpy(&val, &audioData[byteIdx], 4); // 24 / 8
                        sample = val;
                    } else { // PCM int
                        int64_t val;
                        std::memcpy(&val, &audioData[byteIdx], 4); // 16 / 8
                        sample = val / 2147483648.0; // 2^32 / 2
                    }
                    break;
                }
                default:
                    break;
            }

            if (convertToMono && header.numChannels > 1) monoSum += sample;
            else signal[i * header.numChannels + ch] = sample;
        }

        if (convertToMono && header.numChannels > 1) signal[i] = monoSum / header.numChannels;
    }

    return signal;
}

