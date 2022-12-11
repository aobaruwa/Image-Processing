#include <image.h>
#include <source.h>
#include <stdlib.h>

Image::Image(){}
Image::Image(Image &){}
Image::Image(int h, int w){
    width = w;
    height = h;
    maxval = 255;
    data = new Pixel[width*height];
}
int Image::GetWidth(){return width;}
void Image::SetWidth(int w){width = w;}
int Image::GetHeight(){return height;}
void Image::SetHeight(int h){height = h;}
void Image::ResetSize(int h, int w){
    SetHeight(h);
    SetWidth(w);
    data = new Pixel[width*height];
}
void Image::SetData(Pixel *input_data){
    data = input_data;
}
Pixel *Image::GetData()
{
    return this->data;
}
void Image::SetSource(Source *src)
{
    source = src;
}
void Image::Update()
{
    if (source)
    { 
        source->Update();
    }
}
