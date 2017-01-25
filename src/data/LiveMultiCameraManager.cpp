#include "LiveMultiCameraManager.h"


LiveMultiCameraManager::LiveMultiCameraManager()
{
	snprintf(lcm_url, 256, "udpm://239.255.76.67:7667?ttl=%d", Options::get().ttl);
	
	lcm = new lcm::LCM(std::string(lcm_url));

	if(!lcm->good())
	{
		std::exit(EXIT_FAILURE);
	}

	handler = new LcmHandler(deviceDemux, devices);
	lcm->subscribe(Options::get().channel, &LcmHandler::onMessage, handler);

	receiver = new LcmReceiver(*lcm);
	receiver->start();
}

LiveMultiCameraManager::~LiveMultiCameraManager()
{
	receiver->stop();

	delete receiver;

	delete handler;
}	