#include <iostream>
#include <thread>
#include <random>

using namespace std::literals::chrono_literals;

extern "C" void __declspec(dllexport) load() {
	std::cout << "le pimposlaw has arrived" << std::endl;
}

extern "C" void __declspec(dllexport) unload() {
	std::cout << "bye comrades" << std::endl;
}

extern "C" void __declspec(dllexport) run(bool& finished) {
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<std::mt19937::result_type> random(1, 10);

	while (!finished) {
		std::this_thread::sleep_for(100ms);
		std::cout << random(generator) << std::endl;
	}
}