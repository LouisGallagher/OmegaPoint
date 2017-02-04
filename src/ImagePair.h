#ifndef IMAGEPAIR_H_
#define IMAGEPAIR_H_

#include <pangolin/pangolin.h>

class ImagePair
{
	public:
		ImagePair(pangolin::View & dp, pangolin::View & rgb, std::string name)
		:rgb(rgb),
		 dp(dp),
		 rgbLabel(name + "/image.image", name + "/image"),
		 dpLabel(name + "/depth.depth", name + "/depth"),
		 rgbTex(640, 480),
		 dpTex(640, 480),
		 name(name)
		{
			rgbImg.Alloc(640, 480, pangolin::VideoFormatFromString("RGB24"));
			dpImg.Alloc(640, 480, pangolin::VideoFormatFromString("RGB24"));
		}
		ImagePair(const ImagePair & other):ImagePair(other.dp, other.rgb, other.name)
		{}

		~ImagePair(){}

		void Upload()
		{
			rgbTex.Upload(rgbImg.ptr, GL_RGB, GL_UNSIGNED_BYTE);
			dpTex.Upload(dpImg.ptr, GL_RGB, GL_UNSIGNED_BYTE);
		}

		void Display()
		{
			rgb.Activate();
    		rgbTex.RenderToViewport(true);

    		dp.Activate();
       		dpTex.RenderToViewport(true);
		}

		std::string name;
		pangolin::View & rgb;
		pangolin::View & dp;

		pangolin::Var<std::string> rgbLabel;
		pangolin::Var<std::string> dpLabel;

		pangolin::TypedImage rgbImg;
		pangolin::TypedImage dpImg;

		pangolin::GlTexture rgbTex;
		pangolin::GlTexture dpTex;
};



#endif //IMAGE_PAIR_H_