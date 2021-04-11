#include <sstream>
#include <iostream>

int main() {
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << "0x2c";
	ss >> x;
	// output it as a signed type
	std::cout << std::to_string(static_cast<int>(x)) << std::endl;
}
