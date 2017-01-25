#include "LiveLcmLogReader.h"

LiveLcmLogReader::LiveLcmLogReader(std::string name)
:LogReader("", false),
 name(name),
 lastFrameTime(-1),
 lastGot(-1),
 frameBuffers(100)
{
	decompressionBufferDepth = new Bytef[Options::get().width * Options::get().height * 2];
	decompressionBufferImage = new Bytef[Options::get().width * Options::get().height * 3];
}

LiveLcmLogReader::~LiveLcmLogReader()
{
	delete [] decompressionBufferImage;
	delete [] decompressionBufferDepth;
}

void LiveLcmLogReader::getNext()
{
	delete rgb;
	delete depth;
 	
	frameBuffers.pop((uint8_t **)&rgb, (uint8_t **)&depth, timestamp);

	imageSize = Options::get().width * Options::get().height * 3;
	depthSize = Options::get().width * Options::get().height * 2;
}

void LiveLcmLogReader::onFrame(const lcm::Frame * frame)
{
	uint8_t * dp = (uint8_t *)malloc(Options::get().width * Options::get().height * 2);
	uint8_t * im = (uint8_t *)malloc(Options::get().width * Options::get().height * 3);

	if(frame->compressed)
	{
		unsigned long decompressedDepthSize = Options::get().width * Options::get().height * 2;
		Bytef decompressionBuffer[decompressedDepthSize];
		uncompress(&decompressionBuffer[0], (unsigned long*)&decompressedDepthSize, (const Bytef*)(frame->depth.data()), frame->depthSize);		

		CvMat tempMat = cvMat(1, frame->imageSize, CV_8UC1, (void *)(frame->image.data()));
		IplImage * decompressedImage = cvDecodeImage(&tempMat);

		memcpy(dp, (unsigned char*)&decompressionBuffer[0], Options::get().width * Options::get().height * 2);
		memcpy(im, (unsigned char*)decompressedImage->imageData, Options::get().width * Options::get().height * 3);

		cvReleaseImage(&decompressedImage);
	}
	else
	{
		memcpy(dp, frame->depth.data(), Options::get().width * Options::get().height * 2);
		memcpy(im, frame->image.data(), Options::get().width * Options::get().height * 3);
	}

	frameBuffers.push(im, dp, frame->timestamp);
}

const std::string LiveLcmLogReader::getFile()
{
	return name;
}

std::string LiveLcmLogReader::getName()
{
	return name;
}