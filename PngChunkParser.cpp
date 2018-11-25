#include "PngChunkParser.h"

PngChunkParser::PngChunkParser(const char* fname) {
	readFile(fname);
}

void PngChunkParser::readFile(const char* fname) {
	FILE* infile = fopen(fname, "rb");
	if (infile!=NULL) {
		if (checkSignature(infile)) {
			std::cout << "Valid PNG signature at start of file" << std::endl;	
			readChunk(infile); // recursively reads chunks until IEND chunk
			std::cout << "end of file" << std::endl;
		}
		else std::cout << "invalid PNG signature at start of file" << std::endl;		
	}
	else std::cout << "could not open file" << std::endl;
}

// recursively reads chunks until IEND chunk
void PngChunkParser::readChunk(FILE* infile) {
	unsigned char length[4];
	unsigned char type[4];
	for (int i=0; i<4; i++) {
		length[i] = fgetc(infile);
		// std::cout << std::hex << +length[i] << std::dec << std::endl;
	}
	for (int i=0; i<4; i++) {
		type[i] = fgetc(infile);
		// std::cout << std::hex << type[i] << std::dec << std::endl;
	}
	if (memcmp(type, IEND, 4) != 0) { // if nonzero difference with IEND chunk type label
		// big endian
		int int_length = (length[0] << 24) | (length[1] << 16) | (length[2] << 8) | (length[3]);
		std::cout << type[0] << type[1] << type[2] << type[3] << " ";
		std::cout << "length: " << int_length << std::endl;
		if (memcmp(type, tEXt, 4) == 0) { // tEXt chunk found!
			// std::cout << "text chunk found!" << std::endl;

			unsigned char data[int_length];
			int index = 0;
			for (int i=0; i<int_length; i++) {
				data[i] = fgetc(infile);
				// std::cout << std::hex << data[i] << std::dec << std:: endl;
				if (data[i] == 0x00) index = i;
			}
			unsigned char key[index];
			unsigned char val[int_length-index];
			for (int i=0; i<index; i++) {
				key[i] = data[i];
				// std::cout << key[i];
			}
			// std::cout << std::endl;
			for (int i=0; i<int_length-index; i++) {
				val[i] = data[i+index+1];
				// std::cout << val[i];
			}
			// std::cout << std::endl;

			std::string keystr((char *) (key), index);
			std::string valstr((char *) (val), int_length-index);
			saveData(keystr, valstr);

			fseek(infile, 4, SEEK_CUR); // skip CRC
		}
		else fseek(infile, int_length + 4, SEEK_CUR); // seek forward length of non-tEXt chunks, plus their CRC
		readChunk(infile);
	}
}

void PngChunkParser::saveData(std::string keystr, std::string valstr) {
	std::cout << keystr << ": " << valstr << std::endl;
	if (keystr == "TimestampString") {
		pngData.timestamp = valstr;
	} else if (keystr == "Seconds: ") {
		pngData.seconds = std::stof(valstr);
	} else if (keystr == "Position") {
		saveFloat3(pngData.position, valstr);	
	} else if (keystr == "Velocity") {
		saveFloat3(pngData.velocity, valstr);
	} else if (keystr == "Attitude") {
		saveFloat3(pngData.attitude, valstr);
	}
	return;	
}

void PngChunkParser::saveFloat3(float dest_float[3], std::string str) {
	std::string::size_type sz;
	dest_float[0] = std::stof(str.substr(1), &sz); // skip opening parenthesis
	dest_float[1] = std::stof(str.substr(sz+2), &sz); // start at end of previous read and skip ", " (2 characters)
	dest_float[2] = std::stof(str.substr(sz+2), &sz);
	return;
}

bool PngChunkParser::checkSignature(FILE* infile) {
	unsigned char buffer[8];
	for (int i=0; i<8; i++) {
		buffer[i] = fgetc(infile);
		// std::cout << std::hex << +buffer[i] << std::dec << std::endl;
	}
	return memcmp(buffer, SPNG, 8) == 0; // if zero differences with expected signature
}

PngChunkParser::PngData PngChunkParser::getPngData() {
	return pngData;
}
