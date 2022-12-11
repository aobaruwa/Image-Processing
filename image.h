#ifndef IMAGE_H
#define IMAGE_H

class Source;

class Pixel
{
    public:
        unsigned char r, g, b;
};

class Image{
    public:
        Image(); // constructor 1
        Image(int h, int w); // constructor 2
        Image(Image &); // copy constructor

    private:
        int height, width, maxval;
        Pixel *data;
        Source *source;
    
    public:
        int GetHeight();
        int GetWidth();
        int GetMaxval();
        void SetWidth(int w);
        void SetHeight(int h);
        void ResetSize(int h, int w);
        void SetData(Pixel *input_data);
        Pixel *GetData();
        void SetSource(Source *src);
        void Update();

};

#endif