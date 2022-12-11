#ifndef SINK_H
#define SINK_H
#include <image.h>

class Sink{
    protected:
        Image *img1, *img2;
    public:
        void SetInput(Image *img1);
        void SetInput2(Image *img2);
};

#endif