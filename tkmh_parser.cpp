/**
 * @file tkmh_parser.cpp
 * @brief Complete C++ parser implementation for loading and validating .chiral binary container files.
 * Conforming to the TKMH Normative Specification v1.0.0-rc.1
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <string>

#pragma pack(push, 1)
struct ChiralHeader {
    char magic[4];          // "CHRL" (0x4348524C)
    uint32_t version;       // Version Identifier (e.g., 0x00010000)
    uint32_t totalJoints;   // Total Joints (N)
    uint32_t flags;         // Topological and compression flags
    uint64_t tensorOffset;  // Byte offset to Tensors blob
    uint64_t waveOffset;    // Byte offset to VertebralHeights blob
    uint64_t weightOffset;  // Byte offset to JointWeights blob
    float kappaLimit;       // Global Frobenius norm limit
};
#pragma pack(pop)

class ChiralParser {
public:
    static bool LoadAndValidate(const std::string& filepath) {
        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "[ERROR] Failed to open file: " << filepath << std::endl;
            return false;
        }

        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        if (fileSize < static_cast<std::streamsize>(sizeof(ChiralHeader))) {
            std::cerr << "[ERROR] File size (" << fileSize << " bytes) is smaller than mandatory ChiralHeader size (44 bytes)." << std::endl;
            return false;
        }

        ChiralHeader header;
        if (!file.read(reinterpret_cast<char*>(&header), sizeof(ChiralHeader))) {
            std::cerr << "[ERROR] Failed to read .chiral binary header." << std::endl;
            return false;
        }

        // 1. Verify Magic Signature
        if (std::memcmp(header.magic, "CHRL", 4) != 0) {
            std::cerr << "[ERROR] Invalid magic signature. Expected 'CHRL'." << std::endl;
            return false;
        }

        std::cout << "[INFO] Successfully parsed .chiral header:" << std::endl;
        std::cout << "  -> Version:      0x" << std::hex << header.version << std::dec << std::endl;
        std::cout << "  -> Total Joints: " << header.totalJoints << std::endl;
        std::cout << "  -> Flags:        0x" << std::hex << header.flags << std::dec << std::endl;
        std::cout << "  -> Tensor Offset: 0x" << std::hex << header.tensorOffset << std::dec << std::endl;
        std::cout << "  -> Wave Offset:   0x" << std::hex << header.waveOffset << std::dec << std::endl;
        std::cout << "  -> Weight Offset: 0x" << std::hex << header.weightOffset << std::dec << std::endl;
        std::cout << "  -> Kappa Limit:   " << header.kappaLimit << std::endl;

        // 2. Validate Offsets against File Size
        if (header.tensorOffset >= static_cast<uint64_t>(fileSize) ||
            header.waveOffset >= static_cast<uint64_t>(fileSize) ||
            header.weightOffset >= static_cast<uint64_t>(fileSize)) {
            std::cerr << "[ERROR] One or more data payload offsets exceed the file size." << std::endl;
            return false;
        }

        std::cout << "[SUCCESS] .chiral container validation passed without errors." << std::endl;
        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: tkmh_parser <path_to_file.chiral>" << std::endl;
        return 1;
    }

    bool result = ChiralParser::LoadAndValidate(argv[1]);
    return result ? 0 : 1;
}
