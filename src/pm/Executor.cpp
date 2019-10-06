#include "Executor.hpp"
#include <chrono>
#include <thread>
#include "Logger.hpp"

using namespace pm;

void Executor::executeAtFixedRate(long long rate, const std::function<void()>& f) {
	// copypaste, actually. todo cleanup
	using namespace std::chrono;
	auto fpslimit = duration_cast<system_clock::duration>(duration<double>{ 1. / rate });
	auto next = steady_clock::now() + fpslimit;

	long long lastFrame = 0;
	long long currentFrame;
	int frameCount = 0;
	auto prevTime = getTime();
	long long dt;
	while (true) {
		currentFrame = getTime();
		dt = currentFrame - lastFrame;
		lastFrame = currentFrame;
		frameCount++;
		if (currentFrame - prevTime >= 1000000000) {
			if (logRefreshRate)
				log("asd", fmt::format("fps: {}\n", frameCount));
			frameCount = 0;
			prevTime = currentFrame;
		}

		f();
		std::this_thread::sleep_until(next);
		next += fpslimit;
	}
}

void Executor::setLogRefreshRate(bool logRefreshRate) {
	this->logRefreshRate = logRefreshRate;
}
