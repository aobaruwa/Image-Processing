#include <image.h>
#include <filter.h>
#include <logging.h>
#include <stdio.h>
#include <stdlib.h>

Filter::Filter()
{
    img1 = 0;
    img2 = 0;
    processing = false;
    img.SetSource(this);
}


void Filter::Update()
    { 
        char msg[128];
        if (processing)
        {
            sprintf(msg, "%s: loop detected!", SourceName());
            DataFlowException e(SourceName(), msg);
            throw e;
        }
        processing = true;

        if (img1)
        {
            
            sprintf(msg, "%s: about to update input1", SourceName());
            Logger::LogEvent(msg);
            img1->Update();
            sprintf(msg, "%s: done updating input1", SourceName());
            Logger::LogEvent(msg);
            
        }
        if (img2)
        {
            sprintf(msg, "%s: about to update input2", SourceName());
            Logger::LogEvent(msg);
            img2->Update();
            sprintf(msg, "%s: done updating input2", SourceName());
            Logger::LogEvent(msg);
        }
    
        sprintf(msg, "%s: about to execute", SourceName());
        Logger::LogEvent(msg);
        Execute();
        processing=false;
        sprintf(msg, "%s: done executing", SourceName());
        Logger::LogEvent(msg);
       
        
    }

void Shrinker::Execute()
    {
        int H, W, h, w;
        Image *output = this->GetOutput() ;

        if(!img1)
        {
           char msg[128];
           sprintf(msg, "%s: no input1!", SourceName());
           DataFlowException e(SourceName(), msg);
           throw e;
        }
        H = img1->GetHeight();
        W = img1->GetWidth();
        h = H/2;
        w = W/2;

        output->ResetSize(h, w); 

        for (int i=0 ; i < h; i++){
            for (int j=0; j < w; j++){
                output->GetData()[i*w + j] = img1->GetData()[i*2*W + j*2];
            }
        }
    
    } 


void LRCombine::Execute()
    {
    int width1, width2, height1, height2;
    Image *output;
    

    output =  this->GetOutput();
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }
    if (!img2)
    {
        char msg[128];
        sprintf(msg, "%s: no input2", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }
    width1 = img1->GetWidth();
    width2 = img2->GetWidth();
    height1 = img1->GetHeight(); 
    height2 = img2->GetHeight();

    if (height1 != height2)
    {
        char msg[128];
        sprintf(msg, "%s: heights must match: %d, %d", SourceName(), height1, height2);
        DataFlowException e(SourceName(), msg);
        throw e;
    }
    output->ResetSize( height1, width1+width2);

    for (int i =0; i < output->GetHeight(); i++){
        for (int j = 0; j < output->GetWidth(); j++){
            if (j >= width1){
                output->GetData()[i*output->GetWidth() + j] = img2->GetData()[i*width2 + j- width1];
            }
            else {
                output->GetData()[i*output->GetWidth() + j] = img1->GetData()[i*width1 + j];
                }
        }
    }
  
}

void TBCombine::Execute()
{
    int width1, width2, height1, height2;
    Image *output;

    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }
    if (!img2)
    {
        char msg[128];
        sprintf(msg, "%s: no input2", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }
    output = this->GetOutput();
    height1 = img1->GetHeight();
    height2 = img2->GetHeight();
    width1 = img1->GetWidth();
    width2 = img2->GetWidth();

    if (width1 != width2)
    {
        char msg[128];
        sprintf(msg, "%s: widths must match: %d, %d", SourceName(), width1, width2);
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    output->ResetSize(height1+height2, width1);
    for (int i =0; i < output->GetHeight(); i++){
        for (int j = 0; j < output->GetWidth(); j++){
            if (i >= height1){
                output->GetData()[i*output->GetWidth() + j] = img2->GetData()[(i-height1)*width1 + j];
            }
            else {
                output->GetData()[i*output->GetWidth() + j] = img1->GetData()[i*width1 + j];
                }
        }
    }
}

void Blender::SetFactor(double f)
{
    factor = f;
}

static void blend_pixel(Pixel *pixel1, Pixel *pixel2, double factor, Pixel *output)
{
    
    output->r =  int(pixel1->r * factor + pixel2->r * (1-factor));
    output->g =  int(pixel1->g * factor + pixel2->g * (1-factor));
    output->b =  int(pixel1->b * factor + pixel2->b * (1-factor));
    
}
void Blender::Execute()
{
    Image *output;
    
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }
    if (!img2)
    {
        char msg[128];
        sprintf(msg, "%s: no input2", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    if (img1->GetWidth() != img2->GetWidth())
    {
        char msg[128];
        sprintf(msg, "%s: widths must match: %d, %d", SourceName(), img1->GetWidth(), img2->GetWidth());
        DataFlowException e(SourceName(), msg);
        throw e;
    }
    if (img1->GetHeight() != img2->GetHeight())
    {
        char msg[128];
        sprintf(msg, "%s: heights must match: %d, %d", SourceName(), img1->GetHeight(), img2->GetHeight());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    if (factor < 0.0 || factor > 1.0)
    {
        char msg[128];
        sprintf(msg, "%s: Factor must be between 0 and 1 : %.4f", SourceName(), factor);
        DataFlowException e(SourceName(), msg);
        throw e;
    }
    output = this->GetOutput();
    output->ResetSize(img1->GetHeight(), img1->GetWidth()); // input1 and input2 have same dimensions

    for (int i=0; i < output->GetHeight(); i++){
        for (int j=0; j < output->GetWidth(); j++){
            int loc = i*output->GetWidth() + j;
            blend_pixel(img1->GetData()+loc, img2->GetData()+loc, factor, output->GetData()+loc);
            }   
        }

}

void Mirror::Execute()
{
    //check input
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    int W = img1->GetWidth();
    int H = img1->GetHeight();

    //initialize image
    this->GetOutput()->ResetSize(H, W);

    //buffers
    Pixel *input = img1->GetData();
    Pixel *output = img.GetData();

    //copy data
    for (int i = 0; i <  H ; i++){
        for (int j = 0; j < W; j++){
            //copy pixel info
            output[i*W + j] = input[i*W + (W-j-1)];
        }
    }
}

void Rotate::Execute()
{
    int H, W;
    Image *output = this->GetOutput() ;


    if(! img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    H = img1->GetHeight();
    W = img1->GetWidth();

    output->ResetSize(W, H); 

    for (int i=0 ; i < H; i++){
        for (int j=0; j < W; j++){
            output->GetData()[j*H + H-i-1] = img1->GetData()[i*W + j];
            }
    }
}


//subtract pixels
static void subtract_pixel(Pixel *p1, Pixel *p2, Pixel *output)
{
    //subtract_pixel = max(color_1 - color_2, 0)
    output->r = (p1->r >= p2->r) ? (p1->r - p2->r) : 0;
    output->g = (p1->g >= p2->g) ? (p1->g - p2->g) : 0;
    output->b = (p1->b >= p2->b) ? (p1->b - p2->b) : 0;
}

void Subtract::Execute()
{
    //check input
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    //check input
    if (!img2)
    {
        char msg[128];
        sprintf(msg, "%s: no input2!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    int width = img1->GetWidth();
    int height = img1->GetHeight();

    if (img1->GetWidth() != img2->GetWidth())
    {
        char msg[128];
        sprintf(msg, "%s: widths must match: %d, %d", SourceName(), img1->GetWidth(), img2->GetWidth());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    if (img1->GetHeight() != img2->GetHeight())
    {
        char msg[128];
        sprintf(msg, "%s: heights must match: %d, %d", SourceName(), img1->GetHeight(), img2->GetHeight());
        DataFlowException e(SourceName(), msg);
        throw e;
    }
    //initialize image
    this->GetOutput()->ResetSize(height, width);

    //buffers
    Pixel *input1 = img1->GetData();
    Pixel *input2 = img2->GetData();
    Pixel *output = this->GetOutput()->GetData();

    //subtract images
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //pixel in output image
            int loc = (i * width) + j;
            //subtract
            subtract_pixel(input1 + loc, input2 + loc, output + loc);
        }
    }
}

void Grayscale::Execute()
    {
        int H, W, h, w;
        Image *output = this->GetOutput() ;

        if(! img1)
        {
           char msg[128];
           sprintf(msg, "%s: no input1!", SourceName());
           DataFlowException e(SourceName(), msg);
           throw e;
        }
        
        
        H = img1->GetHeight();
        W = img1->GetWidth();

        output->ResetSize(H, W); 

        for (int i=0 ; i < H*W; i++){
            int gscale_val = img1->GetData()[i].r/5 + img1->GetData()[i].g/2 + img1->GetData()[i].b/4;
            output->GetData()[i].r = gscale_val;
            output->GetData()[i].g = gscale_val;
            output->GetData()[i].b = gscale_val;
        }
        
    }

void Blur::Execute()
    {
        int H, W, h, w;
        Image *output = this->GetOutput() ;

        if(! img1)
        {
           char msg[128];
           sprintf(msg, "%s: no input1!", SourceName());
           DataFlowException e(SourceName(), msg);
           throw e;
        }
        
        H = img1->GetHeight();
        W = img1->GetWidth();

        output->ResetSize(H, W); 

        for (int i=0 ; i < H; i++){
            for (int j=0; j < W; j++){
                if (i == 0 || j == 0 || i == H - 1 || j == W-1)
                {
                    output->GetData()[i*W + j] = img1->GetData()[i*W + j];
                }
                else
                {
                    output->GetData()[i*W+j].r = img1->GetData()[(i-1)*W+j-1].r/8 +
                                                img1->GetData()[(i-1)*W+j].r/8 +
                                                img1->GetData()[(i-1)*W+j+1].r/8 +
                                                img1->GetData()[i*W+j-1].r/8 +
                                                img1->GetData()[i*W+j+1].r/8 +
                                                img1->GetData()[(i+1)*W+j-1].r/8 +
                                                img1->GetData()[(i+1)*W+j].r/8 +
                                                img1->GetData()[(i+1)*W+j+1].r/8 ;
                    output->GetData()[i*W+j].g = img1->GetData()[(i-1)*W+j-1].g/8 +
                                                img1->GetData()[(i-1)*W+j].g/8 +
                                                img1->GetData()[(i-1)*W+j+1].g/8 +
                                                img1->GetData()[i*W+j-1].g/8 +
                                                img1->GetData()[i*W+j+1].g/8 +
                                                img1->GetData()[(i+1)*W+j-1].g/8 +
                                                img1->GetData()[(i+1)*W+j].g/8 +
                                                img1->GetData()[(i+1)*W+j+1].g/8 ;
                    output->GetData()[i*W+j].b = img1->GetData()[(i-1)*W + j-1].b/8 +
                                                img1->GetData()[(i-1)*W+j].b/8 +
                                                img1->GetData()[(i-1)*W+j+1].b/8 +
                                                img1->GetData()[i*W+j-1].b/8 +
                                                img1->GetData()[i*W+j+1].b/8 +
                                                img1->GetData()[(i+1)*W+j-1].b/8 +
                                                img1->GetData()[(i+1)*W+j].b/8 +
                                                img1->GetData()[(i+1)*W+j+1].b/8 ;
            }
    
        }
    }
}

void Color::Execute()
{
    Pixel *output;

    //initialize image
    this->GetOutput()->ResetSize(height, width);

    //buffers
    output = img.GetData();

    //color image
    for (int i = 0; i < height * width; i++)
    {
        output[i].r = red;
        output[i].g = green;
        output[i].b = blue;
    }
}


void CheckSum::OutputCheckSum(const char* file)
{
    FILE *f_in;
    Pixel *input;

    //check input
    if (!img1)
    {
        char msg[128];
        sprintf(msg, "%s: no input1!", SourceName());
        DataFlowException e(SourceName(), msg);
        throw e;
    }

    int width, height;
    unsigned char r = 0, g = 0, b = 0;

    //buffers
    input = img1->GetData();

    //Get image params
    width = img1->GetWidth();
    height = img1->GetHeight();

    //calculate checksum
    for (int i = 0; i < width * height; i++)
    {
        r += input[i].r;
        g += input[i].g;
        b += input[i].b;
    }

    //read file with read binary mode
    f_in = fopen(file, "w");

    //write checksum
    fprintf(f_in, "CHECKSUM: %d, %d, %d\n", r, g, b);

    //close file
    fclose(f_in);
}
