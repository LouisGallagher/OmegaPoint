#ifndef LCMLOGREADER_H_
#define LCMLOGREADER_H_

#include <atomic>

#include <opencv2/opencv.hpp>

#include "LogReader.h"
#include "CircularBuffer.h"
#include "../lcmtypes/lcm/Frame.hpp"

class LcmLogReader : public LogReader
{
	public:
		LcmLogReader(std::string name);
		virtual ~LcmLogReader();
     	
     	void getNext();

        int getNumFrames()
        {
			return std::numeric_limits<int>::max();
        }

        bool hasMore()
        {
        	return !receivedLast || !frameBuffers.empty();
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
		
		void onFrame(const lcm::Frame * frame);

	private:
		std::string name;

        bool receivedLast;

		CircularBuffer frameBuffers;
};
#endif // LCMLOGREADER_H_
