#ifndef FILTER_H
#define FILTER_H
#include <source.h>
#include <sink.h>

class Filter : public Source, public Sink
{
    public:
        Filter();
        void Update();
};

class Shrinker : public Filter{
    protected:
        void Execute();
    public:
        const char* SourceName(){return "shrinker";}
};

class LRCombine: public Filter{
    protected:
        void Execute();
    public:
        const char* SourceName(){return "LRCombine";}
};

class TBCombine: public Filter{
    protected:
        void Execute();
    public:
        const char* SourceName(){return "TBCombine";}
};

class Blender : public Filter{
    protected:
        double factor;
        void Execute();
    public:
        void SetFactor(double f);
        const char* SourceName(){return "Blender";}
};

class Mirror: public Filter{
    protected:
        void Execute();
    public:
        const char* SourceName() {return "Mirror";}
};

class Rotate : public Filter
{
  protected:
    void Execute();
  public:
    const char* SourceName() { return "Rotate"; } 
};

class Subtract : public Filter
{
  protected:
    void Execute();
  public:
    const char* SourceName() { return "Subtract"; } 
};

class Grayscale : public Filter
{
  protected:
    void Execute();
  public:
    const char* SourceName() { return "Grayscale"; } 
};

class Blur : public Filter
{
  protected:
    void Execute();
  public:
    const char* SourceName() { return "Blur"; } 
};

class Color : public Filter
{
  int red, green, blue, width, height;
  public:
    Color(int w, int h, int r, int g, int b) : Filter(), height(h), width(w), red(r), green(g), blue(b){};
    const char* SourceName() { return "Color"; } 
  protected:
    void Execute();
};

class CheckSum : public Sink
{
  public:
    const char* SourceName() { return "Checksum"; } 
    void OutputCheckSum(const char* file);
};


#endif