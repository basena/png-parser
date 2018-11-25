#ifndef PNGCHUNKPARSER_H
#define PNGCHUNKPARSER_H

#include <iostream>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

static const unsigned char SPNG[8] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a}; // PNG file signature
static const unsigned char tEXt[4] = {0x74, 0x45, 0x58, 0x74}; // tEXt in ASCII hex
static const unsigned char IEND[4] = {0x49, 0x45, 0x4e, 0x44}; // IEND in ASCII hex

class PngChunkParser {
public:
	struct PngData {
		std::string timestamp;
		float seconds;
		float position[3];
		float velocity[3];
		float attitude[3];
        };		
	PngChunkParser();
	PngChunkParser(const char* fname);
	void readFile(const char* fname);
	PngData getPngData();
private:
	PngData pngData;
	void readChunk(FILE* infile);
	void saveData(std::string keystr, std::string valstr);
	void saveFloat3(float dest_float[3], std::string str); 
	bool checkSignature(FILE* infile);
};


#endif
