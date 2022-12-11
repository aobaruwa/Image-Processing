#ifndef SOURCE_H
#define SOURCE_H
#include <image.h>

class Source {
    protected:
        Image img;
        bool processing;
        virtual void Execute() = 0; // A pure virtual function
    public:
        Image *GetOutput();
      
        virtual const char *SourceName()=0;
        virtual void Update() = 0; 
};
#endif
