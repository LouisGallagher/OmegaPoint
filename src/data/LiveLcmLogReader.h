#ifndef LIVELCMLOGREADER_H_
#define LIVELCMLOGREADER_H_

#include <atomic>

#include <opencv2/opencv.hpp>

#include "LogReader.h"
#include "CircularBuffer.h"
#include "../lcmtypes/lcm/Frame.hpp"

class LiveLcmLogReader : public LogReader
{
	public:
		LiveLcmLogReader(std::string name);
		virtual ~LiveLcmLogReader();
     	
     	void getNext();

        int getNumFrames()
        {
			return std::numeric_limits<int>::max();
        }

        bool hasMore()
        {
        	return true;
        }

        bool rewound()
        {
        	return false;
        }

        void rewind(){}

        void getBack(){}

        void fastForward(int frame){}

        const std::string getFile();

        void setAuto(bool value){}
		
		std::string getName();

		void onFrame(const lcm::Frame * frame);

	private:
		int64_t lastFrameTime;
		int lastGot;

		std::atomic<int> latestFrameIndex;

		std::string name;

		/*static const int numBuffers = 100;
		std::pair<std::pair<uint8_t*, uint8_t*>, int64_t> frameBuffers[numBuffers];*/

		CircularBuffer frameBuffers;
};
#endif // LIVELCMLOGREADER_H_
