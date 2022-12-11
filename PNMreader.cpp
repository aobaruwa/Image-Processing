#include <image.h>
#include <PNMreader.h>
#include <logging.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

using std::cout, std::endl, std::string;

PNMreader::PNMreader(char *file)
{
    infile = new char[strlen(file) + 1];
    strcpy(infile, file);
	img.SetSource(this);
}

void PNMreader::Execute()
{
    int width, height, maxval;
    char magicNum[128];
    
    FILE *f_in = fopen(infile, "rb");
    if (!f_in)
    {
        char msg[128];
        sprintf(msg, "%s: file not found!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }
    fscanf(f_in, "%s\n%d %d\n%d\n", magicNum, &width, &height, &maxval);
    
    this->GetOutput()->ResetSize(height, width);
    fread(this->GetOutput()->GetData(), sizeof(Pixel), width*height, f_in);
    fclose(f_in); 
	}

const char *PNMreader::SourceName()
{
    return "PNMreader";
}

void PNMreader::Update()
{
	char msg[128];
    sprintf(msg, "%s: about to execute", SourceName());
    Logger::LogEvent(msg);
    Execute();
    sprintf(msg, "%s: done executing", SourceName());
    Logger::LogEvent(msg);
}

PNMreader::~PNMreader()
{
    delete [] infile;
    
}