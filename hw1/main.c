#include "bmp.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//Print info about how to use the app
void usage(){
    printf( "Usage: progName <infile> <outfile> [parallel] [threshold] [filter]\n" );
    printf( "<infile> = Input bmp file\n" );
    printf( "<outfile> = Output bmp file\n" );
}


void oil_painting_filter(Image * image, Image * out) {
    int x, y;

    int intensityLevels = 20; 
    float radius = 5.0f;

    for (y = 0; y < 3 * image->sizeY; y+=3) {
        for (x = 0; x < 3 * image->sizeX; x+=3) {
            int max = -1;
            int maxIndex = -1;
            int intensityCount[20] = {};
            int sumR[20] = {};
            int sumG[20] = {};
            int sumB[20] = {};
            int pixelY = y / 3;
            int pixelX = x / 3;

            for (int subY = y - (3 * radius); subY < y + (3 * radius); subY += 3) {
                for (int subX = x - (3 * radius); subX < x + (3 * radius); subX += 3) {
                    if (subY >= 0 && subX >= 0 && subY < 3 * image->sizeY && subX < 3 * image->sizeX) {
                        int subPixelY = subY / 3;
                        int subPixelX = subX / 3;
                        float distance = std::sqrt(
                                std::pow((float) (pixelX - subPixelX), 2.0f) +
                                std::pow((float) (pixelY - subPixelY), 2.0f));

                        if (distance > radius) {
                            continue;
                        }

                        int index = subY * image->sizeX + subX;
                        int red = image->data[index];
                        int green = image->data[index + 1];
                        int blue = image->data[index + 2];
                        
                        int currentIntensity = (int) (((float) (((red + green + blue) / 3) * intensityLevels)) / 255.0f);
                        intensityCount[currentIntensity]++;
                        sumR[currentIntensity] += red;
                        sumG[currentIntensity] += green;
                        sumB[currentIntensity] += blue;
                   }
                }
            }

            for (int intensity = 0; intensity < 20; ++intensity) {
                if (intensityCount[intensity] > max) {
                    max = intensityCount[intensity];
                    maxIndex = intensity;
                }
            }

            int out_index = y * image->sizeX + x;
            out->data[out_index] = sumR[maxIndex] / max;
            out->data[out_index + 1] = sumG[maxIndex] / max;
            out->data[out_index + 2] = sumB[maxIndex] / max;
        }
    }
}


int main(int argc, char **argv)
{
    if (argc < 3) {
        usage();
        return 1;
    }
    
    char *inputFileName = argv[1];
    char *outputFileName = argv[2];
    
    Image *image = (Image *) malloc(sizeof(Image));
    Image *out = (Image *) malloc(sizeof(Image));
    if (image == NULL) {
        fprintf(stderr, "Out of memory\n");
        return(-1);
    }
    
    if (ImageLoad(inputFileName, image)==-1 ||
            ImageLoad(inputFileName, out)==-1 ) {
        printf ("Can't read the image\n");
        return -1;
    }
    
    struct timeval startTimeOfDay, finishTimeOfDay;
    gettimeofday (&startTimeOfDay, NULL);
    
    printf("Apply Filter\n");
    oil_painting_filter(image, out);
    
    if (ImageSave(outputFileName, out)==-1) return(-1);
    free(image);
    free(out);
    return 0;
}


