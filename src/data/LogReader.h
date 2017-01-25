#ifndef LogReader_H_
#define LogReader_H_

#include <atomic>

#include <zlib.h>

#include "../utils/Options.h"

class LogReader
{
    public:
        LogReader(std::string file, bool flipColors)
         : flipColors(flipColors),
           timestamp(0),
           depth(0),
           rgb(0),
           currentFrame(0),
           decompressionBufferDepth(0),
           decompressionBufferImage(0),
           file(file),
           width(Options::get().width),
           height(Options::get().height),
           numPixels(width * height)
        {}

        virtual ~LogReader()
        {}

        virtual void getNext() = 0;

        virtual int getNumFrames() = 0;

        virtual bool hasMore() = 0;

        virtual bool rewound() = 0;

        virtual void rewind() = 0;

        virtual void getBack() = 0;

        virtual void fastForward(int frame) = 0;

        virtual const std::string getFile() = 0;

        virtual void setAuto(bool value) = 0;

        bool flipColors;
        int64_t timestamp;

        unsigned short * depth;
        unsigned char * rgb;
        int currentFrame;

    protected:
        Bytef * decompressionBufferDepth;
        Bytef * decompressionBufferImage;
        unsigned char * depthReadBuffer;
        unsigned char * imageReadBuffer;
        int32_t depthSize;
        int32_t imageSize;

        const std::string file;
        FILE * fp;
        int32_t numFrames;
        int width;
        int height;
        int numPixels;

        //JPEGLoader jpeg;

};


#endif //LogReader_H_