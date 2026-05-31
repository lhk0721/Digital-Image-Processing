#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

// ===== Function Group Hoisting =====
// ----- Utilities ------
void SaveBMPFile(
    BITMAPFILEHEADER hf,
    BITMAPINFOHEADER hInfo,
    RGBQUAD hRGB,
    BYTE* Output,
    int W, int H,
    const char* Filename
);

void SameImage(
    BYTE* Image, BYTE* Output,
    int W, int H
);

void ResetOutput(
    BYTE* Output,
    int W, int H
);

// ----- Color Processing ------
void FillOnePixel(
    BYTE*Image, BYTE*Output,
    int X, int Y, // objective
    int W, int H,
    BYTE R, BYTE G, BYTE B
);

void FillHorizintalLine(
    BYTE*Image, BYTE*Output,
    int Y, // objective
    int W, int H,
    BYTE R, BYTE G, BYTE B
);

void FillRecktangle(
    BYTE*Image, BYTE*Output, 
    int Xs, int Ys, // objective
    int Xf, int Yf, // objective
    int W, int H,
    BYTE R, BYTE G, BYTE B
);

void FillRecktangleGradation(
    BYTE*Image, BYTE*Output, 
    int Xs, int Ys, // objective
    int Xf, int Yf, 
    int W, int H,
    BYTE Rs, BYTE Gs, BYTE Bs,
    BYTE Rf, BYTE Gf, BYTE Bf
)

int main(){
    // BMP Header Variables
    BITMAPFILEHEADER hf; // 14 Bytes
    BITMAPINFOHEADER hInfo; // 40 Bytes
    RGBQUAD hRGB[256];

    // Open Files
    FILE* fp;
    fp = fopen("fruit.bmp", "rb");
    if(fp == NULL){
        printf("File not found!\n");
        return -1;
    }

    // Read Headers
    fread(&hf, sizeof(BITMAPFILEHEADER),1,fp);
    fread(&hInfo, sizeof(BITMAPINFOHEADER),1,fp);

    // Define Size
    int W = hInfo.biWidth;
    int H = hInfo.biHeight;
    int ImgSize = W*H;

    // Allocate Memory
    BYTE* Image;
    BYTE* Output;
    if(hInfo.biBitCount == 24){
        // True Color
        Image = (BYTE*)malloc(ImgSize*3);
        Output = (BYTE*)malloc(ImgSize*3);
        fread(Image, sizeof(BYTE), ImgSize*3, fp);
    }else{
        // Gray Scale
        Image = (BYTE*)malloc(ImgSize);
        Output = (BYTE*)malloc(ImgSize);
        fread(Image, sizeof(BYTE), ImgSize, fp);
    }
    fclose(fp);

}

// ===== Function Group =====
// ----- Utilities ------
void SaveBMPFile(
    BITMAPFILEHEADER hf,
    BITMAPINFOHEADER hInfo,
    RGBQUAD hRGB,
    BYTE* Output,
    int W, int H,
    const char* Filename
){
    FILE* fp = fopen(Filename, "wb");
    if(hInfo.biBitCount == 24){
        fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
        fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
        fwrite(Output, sizeof(BYTE), W*H*3, fp);
    }else{
        fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
        fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
        fwrite(&hRGB, sizeof(RGBQUAD), 256, fp);
        fwrite(Output, sizeof(BYTE), W*H, fp);
    }
    fclose(fp);
}

void SameImage(
    BYTE* Image, BYTE* Output,
    int W, int H
){
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            Output[i*W*3 + j*3] = Image[i*W*3 + j*3];
            Output[i*W*3 + j*3 + 1] = Image[i*W*3 + j*3 + 1];
            Output[i*W*3 + j*3 + 2] = Image[i*W*3 + j*3 + 2];
        }
    }
}

void ResetOutput(
    BYTE* Output,
    int W, int H
){
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            Output[i*W*3 + j*3] = 0;
            Output[i*W*3 + j*3 + 1] = 0;
            Output[i*W*3 + j*3 + 2] = 0;
        }
    }
}

// ----- Color Processing ------
void FillOnePixel(
    BYTE*Image, BYTE*Output, 
    int X, int Y, // objective
    int W, int H,
    BYTE R, BYTE G, BYTE B
){
    SameImage(Image, Output, W,H);
    Output[Y*W*3 +X*3] = B;
    Output[Y*W*3 +X*3 + 1] = G;
    Output[Y*W*3 +X*3 + 2] = R;
}

void FillHorizintalLine(
    BYTE*Image, BYTE*Output,
    int Y, // objective
    int W, int H,
    BYTE R, BYTE G, BYTE B
){
    for(int i = 0; i < W; i++){
        FillOnePixel(Image,Output,i,Y,W,H,R,G,B);
    }
}

void FillRecktangle(
    BYTE*Image, BYTE*Output, 
    int Xs, int Ys, // objective
    int Xf, int Yf, // objective
    int W, int H,
    BYTE R, BYTE G, BYTE B
){
    for(int i = Ys; i <= Yf; i++){
        for(int j = Xs; j <= Xf; j++){
            FillOnePixel(Image, Output, j, i, W, H, R, G, B);
        }
    }
}

void FillRecktangleGradation(
    BYTE*Image, BYTE*Output, 
    int Xs, int Ys, // objective
    int Xf, int Yf, 
    int W, int H,
    BYTE Rs, BYTE Gs, BYTE Bs,
    BYTE Rf, BYTE Gf, BYTE Bf
){
    for(int i = Ys; i <= Yf; i++){
        for(int j = Xs; j <= Xf; j++){
            double wt = (j-Xs)/(double)(Xf-Xs);
            BYTE R = (BYTE)(Rs*(1-wt) + Rf*(wt));
            BYTE G = (BYTE)(Gs*(1-wt) + Gf*(wt));
            BYTE B = (BYTE)(Bs*(1-wt) + Bf*(wt));
            Output[i*W*3 + j*3 + 0] = B;
            Output[i*W*3 + j*3 + 1] = G;
            Output[i*W*3 + j*3 + 2] = R;
        }
    }
}