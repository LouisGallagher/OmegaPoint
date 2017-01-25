#ifndef LCMHANDLER 
#define LCMHANDLER

#include <lcm/lcm-cpp.hpp>

#include <boost/thread.hpp>

#include <atomic>

#include "../utils/Options.h"

class LcmReceiver
{
	public: 
		LcmReceiver(lcm::LCM & lcm);
		virtual ~LcmReceiver();

		void start();
		void stop();
	
	private:
		void receive();

		boost::thread * receiveThread; 

		lcm::LCM & lcm;

		std::atomic_flag receiving{false};
};
#endif //LCMHandler