#ifndef MULTICAMERAMANAGER_H_
#define MULTICAMERAMANAGER_H_

#include <vector>
#include <unordered_map>

#include "LcmLogReader.h"
#include "LcmHandler.h"
#include "../networking/LcmReceiver.h"

class MultiCameraManager
{
	public:
		MultiCameraManager()
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

		virtual ~MultiCameraManager()
		{
			receiver->stop();

			delete receiver;
			delete handler;
			delete lcm;
		}

		std::vector<LogReader *> getDevices() const 
		{
			return std::vector<LogReader *>(devices.begin(), devices.end());
		}


	protected:
		LcmReceiver *receiver;
		LcmHandler *handler;

		std::vector<LcmLogReader*> devices;
		std::unordered_map<std::string, LcmLogReader*> deviceDemux;

		char lcm_url[256];
		lcm::LCM * lcm;
};
#endif //MULTICAMERAMANAGER_H_