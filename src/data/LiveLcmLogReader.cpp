#include "LiveLcmLogReader.h"

LiveLcmLogReader::LiveLcmLogReader(std::string name)
:LogReader("", false),
 name(name),
 frameBuffers(100)
{}

LiveLcmLogReader::~LiveLcmLogReader()
{}

void LiveLcmLogReader::getNext()
{
	if(!hasMore()){ return; }
	delete rgb;
	delete depth;
 	
	imageSize = Options::get().width * Options::get().height * 3;
	depthSize = Options::get().width * Options::get().height * 2;

	frameBuffers.pop((uint8_t **)&rgb, (uint8_t **)&depth, timestamp);
}

void LiveLcmLogReader::onFrame(const lcm::Frame * frame)
{
	uint8_t * dp = (uint8_t *)malloc(frame->depthSize);
	uint8_t * im = (uint8_t *)malloc(frame->imageSize);

	if(frame->compressed)
	{
		unsigned long decompressedDepthSize = Options::get().width * Options::get().height * 2;
		Bytef decompressionBuffer[decompressedDepthSize];
		uncompress(&decompressionBuffer[0], (unsigned long*)&decompressedDepthSize, (const Bytef*)(frame->depth.data()), frame->depthSize);		

		CvMat tempMat = cvMat(1, frame->imageSize, CV_8UC1, (void *)(frame->image.data()));
		IplImage * decompressedImage = cvDecodeImage(&tempMat);

		memcpy(dp, (unsigned char*)&decompressionBuffer[0], frame->depthSize);
		memcpy(im, (unsigned char*)decompressedImage->imageData, frame->imageSize);

		cvReleaseImage(&decompressedImage);
	}
	else
	{
		memcpy(dp, frame->depth.data(), frame->depthSize);
		memcpy(im, frame->image.data(), frame->imageSize);
	}

	frameBuffers.push(im, dp, frame->timestamp);

	receivedLast = frame->last;
}

const std::string LiveLcmLogReader::getFile()
{
	return name;
}

std::string LiveLcmLogReader::getName()
{
	return name;
}