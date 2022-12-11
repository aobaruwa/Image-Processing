#ifndef PNM_WRITER_H
#define PNM_WRITER_H
#include <stdio.h>
#include <sink.h>


class PNMwriter : public Sink
{
    public:

        void Write(char *outfile);

};
#endif