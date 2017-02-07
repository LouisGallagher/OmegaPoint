#include "LcmLogReader.h"

LcmLogReader::LcmLogReader(std::string name)
:LogReader("", false),
 name(name),
 frameBuffers(100)
{}

LcmLogReader::~LcmLogReader()
{}

void LcmLogReader::getNext()
{
	if(!hasMore()){ return; }

	/*delete rgb;
	delete depth;*/
 	
	imageSize = Options::get().width * Options::get().height * 3;
	depthSize = Options::get().width * Options::get().height * 2;

	frameBuffers.pop(m_rgb, m_depth, timestamp);
}

void LcmLogReader::onFrame(const lcm::Frame * frame)
{
	if((receivedLast = frame->last))return;

	std::unique_ptr<unsigned short> dp;
	std::unique_ptr<unsigned char> im; 

	if(frame->compressed)
	{
		unsigned long decompressedDepthSize = Options::get().width * Options::get().height * 2;
		Bytef decompressionBuffer[decompressedDepthSize];
		uncompress(&decompressionBuffer[0], (unsigned long*)&decompressedDepthSize, (const Bytef*)(frame->depth.data()), frame->depthSize);		

		CvMat tempMat = cvMat(1, frame->imageSize, CV_8UC1, (void *)(frame->image.data()));
		IplImage * decompressedImage = cvDecodeImage(&tempMat);

		dp = std::unique_ptr<unsigned short>((unsigned short *)malloc(decompressedDepthSize));
		im = std::unique_ptr<unsigned char>((unsigned char *)malloc(decompressedImage->imageSize));

		memcpy(dp.get(), (unsigned char*)&decompressionBuffer[0], decompressedDepthSize);
		memcpy(im.get(), (unsigned char*)decompressedImage->imageData, decompressedImage->imageSize);

		cvReleaseImage(&decompressedImage);
	}
	else
	{
		dp = std::unique_ptr<unsigned short>((unsigned short *)malloc(frame->depthSize));
		im = std::unique_ptr<unsigned char>((unsigned char *)malloc(frame->imageSize));

		memcpy(dp.get(), frame->depth.data(), frame->depthSize);
		memcpy(im.get(), frame->image.data(), frame->imageSize);
	}

	frameBuffers.push(std::move(im), std::move(dp), frame->timestamp);
}

const std::string LcmLogReader::getFile()
{
	return name;
}