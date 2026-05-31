#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>
BYTE temp[9];

// ===== Image Quality Adjustment ======
void SameImage(BYTE* Img, BYTE* Out, int W, int H);
void InverseImage(BYTE* Img, BYTE* Out, int W, int H);
void BrightnessAdj(BYTE* Img, BYTE* Out, int W, int H, int Val);
void ContrastAdj(BYTE* Img, BYTE* Out, int W, int H, double Val);

// ====== Histogram ======
void ObtainHistogram(BYTE* Img, int* Histo, int W, int H);
void ObtainAHistogram(int* Histo, int* AHisto);
void HistogramEqualization(BYTE* Img, BYTE* Out, int* AHisto, int W, int H);
void HistogramStretching(BYTE* Img, BYTE* Out, int* Histo, int W, int H);

// ====== Binarization ======
int GonzalezBinThresh(int* Histo, int W, int H);
void Binarization(BYTE* Img, BYTE* Out, int W, int H, BYTE Threshold);

// ====== Convolution Filters ======
void AverageConv(BYTE* Img, BYTE* Out, int W, int H);
void GaussAvrConv(BYTE* Img, BYTE* Out, int W, int H);
void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H);
void Prewitt_Y_Conv(BYTE* Img, BYTE* Out, int W, int H);
void Sobel_X_Conv(BYTE* Img, BYTE* Out, int W, int H);
void Sobel_Y_Conv(BYTE* Img, BYTE* Out, int W, int H);
void Laplace_Conv(BYTE* Img, BYTE* Out, int W, int H);
void Laplace_Conv_DC(BYTE* Img, BYTE* Out, int W, int H);

// ====== Utilities ======
void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo, RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName);
void swap(BYTE* a, BYTE* b);
BYTE Median(BYTE* arr, int size);
BYTE MaxPooling(BYTE* arr, int size);
BYTE MinPooling(BYTE* arr, int size);

// ====== Labeling ======
int push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top);
int pop(short* stackx, short* stacky, short* vx, short* vy, int* top);
void m_BlobColoring(BYTE* CutImage, int height, int width);
void BinaryImageEdgeDetection(BYTE* Bin, BYTE* Out, int W, int H);

// ===== Geometric Transformation =====
void VerticalFlip(BYTE* img, int W, int H);
void HorizontalFlip(BYTE* img, int W, int H);
void Translation(BYTE*Image, BYTE*Output,int W, int H, int Tx, int Ty);
void Scaling(BYTE*Image, BYTE*Output,int W, int H, double SF_X, double SF_Y);
void Rotation(BYTE*Image, BYTE*Output,int W, int H, int Angle);

// ===== Color processing =====
void FillColor(BYTE*Image, int X, int Y, int W, int H, BYTE R, BYTE G, BYTE B);

// ====== MAIN ======
int main(){
    // BMP Header Variables
    BITMAPFILEHEADER hf;
    BITMAPINFOHEADER hInfo;
    RGBQUAD hRGB[256];

    // ===== Open File =====
    FILE* fp;
    fp = fopen("tcsample.bmp", "rb");
    if(fp == NULL){
        printf("File not found!\n");
        return -1;
    };

    // ===== Read Headers ===== 
    fread(&hf,sizeof(BITMAPFILEHEADER),1,fp);
    fread(&hInfo, sizeof(BITMAPINFOHEADER),1,fp);
    int W = hInfo.biWidth, H = hInfo.biHeight;
    int ImgSize = W * H;
    BYTE * Image;
    BYTE * Output;

    if(hInfo.biBitCount == 24){ // true color
        // ===== Allocate Memory =====
        Image = (BYTE *)malloc(ImgSize*3);
        Output = (BYTE *)malloc(ImgSize*3);
        // ===== Read Image Data =====
        fread(Image, sizeof(BYTE), ImgSize*3, fp);

    }else{ //index(gray)
        fread(&hRGB,sizeof(RGBQUAD),256,fp);
        // ===== Allocate Memory =====
        Image = (BYTE *)malloc(ImgSize);
        Output = (BYTE *)malloc(ImgSize);
        // ===== Read Image Data =====
        fread(Image, sizeof(BYTE), ImgSize, fp);
    }

    // ===== Histogram =====
    /*
    int Histo[256] = {0};
    int AHisto[256] = {0};
    ObtainHistogram(Image, Histo, W, H);
    ObtainAHistogram(Histo, AHisto);
    */ 

    fclose(fp);

    // ===== Processing =====
    // ----- Image Quality Adjustment -----
    SameImage(Image,Output,W,H);
    /*
    InverseImage(Image,Output,W,H);
    BrightnessAdj(Image, Output,W,H,-50);
    ContrastAdj(Image, Output,W,H,1.5);
    */ 

    // ----- Histogram Processing -----
    /*
    HistogramEqualization(Image, Output, AHisto, W, H);
    HistogramStretching(Image,Output, Histo,W,H);
    */ 

    // ----- Binarization -----
    /*
    int thresh = GonzalezBinThresh(Histo,W,H);
    Binarization(Image, Output, W,H,thresh);
    */ 

    // ----- Convolusion Filtering -----
    /*
    AverageConv(Image, Output, W,H);
    */ 

    // ----- median filtering -----
    /*
    int Length = 9;
    int Margin = Length / 2;
    int Wsize = Length * Length;;
    BYTE*temp = (BYTE*)malloc(sizeof(BYTE)*Wsize);
    int i,j,m,n;
    for (i = Margin; i < H - Margin; i++){
        for (j = Margin; j < W - Margin; j++){
            for (m = -Margin; m <= H-Margin; m++){
                for (n = -Margin; n <= H-Margin; n++){
                    temp[(m + Margin)*Length + (n + Margin)] = Image[(i + m)*W + j+n];
                }
            }
            Output[i*W + j] = Median(temp, Wsize);
        }
    }
    free(temp);
    */

    // ----- Geometric Transformation -----
    // VerticalFlip(Image, W, H);
    // HorizontalFlip(Image, W,H);
    // Translation(Image, Output, W,H, 100, 40);
    // Scaling(Image, Output, W, H, 2.0, 0.7);
    // Rotation(Image, Output, W, H, 60);


    // ----- Color processing -----
    
    // one pixel
    // FillColor(Output,50,50,W,H,255,0,0);
    
    // line
    // for(int i = 0; i<W; i++){
    //     FillColor(Output,i,200,W,H,255,0,0);
    // }

    // Reck
    for(int i = 100; i<= 400; i++){
        for(int j=50;j<=300;j++){
            FillColor(Output,j,i,W,H,255,0,255);
        }
    }

    // Reset
    for(int i=0; i<H; i++){
        for(int j=0; j<W; j++){
            Image[i*W*3 + j*3] = 0;
            Image[i*W*3 + j*3 + 1] = 0;
            Image[i*W*3 + j*3 + 2] = 0;
        }
    }
    SameImage(Image,Output,W,H);

    for(int i=0; i<240; i++){ //R
        for(int j=0; j<W; j++){
            Output[i*W*3 + j*3 + 2]=255;
        }
    }
    for(int i=120; i<360; i++){ //G
        for(int j=0; j<W; j++){
            Output[i*W*3 + j*3 + 1]=255;
        }
    }
    for(int i=240; i<H; i++){ //B
        for(int j=0; j<W; j++){
            Output[i*W*3 + j*3]=255;
        }
    }

    // Reset
    for(int i=0; i<H; i++){
        for(int j=0; j<W; j++){
            Image[i*W*3 + j*3] = 0;
            Image[i*W*3 + j*3 + 1] = 0;
            Image[i*W*3 + j*3 + 2] = 0;
        }
    }
    SameImage(Image,Output,W,H);

    // gradation
    double wt;
    for(int i = 0; i < 120; i++){
        for(int j=0; j<W; j++){
            wt = j/(double)(W-1);
            Output[i*W*3 + j*3]= (BYTE)(255*(1-wt)); //B
            Output[i*W*3 + j*3 + 1]= (BYTE)(255*(wt)); //G
            Output[i*W*3 + j*3 + 2]= (BYTE)(255*(wt)); //R
        }
    }
    
    for(int i = 121; i < 240; i++){
        for(int j=0; j<W; j++){
            wt = j/(double)(W-1);
            Output[i*W*3 + j*3]= (BYTE)(255*(wt)); //B
            Output[i*W*3 + j*3 + 1]= (BYTE)(255*(1-wt)); //G
            Output[i*W*3 + j*3 + 2]= (BYTE)(255*(wt)); //R
        }
    }

    for(int i = 241; i < 360; i++){
        for(int j=0; j<W; j++){
            wt = j/(double)(W-1);
            Output[i*W*3 + j*3]= (BYTE)(255*(wt)); //B
            Output[i*W*3 + j*3 + 1]= (BYTE)(255*(wt)); //G
            Output[i*W*3 + j*3 + 2]= (BYTE)(255*(1-wt)); //R
        }
    }

    // ===== Save Output =====
    
    SaveBMPFile(hf,hInfo,hRGB,Output,W,H,"output_tcsample.bmp");

    // ===== Free Memory =====
    free(Image);
    free(Output);
    return 0;
};

void SaveBMPFile(
    BITMAPFILEHEADER hf,
    BITMAPINFOHEADER hInfo,
    RGBQUAD* hRGB,
    BYTE* Output,
    int W, int H,
    const char* FileName){
    // Open File
    FILE * fp = fopen(FileName, "wb");
    if(hInfo.biBitCount == 24){
        // Write Headers
        fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
        fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);

        // Write Image Data
        fwrite(Output, sizeof(BYTE), W*H*3, fp);

    }else{
        // Write Headers
        fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
        fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
        fwrite(hRGB, sizeof(RGBQUAD), 256, fp);

        // Write Image Data
        fwrite(Output, sizeof(BYTE), W*H, fp);
    }

    // Close File
    fclose(fp);
}

/*
void swap(BYTE* a, BYTE* b){
    BYTE temp = *a;
    *a = *b;
    *b = temp;
};
*/ 

// ===== Image Quality Adjustment ======
void SameImage(BYTE* Img, BYTE* Out, int W, int H){
    int ImgSize = W*H*3;
    for(int i = 0; i < ImgSize; i++){
        Out[i] = Img[i];
    }
};

/*
void InverseImage(BYTE* Img, BYTE* Out, int W, int H){
    int ImgSize = W*H;
    for(int i = 0; i < ImgSize; i++){
        if(Img[i] > 255){
            Img[i] = 0;
        }else{
            Out[i] = 255 - Img[i];
        };
    };
};

void BrightnessAdj(BYTE* Img, BYTE* Out, int W, int H, int Val){
    int ImgSize = W*H;
    for(int i = 0; i < ImgSize; i++){
        if(Img[i] + Val > 255){
            Out[i] = 255;
        }else if(Img[i] + Val < 0){
            Out[i] = 0;
        }else{
            Out[i] = Img[i] + Val;
        };
    };
};

void ContrastAdj(BYTE* Img, BYTE* Out, int W, int H, double Val){
    int ImgSize = W*H;
    for(int i = 0; i < ImgSize; i++){
        if(Img[i] * Val > 255){
            Out[i] = 255;
        }else{
            Out[i] = (BYTE)(Img[i] * Val);
        };
    };
};

*/

// ====== Histogram ======
/*
void ObtainHistogram(BYTE* Img, int* Histo, int W, int H){
    int ImgSize = W*H;
    for (int i = 0; i < ImgSize; i++){
        Histo[Img[i]]++;
    };
};

void ObtainAHistogram(int* Histo, int* AHisto){
    for(int i = 0; i < 256; i++){
        for(int j = 0; j <= i; j++){
            AHisto[i] += Histo[j];
        };
    };
};

void HistogramEqualization(BYTE* Img, BYTE* Out, int* AHisto, int W, int H){
    int ImgSize = W*H;
    int Nt = W*H, Gmax = 255;
    double ratio = Gmax / (double)Nt;

    BYTE NormSum[256];
    for(int i = 0; i < 256; i++){
        NormSum[i] = (BYTE)(ratio * AHisto[i]);
    };

    for(int i = 0; i < ImgSize; i++){
        Out[i] = NormSum[Img[i]];
    };
};

void HistogramStretching(BYTE* Img, BYTE* Out, int* Histo, int W, int H){
    int ImgSize = W*H;
    BYTE min,max;
    for (int i = 0; i < 256; i++){
        if(Histo[i] != 0) { min = i; break; }
    };
    for (int i = 255; i >= 0; i--){
        if(Histo[i] != 0) { max = i; break; }
    };

    for(int i = 0; i < ImgSize; i++){
        Out[i] = (BYTE)((Img[i] - min)/(double)(max-min) * 255.0);
    }
};
*/

// ===== Binarization =====
/*
void Binarization(BYTE* Img, BYTE* Out, int W, int H, BYTE Threshold){
    int ImgSize = W*H;
    for (int i = 0; i < ImgSize; i++){
        if(Img[i] <= Threshold){
            Out[i] = 0;
        }else{
            Out[i] = 255;
        };
    };
};

int GonzalezBinThresh(int* Histo, int W, int H){
    int min = 0;
    int max = 255;
    int threshold = (min + max)/2;
    int newThreshold = -1;
    
    while (true) {
        long long sum1 = 0, sum2 = 0;
        int m1, m2, count1=0, count2=0;

        for(int i=min; i<=threshold; i++){
            sum1 += long long(Histo[i]) * i;
            count1 += Histo[i];
        }

        for(int i=max; i > threshold; i--){
            sum2 += long long(Histo[i]) * i;
            count2 += Histo[i];
        }

        m1 = sum1 / count1;
        m2 = sum2 / count2;
        newThreshold = (m1 + m2)/2;

        if(threshold == newThreshold){
            break;
        }else{
            threshold = newThreshold;
        };
    }

    return threshold;
}

*/ 

// ===== Convolution Filters =====
/*
void AverageConv(BYTE* Img, BYTE* Out, int W, int H){
    double Kernel[3][3] = {
        {1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0}
    };
    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){
        for (int j = 1; j < W-1; j++){
            for (int m = -1; m <= 1;  m++){
                for (int n = -1; n <=1; n++){
                    SumProduct += Img[(i+m)*W + (j + n)] * Kernel[m+1][n+1];
                };
            };
            Out[i*W + j] = (BYTE)(SumProduct/9);
            SumProduct = 0.0;
        };
    };
};

void GausAvrConv(BYTE* Img, BYTE* Out, int W, int H){
    double Kernel[3][3] = {
        {1.0, 2.0, 1.0},
        {2.0, 4.0, 2.0},
        {1.0, 2.0, 1.0}
    };
    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){
        for (int j = 1; j < W-1; j++){
            for (int m = -1; m <= 1;  m++){
                for (int n = -1; n <=1; n++){
                    SumProduct += Img[(i+m)*W + (j + n)] * Kernel[m+1][n+1];
                };
            };
            Out[i*W + j] = (BYTE)(SumProduct/16);
            SumProduct = 0.0;
        };
    };
};

void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H){
    double Kernel[3][3] = {
        {-1.0, 0.0, 1.0},
        {-1.0, 0.0, 1.0},
        {-1.0, 0.0, 1.0}
    };
    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){
        for (int j = 1; j < W-1; j++){
            for (int m = -1; m <= 1;  m++){
                for (int n = -1; n <=1; n++){
                    SumProduct += Img[(i+m)*W + (j + n)] * Kernel[m+1][n+1];
                };
            };
            Out[i*W + j] = abs((long)(SumProduct)) / 3;
            SumProduct = 0.0;
        };
    };
};

void Prewitt_Y_Conv(BYTE* Img, BYTE* Out, int W, int H){
    double Kernel[3][3] = {
        {-1.0, -1.0, -1.0},
        {0.0, 0.0, 0.0},
        {1.0, 1.0, 1.0}
    };
    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){
        for (int j = 1; j < W-1; j++){
            for (int m = -1; m <= 1;  m++){
                for (int n = -1; n <=1; n++){
                    SumProduct += Img[(i+m)*W + (j + n)] * Kernel[m+1][n+1];
                };
            };
            Out[i*W + j] = abs((long)(SumProduct)) / 3;
            SumProduct = 0.0;
        };
    };
};

void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H){
    double Kernel[3][3] = {
        {-1.0, 0.0, 1.0},
        {-2.0, 0.0, 2.0},
        {-1.0, 0.0, 1.0}
    };
    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){
        for (int j = 1; j < W-1; j++){
            for (int m = -1; m <= 1;  m++){
                for (int n = -1; n <=1; n++){
                    SumProduct += Img[(i+m)*W + (j + n)] * Kernel[m+1][n+1];
                };
            };
            Out[i*W + j] = abs((long)(SumProduct)) / 4;
            SumProduct = 0.0;
        };
    };
};

void Prewitt_Y_Conv(BYTE* Img, BYTE* Out, int W, int H){
    double Kernel[3][3] = {
        {-1.0, -1.0, -1.0},
        {-1.0, 9.0, -1.0},
        {-1.0, -1.0, -1.0}
        
    };
    double SumProduct = 0.0;
    for(int i = 1; i < H-1; i++){
        for (int j = 1; j < W-1; j++){
            for (int m = -1; m <= 1;  m++){
                for (int n = -1; n <=1; n++){
                    SumProduct += Img[(i+m)*W + (j + n)] * Kernel[m+1][n+1];
                };
            };
            // Out[i*W + j] = abs((long)(SumProduct)) / 8;
            if(SumProduct > 255){
                Out[i*W + j] = 255;
            }else if(SumProduct < 0){
                Out[i*W + j] = 0;
            }else{
                Out[i*W + j] = (BYTE)SumProduct;
            };
            SumProduct = 0.0;
        };
    };
};

BYTE Median(BYTE* arr, int size){
    const int S = size;
    for(int i = 0; i < size-1; i++){
        for(int j = 0; j < size; j++){
            if(arr[j] > arr[i]){
                swap(&arr[i],&arr[j]);
            }
        }
    }
    return arr[S/2];
};

BYTE MaxPooling(BYTE* arr, int size){
    const int S = size;
    for(int i = 0; i < size-1; i++){
        for(int j = 0; j < size; j++){
            if(arr[j] > arr[i]){
                swap(&arr[i],&arr[j]);
            }
        }
    }
    return arr[S-1];
};

BYTE MinPooling(BYTE* arr, int size){
    const int S = size;
    for(int i = 0; i < size-1; i++){
        for(int j = 0; j < size; j++){
            if(arr[j] > arr[i]){
                swap(&arr[i],&arr[j]);
            }
        }
    }
    return arr[S-1];
};
*/ 

// ===== labelling =====
/*
int push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top)
{
	if (*top >= arr_size) return(-1);
	(*top)++;
	stackx[*top] = vx;
	stacky[*top] = vy;
	return(1);
}

int pop(short* stackx, short* stacky, short* vx, short* vy, int* top)
{
	if (*top == 0) return(-1);
	*vx = stackx[*top];
	*vy = stacky[*top];
	(*top)--;
	return(1);
}

// labelling code using GlassFire algorythm
void m_BlobColoring(BYTE* CutImage, int height, int width)
{
	int i, j, m, n, top, area, Out_Area, index, BlobArea[1000];
	long k;
	short curColor = 0, r, c;
	Out_Area = 1;

	short* stackx = new short[height * width];
	short* stacky = new short[height * width];
	short* coloring = new short[height * width];

	int arr_size = height * width;

	for (k = 0; k < height * width; k++) coloring[k] = 0;

	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			if (coloring[index + j] != 0 || CutImage[index + j] != 255) continue;
			r = i; c = j; top = 0; area = 1;
			curColor++;

			while (1)
			{
			GRASSFIRE:
				for (m = r - 1; m <= r + 1; m++)
				{
					index = m * width;
					for (n = c - 1; n <= c + 1; n++)
					{
						if (m < 0 || m >= height || n < 0 || n >= width) continue;

						if ((int)CutImage[index + n] == 255 && coloring[index + n] == 0)
						{
							coloring[index + n] = curColor;
							if (push(stackx, stacky, arr_size, (short)m, (short)n, &top) == -1) continue;
							r = m; c = n; area++;
							goto GRASSFIRE;
						}
					}
				}
				if (pop(stackx, stacky, &r, &c, &top) == -1) break;
			}
			if (curColor < 1000) BlobArea[curColor] = area;
		}
	}

	float grayGap = 255.0f / (float)curColor;

	for (i = 1; i <= curColor; i++)
	{
		if (BlobArea[i] >= BlobArea[Out_Area]) Out_Area = i;
	}
	for (k = 0; k < width * height; k++) CutImage[k] = 255;

	for (k = 0; k < width * height; k++)
	{
		if (coloring[k] == Out_Area) CutImage[k] = 0; //show biggest only (size filtering)
        // CutImage[k] = (unsigned char)(coloring[k] * grayGap);
	}

	delete[] coloring;
	delete[] stackx;
	delete[] stacky;
}

void BinaryImageEdgeDetection(BYTE* Bin, BYTE*Out, int W, int H){
    
    for(int i=0; i<H; i++){
        for(int j = 0; j < W; j++){
            if(Bin[i*W + j] == 0){
                if(!(
                    Bin[(i-1)*W + j] == 0 && 
                    Bin[(i+1)*W + j] == 0 && 
                    Bin[i*W + (j-1)] == 0 && 
                    Bin[i*W + (j+1)] == 0
                ))Out[i*W + j] = 255;
            };
        }
    }
}

*/ 

// ===== Geometric Transformation =====
/*


// ----- flip -----
void VerticalFlip(BYTE* img, int W, int H){
    for(int i = 0; i < H/2; i++){
        for(int j = 0; j < W; j++){
            swap(&img[i*W + j], &img[(H-1-i)*W + j]);
        }
    }
};

void HorizontalFlip(BYTE* img, int W, int H){
    for(int i = 0; i < W/2; i++){
        for(int j = 0; j < H; j++){
            swap(&img[j*W + i], &img[j*W + (W-1-i)]);
        }
    }
};

// ----- Translation -----
void Translation(BYTE*Image, BYTE*Output,int W, int H, int Tx, int Ty){
    Ty *= -1;
    for(int i=0; i<H; i++){
        for(int j=0; j <W; j++){
            if(((i + Ty < H) && (i + Ty >= 0)) && ((j + Tx < W) && (j + Tx >=0))){
                Output[(i + Ty)*W + (j+Tx)] = Image[i*W + j];
            }
        }
    }
}

// ----- Scaling -----
void Scaling(BYTE*Image, BYTE*Output,int W, int H, double SF_X, double SF_Y){
    int tmpX,tmpY;
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            tmpX = (int)(j/SF_X);
            tmpY = (int)(i/SF_Y);
            if((tmpY < H) && (tmpX < W)){
                Output[i*W + j] = Image[tmpY*W + tmpX];
            }
        }
    }
}

// ----- Rotation -----
void Rotation(BYTE*Image, BYTE*Output,int W, int H, int Angle){
    int tmpX,tmpY;
    double Radian = Angle*3.141592/180.0;
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            tmpX = (int)(cos(Radian)*j + sin(Radian)*i);
            tmpY = (int)(-sin(Radian)*j + cos(Radian)*i);
            if(((tmpY < H) && (tmpY >= 0)) && ((tmpX < W) && (tmpX >=0))){
                Output[i*W + j] = Image[ tmpY*W + tmpX];
            }
        }
    }
};
*/

void FillColor(BYTE*Image, int X, int Y, int W, int H, BYTE R, BYTE G, BYTE B){
    Image[Y*W*3 + X*3] = B;
    Image[Y*W*3 + X*3 +1] = G;
    Image[Y*W*3 + X*3 +2] = R;
}