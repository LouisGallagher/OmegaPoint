#ifndef MAINCONTROLLER_H_
#define MAINCONTROLLER_H_

#include "ImagePair.h"
#include "../utils/Options.h"
#include "../data/LcmLogReader.h"
#include "../data/MultiCameraManager.h"	

#include <iostream>
#include <vector>
#include <map>

#include <pangolin/pangolin.h>

class MainController
{
	public:
		MainController();
		virtual ~MainController();

		void launch(int argc, char ** argv);

	private:
		MultiCameraManager cameraManager;

		void run();
};
#endif //MAINCONTROLLER_H_