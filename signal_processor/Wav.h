#include <cstdint>
#include <string>
#include <vector>

#define FOURCC_LENGTH 4 // Four character code
#define START_OF_FMT_DATA 20 // In bytes

// .WAV file header struct
struct WavHeader {
    /*---------RIFF chunk----------*/
    char chunkId[FOURCC_LENGTH]; // "RIFF"
    uint32_t chunkSize;
    char format[FOURCC_LENGTH]; // "WAVE"
    /*---------Format chunk----------*/
    char subchunk1Id[FOURCC_LENGTH]; // "fmt "
    uint32_t subchunk1Size;
    uint16_t audioFormat; // SUBCHUNK 1 START
    uint16_t numChannels; // mono = 1, stereo = 2
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample; // 20 Bytes for RIFF and Format (w/o LIST ...)
    /*---------Data chunk----------*/
    char subchunk2Id[FOURCC_LENGTH]; // "data"
    uint32_t subchunk2Size;
    // SUBCHUNK 2 START (audio data)
};

class WavFile {
public:
    WavHeader header;
    
    WavFile(std::string file);

    bool load(std::string file);
    std::vector<double> extractSignal(bool convertToMono = true);
    operator bool() { return valid; }

private:
    std::vector<uint8_t> audioData;
    bool valid;
};