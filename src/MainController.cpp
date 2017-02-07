#include "MainController.h"


MainController::MainController()
{}

MainController::~MainController()
{}

void MainController::run()
{
	pangolin::CreateWindowAndBind("OmegaPoint", 1280, 960);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

	pangolin::Display("multi")
		.SetBounds(0.0, 1.0, 0.0, 1.0)
        .SetLayout(pangolin::LayoutEqual);

    unsigned short * depthBuffer = new unsigned short[Options::get().width * Options::get().height];

    std::map<std::string, ImagePair> liveViews;

	while(!pangolin::ShouldQuit())
	{
		std::vector<LogReader *> devices = cameraManager.getDevices();

		std::vector<LogReader *>::iterator device;

		for(auto & device : devices)
		{
			std::map<std::string, ImagePair>::iterator view;

			if(liveViews.count(device->getFile()) == 0)
			{
            	pangolin::View& im = pangolin::Display(device->getFile() + "/image")
    			.SetAspect(640.0f/480.0f);

    			pangolin::View& dp = pangolin::Display(device->getFile() + "/depth")
    			.SetAspect(640.0f/480.0f);

                ImagePair newView(dp, im, device->getFile());

             	pangolin::Display("multi")
             		.AddDisplay(newView.rgb)
             		.AddDisplay(newView.dp);

             	view = liveViews.insert(std::pair<std::string, ImagePair>(device->getFile(), newView)).first;
			}
			else
			{
				view = liveViews.find(device->getFile());
			}  			

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            device->getNext();

            memcpy(view->second.rgbImg.ptr, device->rgb().get(), 640 * 480 * 3);
            memcpy(&depthBuffer[0], device->depth().get(), 640 * 480 * 2);

            float max = 0;
            for(int i = 0; i < Options::get().width * Options::get().height; i++)
            {
            	if(depthBuffer[i] > max)
            	{
            		max = depthBuffer[i];
            	}
            }

            int inner = 0;
            for(int i = 0; i < Options::get().width * Options::get().height; i++, inner += 3)
            {
            	view->second.dpImg.ptr[inner + 0] = ((float)depthBuffer[i] / max) * 255.0f;
            	view->second.dpImg.ptr[inner + 1] = ((float)depthBuffer[i] / max) * 255.0f;
            	view->second.dpImg.ptr[inner + 2] = ((float)depthBuffer[i] / max) * 255.0f;
            }

           	view->second.Upload();
		}
        
        for(auto & view : liveViews)
        {
            view.second.Display();
        }

        pangolin::FinishFrame();
	}	

	delete [] depthBuffer;

	pangolin::Quit();
}

void MainController::launch(int argc, char ** argv)
{
	Options::get(argc, argv);

	run();	
}