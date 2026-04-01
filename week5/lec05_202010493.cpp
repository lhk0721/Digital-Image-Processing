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
void freeer (BYTE* Image, BYTE* Output, BYTE* Output_temp, BYTE* Output_temp_2);
void ObtainHistogram (BYTE*Img,int*Histo,int W, int H);
void HistogramStretching(BYTE*Img, int* Histo, BYTE*Out, int W, int H);
void ObtainAHisto(int* Histo, int* AHisto);
void HistogramEqualization(BYTE*Img, int* Histo, int* AHisto, BYTE*Out, int W, int H);
void Binarization(BYTE*Img, BYTE*Out, int W, int H, BYTE Threshold);
int GonzalezBinThresh(BYTE*Img, BYTE*Out, int*Histo, int W, int H, int tolerance);
void GetMinMax(int*Histo, BYTE*Low, BYTE*High);
void AverageConv(BYTE*Img, BYTE*Out, int W, int H);
void GaussAvgConv(BYTE*Img, BYTE*Out, int W, int H);
void Prewitt_X_Conv(BYTE*Img, BYTE*Output_temp, int W, int H);
void Prewitt_Y_Conv(BYTE*Img, BYTE*Output_temp, int W, int H);
void Sobel_X_Conv(BYTE*Img, BYTE*Output_temp, int W, int H);
void Sobel_Y_Conv(BYTE*Img, BYTE*Output_temp, int W, int H);
void Laplace_Conv(BYTE*Img, BYTE*Output_temp, int W, int H);
void Laplace_Conv_DC(BYTE*Img, BYTE*Out, int W, int H);

int main() {
    fp = fopen("LENNA.bmp", "rb");
	if(fp == NULL) return -1;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);

    int ImgSize = hInfo.biWidth * hInfo.biHeight;

    BYTE * Image = (BYTE*)malloc(ImgSize);
    fread(Image, sizeof(BYTE), ImgSize, fp);
    BYTE * Output = (BYTE*)malloc(ImgSize);
    BYTE * Output_temp = (BYTE*)malloc(ImgSize);
    BYTE * Output_temp_2 = (BYTE*)malloc(ImgSize);
    
    int Histo[256] = {0};
    int AHisto[256] = {0};
    ObtainHistogram(Image,Histo,hInfo.biWidth,hInfo.biHeight);
    ObtainAHisto(Histo,AHisto);

    int Thres = GonzalezBinThresh(Image,Output,Histo,hInfo.biWidth,hInfo.biHeight,2);
    
    if (Thres == -1) {
        printf("Error: threshold calculation failed\n");
        freeer(Image, Output,Output_temp,Output_temp_2);
        fclose(fp);
        return -1;
    }
    

	// sameImg(ImgSize, Image, Output);
    // BrightnessAdj(Image,Output,hInfo.biWidth,hInfo.biHeight,70);
    // ContrastAdj(Image,Output,hInfo.biWidth,hInfo.biHeight,1.5);
    // InverseImg (Image, Output, hInfo.biWidth, hInfo.biHeight);
    // HistogramStretching(Image,Histo,Output,hInfo.biWidth,hInfo.biHeight);
    // HistogramEqualization(Image,Histo,AHisto,Output,hInfo.biWidth,hInfo.biHeight);
    // Binarization(Image, Output, hInfo.biWidth, hInfo.biHeight, Thres);
    // AverageConv(Image, Output,hInfo.biWidth,hInfo.biHeight);
    // GaussAvgConv(Image, Output,hInfo.biWidth,hInfo.biHeight);

    // Prewitt_X_Conv(Image, Output_temp,hInfo.biWidth,hInfo.biHeight);
    // Binarization(Output_temp, Output_temp, hInfo.biWidth, hInfo.biHeight, 50);
    // Prewitt_Y_Conv(Image, Output_temp_2,hInfo.biWidth,hInfo.biHeight);
    // Binarization(Output_temp_2, Output_temp_2, hInfo.biWidth, hInfo.biHeight, 51);
    // for(int i =0; i < ImgSize; i++){
    //     if(Output_temp[i] > Output_temp_2[i]) Output_temp_2[i] = Output_temp[i];
    // };
    // Binarization(Output_temp_2, Output_temp_2, hInfo.biWidth, hInfo.biHeight, 52);
    
    // Sobel_X_Conv(Image, Output_temp,hInfo.biWidth,hInfo.biHeight);
    // Binarization(Output_temp, Output_temp, hInfo.biWidth, hInfo.biHeight, 53);
    // Sobel_Y_Conv(Image, Output_temp_2,hInfo.biWidth,hInfo.biHeight);
    // Binarization(Output_temp_2, Output_temp_2, hInfo.biWidth, hInfo.biHeight, 54);
    // for(int i =0; i < ImgSize; i++){
    //     if(Output_temp[i] > Output_temp_2[i]) Output_temp_2[i] = Output_temp[i];
    // };
    // Binarization(Output_temp_2, Output_temp_2, hInfo.biWidth, hInfo.biHeight, 55);
    // Laplace_Conv(Image, Output, hInfo.biWidth,hInfo.biHeight);
    GaussAvgConv(Image, Output_temp,hInfo.biWidth,hInfo.biHeight);
    Laplace_Conv_DC(Output_temp, Output, hInfo.biWidth,hInfo.biHeight);
    
    freeer(Image, Output,Output_temp,Output_temp_2);
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

    GetMinMax(Histo, &Low, &High);

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
    char filename[200];
    for(int i = 0; i < ImgSize; i++){
        if(Img[i] < Threshold){
            Out[i] = 0;
        }else{
            Out[i] = 255;
        }
    }
    snprintf(filename, sizeof(filename), "binarized(Thresh_%u)", Threshold);
    outputPrinter(filename, ImgSize, Img, Out);
};

int GonzalezBinThresh(BYTE*Img, BYTE*Out, int*Histo, int W, int H, int tolerance){
    int ImgSize = W * H;
    BYTE Low=0, High=0;

    GetMinMax(Histo, &Low, &High);
    int T1 = (Low + High)/2;
    int T2;

    while(true){
        int ALow = 0, AHigh = 0;
        int LowCount = 0, HighCount = 0;
        int m1, m2;
    
        for (int i = Low; i <= T1; i ++){
            if(Histo[i] != 0){
                ALow = ALow + i*Histo[i];
                LowCount = LowCount + Histo[i];
            }
        }

        for (int i = T1 + 1; i <= High; i ++){
            if(Histo[i] != 0){
                AHigh = AHigh + i*Histo[i];
                HighCount = HighCount + Histo[i];
            }
        }

        if(LowCount == 0 || HighCount == 0){
            return -1;
        }
        m1 = ALow / LowCount;
        m2 = AHigh / HighCount;
        T2 = (m1 + m2)/2;

        if(abs(T2 - T1) < tolerance){
            break;
        }else{
            T1 = T2;
        };
    };
    return T1;
};

void GetMinMax(int*Histo, BYTE*Low, BYTE*High){
    for (int i = 0; i <= 255; i++){
        if(Histo[i] != 0){
            *Low = i;
            break;
        };
    };
    for (int i = 255; i >= 0; i--){
        if(Histo[i] != 0){
            *High = i;
            break;
        };
    };
};

void AverageConv(BYTE*Img, BYTE*Out, int W, int H){
    int ImgSize = W * H;
    double Kernel[3][3] = {
        0.11111,0.11111,0.11111,
        0.11111,0.11111,0.11111,
        0.11111,0.11111,0.11111
    };
    // 마진(테두리) 픽셀은 원본값 복사
    // for(int j = 0; j < W; j++) { Out[j] = Img[j]; Out[(H-1)*W+j] = Img[(H-1)*W+j]; }
    // for(int i = 0; i < H; i++) { Out[i*W] = Img[i*W]; Out[i*W+(W-1)] = Img[i*W+(W-1)]; }

    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){ //Y좌표(행)
        for(int j = 1; j < W-1 ; j++){  // x좌표 (열)
            for(int m = -1; m <= 1; m ++){ // Kernel 행
                for(int n = -1; n<= 1; n ++){ // Kernel 열
                    SumProduct += Img[(i+m)*W+(j+n)] * Kernel[m+1][n+1];
                }
            }
            Out[i*W + j] = (BYTE)SumProduct;
            SumProduct = 0.0;
        };
    };
    outputPrinter("AverageConvolutionalized", ImgSize, Img, Out);
};

void GaussAvgConv(BYTE*Img, BYTE*Out, int W, int H){ // 가우시안 평활화
    int ImgSize = W * H;
    double Kernel[3][3] = { 
        0.0625,0.125,0.0625,
        0.125,0.25,0.125,
        0.0625,0.125,0.0625
    };
    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){ //Y좌표(행)
        for(int j = 1; j < W-1 ; j++){  // x좌표 (열)
            for(int m = -1; m <= 1; m ++){ // Kernel 행
                for(int n = -1; n<= 1; n ++){ // Kernel 열
                    SumProduct += Img[(i+m)*W+(j+n)] * Kernel[m+1][n+1];
                }
            }
            Out[i*W + j] = SumProduct;
            SumProduct = 0.0;
        };
    };
    outputPrinter("GaussAvgConvolutionalized", ImgSize, Img, Out);
};

void Prewitt_X_Conv(BYTE*Img, BYTE*Output_temp, int W, int H){ 
    int ImgSize = W * H;
    double Kernel[3][3] = { 
        -1.0,0.0,1.0,
        -1.0,0.0,1.0,
        -1.0,0.0,1.0
    };
    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){ //Y좌표(행)
        for(int j = 1; j < W-1 ; j++){  // x좌표 (열)
            for(int m = -1; m <= 1; m ++){ // Kernel 행
                for(int n = -1; n<= 1; n ++){ // Kernel 열
                    SumProduct += Img[(i+m)*W+(j+n)] * Kernel[m+1][n+1];
                }
            }
            //0~765 => 0~255
            Output_temp[i*W + j] = abs((long)SumProduct)/3;
            SumProduct = 0.0;
        };
    };
    outputPrinter("Prewitt_X_Convolutionalized", ImgSize, Img, Output_temp);
};

void Prewitt_Y_Conv(BYTE*Img, BYTE*Output_temp, int W, int H){ 
    int ImgSize = W * H;
    double Kernel[3][3] = { 
        -1.0,-1.0,-1.0,
        0.0,0.0,0.0,
        1.0,1.0,1.0
    };
    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){ //Y좌표(행)
        for(int j = 1; j < W-1 ; j++){  // x좌표 (열)
            for(int m = -1; m <= 1; m ++){ // Kernel 행
                for(int n = -1; n<= 1; n ++){ // Kernel 열
                    SumProduct += Img[(i+m)*W+(j+n)] * Kernel[m+1][n+1];
                }
            }
            //0~765 => 0~255
            Output_temp[i*W + j] = abs((long)SumProduct)/3;
            SumProduct = 0.0;
        };
    };
    outputPrinter("Prewitt_Y_Convolutionalized", ImgSize, Img, Output_temp);
};
void Sobel_X_Conv(BYTE*Img, BYTE*Output_temp, int W, int H){ 
    int ImgSize = W * H;
    double Kernel[3][3] = { 
        -1.0,0.0,1.0,
        -2.0,0.0,2.0,
        -1.0,0.0,1.0
    };
    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){ //Y좌표(행)
        for(int j = 1; j < W-1 ; j++){  // x좌표 (열)
            for(int m = -1; m <= 1; m ++){ // Kernel 행
                for(int n = -1; n<= 1; n ++){ // Kernel 열
                    SumProduct += Img[(i+m)*W+(j+n)] * Kernel[m+1][n+1];
                }
            }
            //0~1020 => 0~255
            Output_temp[i*W + j] = abs((long)SumProduct)/4;
            SumProduct = 0.0;
        };
    };
    outputPrinter("Sobel_X_Convolutionalized", ImgSize, Img, Output_temp);
};

void Sobel_Y_Conv(BYTE*Img, BYTE*Output_temp, int W, int H){ 
    int ImgSize = W * H;
    double Kernel[3][3] = { 
        -1.0,-2.0,-1.0,
        0.0,0.0,0.0,
        1.0,2.0,1.0
    };
    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){ //Y좌표(행)
        for(int j = 1; j < W-1 ; j++){  // x좌표 (열)
            for(int m = -1; m <= 1; m ++){ // Kernel 행
                for(int n = -1; n<= 1; n ++){ // Kernel 열
                    SumProduct += Img[(i+m)*W+(j+n)] * Kernel[m+1][n+1];
                }
            }
            //0~1020 => 0~255
            Output_temp[i*W + j] = abs((long)SumProduct)/4;
            SumProduct = 0.0;
        };
    };
    outputPrinter("Sobel_Convolutionalized", ImgSize, Img, Output_temp);
};

void Laplace_Conv(BYTE*Img, BYTE*Out, int W, int H){ 
    int ImgSize = W * H;
    double Kernel[3][3] = { 
        -1.0,-1.0,-1.0,
        -1.0,8.0,-1.0,
        -1.0,-1.0,-1.0
    };
    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){ //Y좌표(행)
        for(int j = 1; j < W-1 ; j++){  // x좌표 (열)
            for(int m = -1; m <= 1; m ++){ // Kernel 행
                for(int n = -1; n<= 1; n ++){ // Kernel 열
                    SumProduct += Img[(i+m)*W+(j+n)] * Kernel[m+1][n+1];
                }
            }
            // 0~2040 => 0~255
            Out[i*W + j] = abs((long)SumProduct)/8;
            SumProduct = 0.0;
        };
    };
    outputPrinter("Laplace_Convolutionalized", ImgSize, Img, Out);
};
void Laplace_Conv_DC(BYTE*Img, BYTE*Out, int W, int H){ 
    int ImgSize = W * H;
    double Kernel[3][3] = { 
        -1.0,-1.0,-1.0,
        -1.0,9.0,-1.0,
        -1.0,-1.0,-1.0
    };
    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){ //Y좌표(행)
        for(int j = 1; j < W-1 ; j++){  // x좌표 (열)
            for(int m = -1; m <= 1; m ++){ // Kernel 행
                for(int n = -1; n<= 1; n ++){ // Kernel 열
                    SumProduct += Img[(i+m)*W+(j+n)] * Kernel[m+1][n+1];
                }
            }
            // Out[i*W + j] = abs((long)SumProduct)/8;
            if(SumProduct > 255.0) Out[i*W + j] = 255;
            else if(SumProduct < 0.0) Out[i*W + j] = 0;
            else Out[i*W + j] = (BYTE)SumProduct;
            SumProduct = 0.0;
        };
    };
    outputPrinter("Laplace_DC_Convolutionalized", ImgSize, Img, Out);
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

void freeer (
    BYTE* Image, 
    BYTE* Output,
    BYTE* Output_temp,
    BYTE* Output_temp_2
){
    free(Image);
    free(Output);
    free(Output_temp);
    free(Output_temp_2);
};
