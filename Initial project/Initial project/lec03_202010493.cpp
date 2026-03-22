//202010493

#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void sameImg(int ImgSize, BYTE* Image, BYTE* Output);
void brighterImg(int ImgSize , BYTE* Image, BYTE* Output);
void negativeImg(int ImgSize , BYTE* Image, BYTE* Output);
void outputPrinter(const char* filename, int ImgSize, BYTE* Image, BYTE* Output);
void freeer (BYTE* Image, BYTE* Output);

FILE* fp;
BITMAPFILEHEADER hf; //14 byte
BITMAPINFOHEADER hInfo; // 40 byte
RGBQUAD hRGB[256]; // 1024 byte

int main() {
    fp = fopen("lenna.bmp", "rb");
	if(fp == NULL) return -1;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);

    int ImgSize = hInfo.biWidth * hInfo.biHeight; 
    BYTE * Image = (BYTE*)malloc(ImgSize); 
    BYTE * Output = (BYTE*)malloc(ImgSize);
    fread(Image, sizeof(BYTE), ImgSize, fp);

	sameImg(ImgSize, Image, Output);
    brighterImg(ImgSize, Image, Output);
	negativeImg(ImgSize, Image, Output);
    freeer(Image, Output);
};

void sameImg(int ImgSize, BYTE* Image, BYTE* Output){
    for (int i = 0; i < ImgSize; i++) {
        Output[i] = Image[i];
    }
    outputPrinter("output1",ImgSize, Image, Output);
};

void brighterImg(int ImgSize, BYTE* Image, BYTE* Output){
    for (int i = 0; i < ImgSize; i++) {
        Output[i] = 255 - Image[i] + 50;
    }
    
    outputPrinter("output2", ImgSize, Image, Output);
};

void negativeImg(int ImgSize, BYTE* Image, BYTE* Output){
    for (int i = 0; i < ImgSize; i++) {
		Output[i] = 255 - Image[i];
	}

    outputPrinter("output3",ImgSize, Image, Output);
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