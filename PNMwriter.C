#include <stdio.h>
#include <stdlib.h>
#include <PNMwriter.h>

void PNMwriter::Write(char *outfile)
{
    int W, H, maxval=255;
    char magicNum[] = "P6";
    
    
    W = this->img1->GetWidth();
    H = this->img1->GetHeight();

    FILE *f_out = fopen(outfile, "wb");
    fprintf(f_out, "%s\n%d %d\n%d\n", magicNum, W, H, maxval);
    fwrite(this->img1->GetData(), sizeof(Pixel), W*H, f_out);
    fclose(f_out);
}

