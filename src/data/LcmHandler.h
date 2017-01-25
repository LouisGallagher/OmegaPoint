#ifndef LCMHANDLER_H_
#define LCMHANDLER_H_

#include <vector>
#include <unordered_map>

#include "LiveLcmLogReader.h"

class LcmHandler
{
	public:
		LcmHandler(std::unordered_map<std::string, LiveLcmLogReader*> & deviceDemux, std::vector<LiveLcmLogReader*> & devices)
		:deviceDemux(deviceDemux),
		 devices(devices)
		{

		}

		virtual ~LcmHandler()
		{

		}	

		void onMessage(const lcm::ReceiveBuffer * rbuf,  const std::string & chan, const lcm::Frame * frame)
		{
			auto device = deviceDemux.find(frame->senderName);

			if(device == deviceDemux.end())
			{
				LiveLcmLogReader * newDevice = new LiveLcmLogReader(frame->senderName);
				newDevice->onFrame(frame);

				devices.push_back(newDevice);

				deviceDemux[newDevice->getName()] = newDevice;
			}
			else
			{
				device->second->onFrame(frame);
			}
		}

	private:
		std::vector<LiveLcmLogReader * > & devices;
		std::unordered_map<std::string, LiveLcmLogReader*> & deviceDemux;
};
#endif//LCMHANDLER