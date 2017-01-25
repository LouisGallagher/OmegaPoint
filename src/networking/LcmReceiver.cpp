#include "LcmReceiver.h"

LcmReceiver::LcmReceiver(lcm::LCM & lcm)
:lcm(lcm),
 receiving(ATOMIC_FLAG_INIT),
 receiveThread(0)
{
	if(!lcm.good())
	{
		std::exit(EXIT_FAILURE);
	}
}

LcmReceiver::~LcmReceiver()
{

}

void LcmReceiver::receive()
{
	 while(receiving.test_and_set(std::memory_order_acquire) &&
	 	   lcm.handleTimeout(100) >= 0);

	 receiving.clear(std::memory_order_release);
}

void LcmReceiver::start()
{
	assert(!receiving.test_and_set(std::memory_order_acquire) && !receiveThread);
	
	receiveThread = new boost::thread(boost::bind(&LcmReceiver::receive, this));
}

void LcmReceiver::stop()
{
	assert(receiving.test_and_set(std::memory_order_acquire) && receiveThread);

	receiving.clear(std::memory_order_release);

	receiveThread->join();

	receiveThread = 0;
}