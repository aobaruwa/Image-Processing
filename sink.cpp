#include <image.h>
#include <sink.h>

void Sink::SetInput(Image *image){
    this->img1 = image;
}
void Sink::SetInput2(Image *image){
    this->img2 = image;
}