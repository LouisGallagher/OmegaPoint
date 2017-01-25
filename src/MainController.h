#ifndef MAINCONTROLLER_H_
#define MAINCONTROLLER_H_

#include "../utils/Options.h"
#include "../data/LiveLcmLogReader.h"
#include "../data/LiveMultiCameraManager.h"	

#include <iostream>
#include <vector>
#include <map>

#include <pangolin/pangolin.h>

class MainController
{
	public:
		MainController();
		virtual ~MainController(){}

		void launch(int argc, char ** argv);

	private:
		LiveMultiCameraManager cameraManager;

		void run();
};
#endif //MAINCONTROLLER_H_