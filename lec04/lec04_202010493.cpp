//202010493

#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void sameImg(int ImgSize, BYTE* Image, BYTE* Output);
// void brighterImg(int ImgSize , BYTE* Image, BYTE* Output); -- replaced with 'BrightnessAdj'
void BrightnessAdj(BYTE*Img, BYTE*Out, int W, int H, int Val);
// void negativeImg(int ImgSize , BYTE* Image, BYTE* Output); -- replaced with 'InverseImg'
void InverseImg (BYTE*Img, BYTE*Out, int W, int H);
void outputPrinter(const char* filename, int ImgSize, BYTE* Image, BYTE* Output);
void freeer (BYTE* Image, BYTE* Output);

FILE* fp;
BITMAPFILEHEADER hf; //14 byte
BITMAPINFOHEADER hInfo; // 40 byte
RGBQUAD hRGB[256]; // 1024 byte

int main() {
    fp = fopen("coin.bmp", "rb");
	if(fp == NULL) return -1;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);

    int ImgSize = hInfo.biWidth * hInfo.biHeight; 
    BYTE * Image = (BYTE*)malloc(ImgSize); 
    BYTE * Output = (BYTE*)malloc(ImgSize);
    fread(Image, sizeof(BYTE), ImgSize, fp);

	sameImg(ImgSize, Image, Output);
    // brighterImg(ImgSize, Image, Output); -- replaced with 'BrightnessAdj'
    BrightnessAdj(Image,Output,hInfo.biWidth,hInfo.biHeight,70);
	// negativeImg(ImgSize, Image, Output); -- replaced with 'InverseImg'
    InverseImg (Image, Output, hInfo.biWidth, hInfo.biHeight);
    freeer(Image, Output);
};

void sameImg(int ImgSize, BYTE* Image, BYTE* Output){
    for (int i = 0; i < ImgSize; i++) {
        Output[i] = Image[i];
    }
    outputPrinter("output1",ImgSize, Image, Output);
};

/* -- replaced with 'BrightnessAdj'
void brighterImg(int ImgSize, BYTE* Image, BYTE* Output){
    for (int i = 0; i < ImgSize; i++) {
        Output[i] = 255 - Image[i] + 50;
    }
    
    outputPrinter("output2", ImgSize, Image, Output);
};
*/

void BrightnessAdj(BYTE*Img, BYTE*Out, int W, int H, int Val){
    int ImgSize = W*H;
    for (int i = 0; i < ImgSize; i++) {
        if(Img[i] + Val > 255){
            Out[i] = 255;
        }else if(Img[i] + Val < 0){
            Out[i] = 0;
        }else Out[i] = Img[i] + Val;
    };
    outputPrinter("output2", ImgSize, Img, Out);
};

/* -- replaced with 'InverseImg'
void negativeImg(int ImgSize, BYTE* Image, BYTE* Output){
    for (int i = 0; i < ImgSize; i++) {
		Output[i] = 255 - Image[i];
	}

    outputPrinter("output3",ImgSize, Image, Output);
};
*/

void InverseImg (BYTE*Img, BYTE*Out, int W, int H){
    int ImgSize = W*H;
    for (int i = 0; i < ImgSize; i++) {
		Out[i] = 255 - Img[i];
	}
    return;
    outputPrinter("output3", ImgSize, Img, Out);
};

void outputPrinter(const char* filename, int ImgSize, BYTE* Image, BYTE* Output){
    char path[256];
    snprintf(path, sizeof(path),"%s.bmp", filename);
    fp = fopen(path, "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);
}

void freeer (BYTE* Image, BYTE* Output){
    free(Image);
    free(Output);
};
