#ifndef PNM_READER_H
#define PNM_READER_H
#include <source.h>
#include <stdio.h>
#include <string>

using std::string;

class PNMreader : public Source{
    private:
        char *infile;

    public:
        PNMreader(char *file);
        const char *SourceName();
        void Execute();
        void Update();
        ~PNMreader();
};
#endif