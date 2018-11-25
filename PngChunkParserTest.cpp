#include "PngChunkParser.h"
#include <iomanip>

int main ()
{
	PngChunkParser *parser = new PngChunkParser("test.png");
	PngChunkParser::PngData data = parser->getPngData();
	std::cout << std::fixed << std::setprecision(5) << std::endl;
	std::cout << "Contents of PngData struct:" << std::endl;
	std::cout << "Timestamp: " << data.timestamp << std::endl;
	std::cout << "Seconds: "   << data.seconds   << std::endl;
	std::cout << "Position: "  << data.position[0] << " " << data.position[1] << " " << data.position[2] << std::endl;
	std::cout << "Velocity: "  << data.velocity[0] << " " << data.velocity[1] << " " << data.velocity[2] << std::endl;
	std::cout << "Attitude: "  << data.attitude[0] << " " << data.attitude[1] << " " << data.attitude[2] << std::endl;
}
