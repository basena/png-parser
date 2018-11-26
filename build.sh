#!/bin/bash
g++ -std=c++11 PngChunkParserTest.cpp PngChunkParser.cpp -o PngChunkParserTest && \
echo "Build successful. Test with ./PngChunkParserTest"
