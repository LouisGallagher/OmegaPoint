#ifndef MULTICAMERAMANAGER_H_
#define MULTICAMERAMANAGER_H_

#include <vector>
#include <unordered_map>

#include "LiveLcmLogReader.h"

class MultiCameraManager
{
	public:
		MultiCameraManager(){}
		virtual ~MultiCameraManager()
		{
			delete lcm;
		}

		std::vector<LiveLcmLogReader *> getDevices() const 
		{
			//TODO: remove dead log readers
			// for()
			// {

			// }
			return devices;
		}

	protected:
		std::vector<LiveLcmLogReader*> devices;
		std::unordered_map<std::string,LiveLcmLogReader*> deviceDemux;

		char lcm_url[256];
		lcm::LCM * lcm;
};
#endif //MULTICAMERAMANAGER_H_