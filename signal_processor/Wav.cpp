#include "Wav.h"
#include <iostream>
#include <fstream>

// TODO: Add documentation for each function .. eventually

WavFile::WavFile(std::string file) {
    load(file);
}

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

    // TODO: have to parse over non standard (but valid) wav formats including LIST + INFO ...
}   

std::vector<double> WavFile::extractSignal(bool convertToMono) {

}

