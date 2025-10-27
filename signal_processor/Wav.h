#include <cstdint>
#include <string>
#include <vector>

#define FOURCC_LENGTH 4 // Four character code

struct WavHeader {
    /*---------RIFF chunk----------*/
    char chunkId[FOURCC_LENGTH]; // "RIFF"
    uint32_t chunkSize;
    char format[FOURCC_LENGTH]; // "WAVE"
    /*---------Format sub-chunk----------*/
    char subchunk1Id[FOURCC_LENGTH]; // "fmt "
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels; // mono = 1, stereo = 2
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    /*---------Data sub-chunk----------*/
    char subchunk2Id[FOURCC_LENGTH]; // "data"
    uint32_t subchunk2Size;
};

class WavFile {
public:
    WavFile(std::string file);

    bool load(std::string file);
    std::vector<double> extractSignal(bool convertToMono = true);

private:
    WavHeader header;
    std::vector<uint8_t> audioData;
};