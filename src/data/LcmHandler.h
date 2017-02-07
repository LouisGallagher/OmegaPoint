#ifndef LCMHANDLER_H_
#define LCMHANDLER_H_

#include <vector>
#include <unordered_map>

#include <lcm/lcm-cpp.hpp>

#include "LcmLogReader.h"

class LcmHandler
{
	public:
		LcmHandler(std::unordered_map<std::string, LcmLogReader*> & deviceDemux, std::vector<LcmLogReader*> & devices)
		:deviceDemux(deviceDemux),
		 devices(devices)
		{}

		virtual ~LcmHandler()
		{}	

		void onMessage(const lcm::ReceiveBuffer * rbuf,  const std::string & chan, const lcm::Frame * frame)
		{
			auto device = deviceDemux.find(frame->senderName);

			if(device == deviceDemux.end())
			{
				LcmLogReader * newDevice = new LcmLogReader(frame->senderName);
				newDevice->onFrame(frame);

				devices.push_back(newDevice);

				deviceDemux[newDevice->getFile()] = newDevice;
			}
			else
			{
				device->second->onFrame(frame);
			}
		}

	private:
		std::vector<LcmLogReader * > & devices;
		std::unordered_map<std::string, LcmLogReader*> & deviceDemux;
};
#endif//LCMHANDLER