#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int main() {
	BITMAPFILEHEADER hf; //14 byte
	BITMAPINFOHEADER hInfo; // 40 byte
	RGBQUAD hRGB[256]; // 1024 byte

	char path[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, path);
	printf("%s\n", path);


	FILE* fp;
	fp = fopen("LENNA.bmp", "rb");
	if (fp == NULL) {
		printf("File not found! \n");
		return-1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER),1,fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight; //왜 동적할당하는가?
	BYTE* Image = (BYTE*)malloc(ImgSize); // void로 받는 이유, 왜 1바이트인가. 
		// BYTE Image(ImageSize) 배열처럼 쓸 수 있는 것. 다만 배열의 크기는 변수가 될 수 없기에 동적할당 하는 것이다.
	BYTE * Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	for (int i = 0; i < ImgSize; i++) {
		Output[i] = 255 - Image[i];
	}

	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);
	free(Image);
	free(Output);
}
