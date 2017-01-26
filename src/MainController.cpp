#include "MainController.h"


MainController::MainController()
{

}


void MainController::run()
{
	pangolin::CreateWindowAndBind("OmegaPoint", 1280, 960);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

	pangolin::Display("multi")
		.SetBounds(0.0, 1.0, 0.0, 1.0)
        .SetLayout(pangolin::LayoutEqual);

    unsigned short * depthBuffer = new unsigned short[Options::get().width * Options::get().height];

    std::map<std::string, std::pair<pangolin::View &, pangolin::View &>> liveViews;

	while(!pangolin::ShouldQuit())
	{
		std::vector<LiveLcmLogReader *> devices = cameraManager.getDevices();

		std::vector<LiveLcmLogReader *>::iterator device;

		for(device = devices.begin(); device != devices.end(); device++)
		{
			std::map<std::string, std::pair<pangolin::View &, pangolin::View &>>::iterator views;
			
			if(liveViews.count((*device)->getName()) == 0)
			{
				pangolin::View& im = pangolin::Display((*device)->getName() + "/image")
    			.SetAspect(640.0f/480.0f);

    			pangolin::View& dp = pangolin::Display((*device)->getName() + "/depth")
    			.SetAspect(640.0f/480.0f);

             	pangolin::Display("multi")
             		.AddDisplay(im)
             		.AddDisplay(dp);

             	views = liveViews.insert({(*device)->getName(), {im, dp}}).first;
			}
			else
			{
				views = liveViews.find((*device)->getName());
			}  			

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            (*device)->getNext();

            pangolin::TypedImage rgbImg;
            rgbImg.Alloc(640, 480, pangolin::VideoFormatFromString("RGB24"));
            pangolin::TypedImage depthImg;
            depthImg.Alloc(640, 480, pangolin::VideoFormatFromString("RGB24"));

            memcpy(rgbImg.ptr, (*device)->rgb, 640 * 480 * 3);
            memcpy(&depthBuffer[0], (*device)->depth, 640 * 480 * 2);

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
            	depthImg.ptr[inner + 0] = ((float)depthBuffer[i] / max) * 255.0f;
            	depthImg.ptr[inner + 1] = ((float)depthBuffer[i] / max) * 255.0f;
            	depthImg.ptr[inner + 2] = ((float)depthBuffer[i] / max) * 255.0f;
            }

            pangolin::GlTexture rgbTex(640, 480);
			rgbTex.Upload(rgbImg.ptr, GL_RGB, GL_UNSIGNED_BYTE);
			pangolin::GlTexture depthTex(640, 480);
			depthTex.Upload(depthImg.ptr, GL_RGB, GL_UNSIGNED_BYTE);

			views->second.first.Activate();
    		rgbTex.RenderToViewport(true);

    		views->second.second.Activate();
       		depthTex.RenderToViewport(true);
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