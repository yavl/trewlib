#include <functional>
#include <chrono>

namespace trew {
	/**
	 * A class that can execute functions at fixed rate.
	 *
	 * Could be used for game server's main loop.
	 */
	class Executor {
	public:
		Executor() = default;
		virtual ~Executor() = default;
		virtual void executeAtFixedRate(long long rate, const std::function<void()>& f);
		virtual void setLogRefreshRate(bool logRefreshRate);
	private:
		inline long long getTime() {
			return timer.now().time_since_epoch().count();
		}

		static std::chrono::high_resolution_clock timer;
		bool logRefreshRate;
	};
}