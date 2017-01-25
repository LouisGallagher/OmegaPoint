#ifndef LiveMultiCameraLogManager_H_
#define LiveMultiCameraLogManager_H_

#include "../utils/Options.h"
#include "../networking/LcmReceiver.h"
#include "MultiCameraManager.h"
#include "LcmHandler.h"

class LiveMultiCameraManager : public MultiCameraManager
{
	public:
		LiveMultiCameraManager();
		virtual ~LiveMultiCameraManager();

	private:
		LcmReceiver *receiver;
		LcmHandler *handler;
};


#endif // LiveMultiCameraLogManager_H_