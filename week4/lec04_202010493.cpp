//202010493

#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

FILE* fp;
BITMAPFILEHEADER hf; //14 byte
BITMAPINFOHEADER hInfo; // 40 byte
RGBQUAD hRGB[256]; // 1024 byte

void sameImg(int ImgSize, BYTE* Image, BYTE* Output);
void BrightnessAdj(BYTE*Img, BYTE*Out, int W, int H, int Val);
void InverseImg (BYTE*Img, BYTE*Out, int W, int H);
void outputPrinter(const char* filename, int ImgSize, BYTE* Image, BYTE* Output);
void ContrastAdj(BYTE*Img, BYTE*Out, int W, int H, double Val);
void freeer (BYTE* Image, BYTE* Output);
void ObtainHistogram (BYTE*Img,int*Histo,int W, int H);
void HistogramStretching(BYTE*Img, int* Histo, BYTE*Out, int W, int H);
void ObtainAHisto(int* Histo, int* AHisto);
void HistogramEqualization(BYTE*Img, int* Histo, int* AHisto, BYTE*Out, int W, int H);
void Binarization(BYTE*Img, BYTE*Out, int W, int H, BYTE Threshold);
// int GonzalezBinThresh(); // <-과제

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
    
    int Histo[256] = {0};
    int AHisto[256] = {0};
    ObtainHistogram(Image,Histo,hInfo.biWidth,hInfo.biHeight);
    ObtainAHisto(Histo,AHisto);

    // int Thres = GonzalezBinThresh();

	sameImg(ImgSize, Image, Output);
    BrightnessAdj(Image,Output,hInfo.biWidth,hInfo.biHeight,70);
    ContrastAdj(Image,Output,hInfo.biWidth,hInfo.biHeight,1.5);
    InverseImg (Image, Output, hInfo.biWidth, hInfo.biHeight);
    HistogramStretching(Image,Histo,Output,hInfo.biWidth,hInfo.biHeight);
    HistogramEqualization(Image,Histo,AHisto,Output,hInfo.biWidth,hInfo.biHeight);
    Binarization(Image, Output, hInfo.biWidth, hInfo.biHeight, 80);
    freeer(Image, Output);
};

void sameImg(int ImgSize, BYTE* Image, BYTE* Output){
    for (int i = 0; i < ImgSize; i++) {
        Output[i] = Image[i];
    }
    outputPrinter("same",ImgSize, Image, Output);
};

void BrightnessAdj(BYTE*Img, BYTE*Out, int W, int H, int Val){
    int ImgSize = W*H;
    for (int i = 0; i < ImgSize; i++) {
        if(Img[i] + Val > 255){
            Out[i] = 255;
        }else if(Img[i] + Val < 0){
            Out[i] = 0;
        }else Out[i] = Img[i] + Val;
    };
    outputPrinter("brightness_adjusted", ImgSize, Img, Out);
};

void InverseImg (BYTE*Img, BYTE*Out, int W, int H){
    int ImgSize = W*H;
    for (int i = 0; i < ImgSize; i++) {
		Out[i] = 255 - Img[i];
	}
    outputPrinter("inverse", ImgSize, Img, Out);
};

void ContrastAdj(BYTE*Img, BYTE*Out, int W, int H, double Val){
    int ImgSize = W*H;
    for (int i = 0; i < ImgSize; i++) {
        // assume that val >= 0
        if(Img[i] * Val > 255.0){
            Out[i] = 255.0;
        }else Out[i] = (BYTE)Img[i] * Val;
    };
    outputPrinter("contrast_adjusted", ImgSize, Img, Out);
};

void ObtainHistogram (BYTE*Img,int*Histo,int W, int H){
    int ImgSize = W*H;
    for(int i = 0; i < ImgSize; i++){
        Histo[Img[i]]++;
    };
};

void HistogramStretching(BYTE*Img, int*Histo, BYTE*Out, int W, int H){
    int ImgSize = W*H;
    BYTE Low, High;

    for(int i = 0; i < 256; i++){
        if(Histo[i] != 0) {
            Low = i;
            break;
        };
    };

    for(int i = 255; i >=0; i--){
        if(Histo[i] != 0) {
            High = i;
            break;
        };
    };

    for(int i = 0; i < ImgSize; i++){
        Out[i] = (BYTE)(Img[i] - Low) / (double)(High - Low) * 255.0;
    };
    outputPrinter("histogram_stretched", ImgSize, Img, Out);
};

void ObtainAHisto(int* Histo, int* AHisto){
    for(int i = 1; i < 256; i++){
        AHisto[i] = AHisto[i-1] + Histo[i];
    };
};

void HistogramEqualization(BYTE*Img, int* Histo, int* AHisto, BYTE*Out, int W, int H){
    int ImgSize = W*H;
    int Nt = hInfo.biWidth*hInfo.biHeight, Gmax = 255;
    double Ratio = Gmax / (double)Nt;
    BYTE NormSum[256];
    for(int i=0; i < 256; i++){
        NormSum[i] = (BYTE)(Ratio * AHisto[i]);
    }
    for(int i=0; i< ImgSize; i++){
        Out[i] = NormSum[Img[i]];
    }
    outputPrinter("histogram_equalized", ImgSize, Img, Out);
};

void Binarization(BYTE*Img, BYTE*Out, int W, int H, BYTE Threshold){
    int ImgSize = W*H;
    for(int i = 0; i < ImgSize; i++){
        if(Img[i] < Threshold){
            Out[i] = 0;
        }else{
            Out[i] = 255;
        }
    }
    outputPrinter("binarized", ImgSize, Img, Out);
};

void outputPrinter(const char* filename, int ImgSize, BYTE* Image, BYTE* Output){
    char path[256];
    CreateDirectoryA("outputs", NULL);
    snprintf(path, sizeof(path),"outputs\\%s.bmp", filename);
    fp = fopen(path, "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);
};

void freeer (BYTE* Image, BYTE* Output){
    free(Image);
    free(Output);
};
