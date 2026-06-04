// 202010493 이현규
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

// ===== Function Group Hoisting =====
// ----- Utilities ------
void SaveBMPFile(
    BITMAPFILEHEADER hf,
    BITMAPINFOHEADER hInfo,
    RGBQUAD* hRGB,
    BYTE* Output,
    int W, int H,
    const char* Filename
);

void SameImage(
    BITMAPINFOHEADER hInfo,
    BYTE* Image, BYTE* Output,
    int W, int H
);

void InverseImage(
    BYTE* Image, BYTE* Output,
    int W, int H
);

void ResetOutput(
    BITMAPINFOHEADER hInfo,
    BYTE* Output,
    int W, int H
);

void ResethRGB(RGBQUAD* hRGB);

// ----- Color Processing ------
void FillOnePixel(
    BITMAPINFOHEADER hInfo,
    BYTE*Image, BYTE*Output, 
    int X, int Y, // objective
    int W, int H,
    BYTE R, BYTE G, BYTE B
);

void FillHorizintalLine(
    BITMAPINFOHEADER hInfo,
    BYTE*Image, BYTE*Output,
    int Y, // objective
    int W, int H,
    BYTE R, BYTE G, BYTE B
);

void FillRecktangle(
    BITMAPINFOHEADER hInfo,
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
);

void RGB2YCbCr(
    BYTE* Image,
    BYTE* Y,
    BYTE* Cb,
    BYTE* Cr,
    int W, int H
);

void YCbCrSeperator(
    FILE* fp, 
    BYTE* Y,
    BYTE* Cb,
    BYTE* Cr,
    int W, int H
);

void StrawberryRedFilter_RGB(
    BYTE* Image, BYTE* Output,
    int W, int H
);

void StrawberryRedFilter_YCbCr(
    BYTE* Image, BYTE* Output,
    BYTE* Cb,
    BYTE* Cr,
    int W, int H
);

// ----- Mophology -----

void Erosion(
    BYTE*Image, BYTE*Output, 
    int W, int H
);

void Dilation(
    BYTE*Image, BYTE*Output, 
    int W, int H
);

unsigned char** imageMatrix;
unsigned char blankPixel = 255, imagePixel = 0;

typedef struct {
	int row, col;
}pixel;

int getBlackNeighbours(
    int row, int col
);

int getBWTransitions(
    int row, int col
);

int zhangSuenTest1(
    int row, int col
);

int zhangSuenTest2(
    int row, int col
);

void zhangSuen(
    unsigned char* image, 
    unsigned char* output, 
    int rows, int cols
);

// ----- FeatureExtraction ------
void FeatureExtractThinImage(
    BYTE * Image, BYTE * Output, 
    int W, int H
);

// ===== Main ======

int main(){
    // BMP Header Variables
    BITMAPFILEHEADER hf; // 14 Bytes
    BITMAPINFOHEADER hInfo; // 40 Bytes
    RGBQUAD hRGB[256];

    // Open Files
    FILE* fp;
    fp = fopen("dilation.bmp", "rb");
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
    BYTE* Y = (BYTE*)malloc(ImgSize);
    BYTE* Cb = (BYTE*)malloc(ImgSize);
    BYTE* Cr = (BYTE*)malloc(ImgSize);
    if(hInfo.biBitCount == 24){
        // True Color
        Image = (BYTE*)malloc(ImgSize*3);
        Output = (BYTE*)malloc(ImgSize*3);
        fread(Image, sizeof(BYTE), ImgSize*3, fp);
    }else{
        // Gray Scale
        fread(hRGB, sizeof(RGBQUAD), 256, fp);
        Image = (BYTE*)malloc(ImgSize);
        Output = (BYTE*)malloc(ImgSize);
        fread(Image, sizeof(BYTE), ImgSize, fp);
    }
    fclose(fp);

    // Initiation
    ResethRGB(hRGB);
    ResetOutput(hInfo,Output,W,H);


    Dilation(Image, Output,W,H);
    Dilation(Output, Image,W,H);
    Dilation(Image, Output,W,H);
    Erosion(Output, Image,W,H);
    Erosion(Image, Output,W,H);
    Erosion(Output, Image,W,H);
    InverseImage(Image, Image,W, H);
    zhangSuen(Image,Output,H,W);

    // Make Image == Output to seperate buffer
    SameImage(hInfo,Output,Image,W,H);

    // Detect Feature
    FeatureExtractThinImage(Image,Output,W,H);

    SaveBMPFile(hf,hInfo,hRGB,Output,W,H,"FeatureExtractThinImage_output.bmp");

    // Free Memory
    free(Image); 
    free(Output);
    free(Y);
    free(Cb);
    free(Cr);
}

// ===== Function Group =====
// ----- Utilities ------
void SaveBMPFile(
    BITMAPFILEHEADER hf,
    BITMAPINFOHEADER hInfo,
    RGBQUAD* hRGB,
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
        fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
        fwrite(Output, sizeof(BYTE), W*H, fp);
    }
    fclose(fp);
}

void SameImage(
    BITMAPINFOHEADER hInfo,
    BYTE* Image, BYTE* Output,
    int W, int H
){
    if(hInfo.biBitCount == 24){
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                Output[i*W*3 + j*3] = Image[i*W*3 + j*3];
                Output[i*W*3 + j*3 + 1] = Image[i*W*3 + j*3 + 1];
                Output[i*W*3 + j*3 + 2] = Image[i*W*3 + j*3 + 2];
            }
        }
    }else{
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                Output[i*W + j] = Image[i*W + j];
            }
        }
    }
}

void InverseImage(
    BYTE* Image, BYTE* Output,
    int W, int H
){
    for(int i=0; i<H; i++){
        for(int j=0; j<W; j++){
            if(Image[i*W +j] == 0){
                Output[i*W +j] = 255;
            }else{
                Output[i*W +j] = 0;
            }
        }
    }
}

void ResetOutput(
    BITMAPINFOHEADER hInfo,
    BYTE* Output,
    int W, int H
){
    if(hInfo.biBitCount == 24){
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                Output[i*W*3 + j*3] = 0;
                Output[i*W*3 + j*3 + 1] = 0;
                Output[i*W*3 + j*3 + 2] = 0;
            }
        }
    }else{
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                Output[i*W + j] = 0;
            }
        }
    }
}

// ----- Color Processing ------
void FillOnePixel(
    BITMAPINFOHEADER hInfo,
    BYTE*Image, BYTE*Output, 
    int X, int Y, // objective
    int W, int H,
    BYTE R, BYTE G, BYTE B
){
    SameImage(hInfo,Image, Output, W,H);
    Output[Y*W*3 +X*3] = B;
    Output[Y*W*3 +X*3 + 1] = G;
    Output[Y*W*3 +X*3 + 2] = R;
}

void FillHorizintalLine(
    BITMAPINFOHEADER hInfo,
    BYTE*Image, BYTE*Output,
    int Y, // objective
    int W, int H,
    BYTE R, BYTE G, BYTE B
){
    for(int i = 0; i < W; i++){
        FillOnePixel(hInfo,Image,Output,i,Y,W,H,R,G,B);
    }
}

void FillRecktangle(
    BITMAPINFOHEADER hInfo,
    BYTE*Image, BYTE*Output, 
    int Xs, int Ys, // objective
    int Xf, int Yf, // objective
    int W, int H,
    BYTE R, BYTE G, BYTE B
){
    for(int i = Ys; i <= Yf; i++){
        for(int j = Xs; j <= Xf; j++){
            FillOnePixel(hInfo,Image, Output, j, i, W, H, R, G, B);
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

void RGB2YCbCr(
    BYTE* Image,
    BYTE* Y,
    BYTE* Cb,
    BYTE* Cr,
    int W, int H
){
    for(int i=0; i<H; i++){
        for(int j=0; j<W; j++){
            Y[i*W + j] = (BYTE)(
                0.299*Image[(i*W + j)*3 +2] +
                0.587*Image[(i*W + j)*3 +1] +
                0.114*Image[(i*W + j)*3 +0]
            );
            Cb[i*W + j] = (BYTE)(
                -0.16874*Image[(i*W + j)*3 +2] +
                -0.3313* Image[(i*W + j)*3 +1] +
                0.5*     Image[(i*W + j)*3 +0] +
                128
            );
            Cr[i*W + j] = (BYTE)(
                0.5*    Image[(i*W + j)*3 +2] +
                -0.4187*Image[(i*W + j)*3 +1] +
                -0.0813*Image[(i*W + j)*3 +0] +
                128
            );
        }
    }
}

void StrawberryRedFilter_RGB(
    BYTE* Image, BYTE* Output,
    int W, int H
){
    for(int i=0; i<H; i++){
        for(int j=0; j<W; j++){
            if(
                (Image[i*W*3 + j*3 + 2] > 130) &&
                (Image[i*W*3 + j*3 + 1] < 50) &&
                (Image[i*W*3 + j*3 + 0] < 150)
            ){
                Output[i*W*3 + j*3 + 0] = Image[i*W*3 + j*3 + 0];
                Output[i*W*3 + j*3 + 1] = Image[i*W*3 + j*3 + 1];
                Output[i*W*3 + j*3 + 2] = Image[i*W*3 + j*3 + 2];
            }else{
                Output[i*W*3 + j*3 + 0] = 0;
                Output[i*W*3 + j*3 + 1] = 0;
                Output[i*W*3 + j*3 + 2] = 0;
            }
        }
    }
}

void StrawberryRedFilter_YCbCr(
    BYTE* Image, BYTE* Output,
    BYTE* Cb,
    BYTE* Cr,
    int W, int H
){
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            if(
                (Cb[i*W + j] < 140) &&
                (Cr[i*W + j] > 190)
            ){
                Output[i*W*3 + j*3 + 0] = Image[i*W*3 + j*3 + 0];
                Output[i*W*3 + j*3 + 1] = Image[i*W*3 + j*3 + 1];
                Output[i*W*3 + j*3 + 2] = Image[i*W*3 + j*3 + 2];
            }else{
                Output[i*W*3 + j*3 + 0] = 0;
                Output[i*W*3 + j*3 + 1] = 0;
                Output[i*W*3 + j*3 + 2] = 0;
            }
        }
    }
}

void YCbCrSeperator(
    FILE* fp, 
    BYTE* Y,
    BYTE* Cb,
    BYTE* Cr,
    int W, int H
){
    // Strawberry -> (Cb > 130) && (Cr > 200)
    fp = fopen("Y.raw","wb");
    fwrite(Y, sizeof(BYTE), W*H, fp);
    fclose(fp);
    fp = fopen("Cb.raw","wb");
    fwrite(Cb, sizeof(BYTE), W*H, fp);
    fclose(fp);
    fp = fopen("Cr.raw","wb");
    fwrite(Cr, sizeof(BYTE), W*H, fp);
    fclose(fp);
}

// ----- Mophology -----

void Erosion(
    BYTE*Image, BYTE*Output, 
    int W, int H
){
    for(int i=1; i<H-1; i++){
        for(int j = 1; j < W-1; j++){
            if(Image[i*W + j] == 255){ //Foreground
                if(
                    !((Image[(i-1)*W + j] == 255) &&
                    (Image[(i+1)*W + j] == 255) &&
                    (Image[(i)*W + j-1] == 255) &&
                    (Image[(i)*W + j+1] == 255))
                ){ 
                    Output[i*W + j] = 0; 
                }else{
                    Output[i*W + j] = 255;
                }
            }else{ 
                Output[i*W + j] = 0; 
            }
        }
    }
}

void Dilation(
    BYTE*Image, BYTE*Output, 
    int W, int H
){
    for(int i=1; i<H-1; i++){
        for(int j = 1; j < W-1; j++){
            if(Image[i*W + j] == 0){ //Background
                if(
                    !((Image[(i-1)*W + j] == 0) &&
                    (Image[(i+1)*W + j] == 0) &&
                    (Image[(i)*W + j-1] == 0) &&
                    (Image[(i)*W + j+1] == 0))
                ){ 
                    Output[i*W + j] = 255; 
                }else{
                    Output[i*W + j] = 0;
                }
            }else{ 
                Output[i*W + j] = 255; 
            }
        }
    }
}

void ResethRGB(RGBQUAD* hRGB){
    for(int i=0; i<256; i++){
        hRGB[i].rgbRed 
        = hRGB[i].rgbGreen 
        = hRGB[i].rgbBlue 
        = hRGB[i].rgbReserved 
        = i;
    }
}

int getBlackNeighbours(int row, int col) {

	int i, j, sum = 0;

	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++) {
			if (i != 0 || j != 0)
				sum += (imageMatrix[row + i][col + j] == imagePixel);
		}
	}

	return sum;
}

int getBWTransitions(int row, int col) {
	return 	(
        ((imageMatrix[row - 1][col] == blankPixel) && 
        (imageMatrix[row - 1][col + 1] == imagePixel))
		+ 
        ((imageMatrix[row - 1][col + 1] == blankPixel) && 
        (imageMatrix[row][col + 1] == imagePixel))
		+ 
        ((imageMatrix[row][col + 1] == blankPixel) && 
        (imageMatrix[row + 1][col + 1] == imagePixel))
		+ 
        ((imageMatrix[row + 1][col + 1] == blankPixel) && 
        (imageMatrix[row + 1][col] == imagePixel))
		+ 
        ((imageMatrix[row + 1][col] == blankPixel) && 
        (imageMatrix[row + 1][col - 1] == imagePixel))
		+ 
        ((imageMatrix[row + 1][col - 1] == blankPixel) && 
        (imageMatrix[row][col - 1] == imagePixel))
		+ 
        ((imageMatrix[row][col - 1] == blankPixel) && 
        (imageMatrix[row - 1][col - 1] == imagePixel))
		+ 
        ((imageMatrix[row - 1][col - 1] == blankPixel) && 
        (imageMatrix[row - 1][col] == imagePixel)));
}

int zhangSuenTest1(int row, int col) {
	int neighbours = getBlackNeighbours(row, col);

	return ((neighbours >= 2 && neighbours <= 6)
		&& (getBWTransitions(row, col) == 1)
		&& (imageMatrix[row - 1][col] == blankPixel || imageMatrix[row][col + 1] == blankPixel || imageMatrix[row + 1][col] == blankPixel)
		&& (imageMatrix[row][col + 1] == blankPixel || imageMatrix[row + 1][col] == blankPixel || imageMatrix[row][col - 1] == blankPixel));
}

int zhangSuenTest2(int row, int col) {
	int neighbours = getBlackNeighbours(row, col);

	return ((neighbours >= 2 && neighbours <= 6)
		&& (getBWTransitions(row, col) == 1)
		&& (imageMatrix[row - 1][col] == blankPixel || imageMatrix[row][col + 1] == blankPixel || imageMatrix[row][col - 1] == blankPixel)
		&& (imageMatrix[row - 1][col] == blankPixel || imageMatrix[row + 1][col] == blankPixel || imageMatrix[row][col + 1] == blankPixel));
}

void zhangSuen(unsigned char* image, unsigned char* output, int rows, int cols) {

	int startRow = 1, startCol = 1, endRow, endCol, i, j, count, processed;

	pixel* markers;

	imageMatrix = (unsigned char**)malloc(rows * sizeof(unsigned char*));

	for (i = 0; i < rows; i++) {
		imageMatrix[i] = (unsigned char*)malloc((cols + 1) * sizeof(unsigned char));
		for (int k = 0; k < cols; k++) imageMatrix[i][k] = image[i * cols + k];
	}

	endRow = rows - 2;
	endCol = cols - 2;
	do {
		markers = (pixel*)malloc((endRow - startRow + 1) * (endCol - startCol + 1) * sizeof(pixel));
		count = 0;

		for (i = startRow; i <= endRow; i++) {
			for (j = startCol; j <= endCol; j++) {
				if (imageMatrix[i][j] == imagePixel && zhangSuenTest1(i, j) == 1) {
					markers[count].row = i;
					markers[count].col = j;
					count++;
				}
			}
		}

		processed = (count > 0);

		for (i = 0; i < count; i++) {
			imageMatrix[markers[i].row][markers[i].col] = blankPixel;
		}

		free(markers);
		markers = (pixel*)malloc((endRow - startRow + 1) * (endCol - startCol + 1) * sizeof(pixel));
		count = 0;

		for (i = startRow; i <= endRow; i++) {
			for (j = startCol; j <= endCol; j++) {
				if (imageMatrix[i][j] == imagePixel && zhangSuenTest2(i, j) == 1) {
					markers[count].row = i;
					markers[count].col = j;
					count++;
				}
			}
		}

		if (processed == 0)
			processed = (count > 0);

		for (i = 0; i < count; i++) {
			imageMatrix[markers[i].row][markers[i].col] = blankPixel;
		}

		free(markers);
	} while (processed == 1);


	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			output[i * cols + j] = imageMatrix[i][j];
		}
	}
}

// ----- FeatureExtraction -----
void FeatureExtractThinImage(
    BYTE * Image, BYTE * Output, 
    int W, int H
){  
    // ===== temp, freq, prev, line: for branch
    int temp;
    int freq;
    int prev;
    
    for(int i = 1; i < H-1; i++){
        for(int j = 1; j < W-1; j++){
            int sum = 0;
            freq = 0;
            int line[9];
            for (int di = -1; di <=1; di++){
                for (int dj = -1; dj <=1; dj++){
                    if(di ==0 && dj ==0) continue;
                    sum += Image[(i + di)*W + (j + dj)]; 
                    line[(1+di)*3 + (1+dj)] = Image[(i + di)*W + (j + dj)];
                }
            }

            // rearange raster sequence (line[0..8], 4=center) as clock ring
            // line[0..7] = TL, T, TR, R, BR, B, BL, L
            int Lv  = line[3]; // L
            int Rv  = line[5]; // R
            int Bv  = line[7]; // B
            int BRv = line[8]; // LB
            line[3] = Rv; // R
            line[4] = BRv; // RB
            line[5] = Bv; // B
            // line[6] = BL (LB)
            line[7] = Lv; // L

            // edge detection 
            prev = line[0];
            for(int p = 1; p<8; p++){
                if(line[p] != line[p-1]){
                    freq += 1;
                }
            }
            
            if(line[0] != line[7]){   // wrap: Circle
                freq += 1;
            }

            if( Image[i*W + j] == 0){
                if( sum == 1785 || freq == 6 
                    // ( // End
                    // (Image[(i-1)*W + (j-1)]) +
                    // (Image[(i-1)*W + (j+0)]) +
                    // (Image[(i-1)*W + (j+1)]) +

                    // (Image[(i+0)*W + (j-1)]) +
                    // // (Image[(i+0)*W + (j+0)]) +
                    // (Image[(i+0)*W + (j+1)]) +

                    // (Image[(i+1)*W + (j-1)]) +
                    // (Image[(i+1)*W + (j+0)]) +
                    // (Image[(i+1)*W + (j+1)]) == 1785)
                ){
                    for (int di = -1; di <=1; di++){
                        for (int dj = -1; dj <=1; dj++){
                            if(di == 0 && dj == 0) continue;
                            Output[(i + di)*W + (j + dj)] = 80;
                        }
                    }
                    // (Output[(i-1)*W + (j-1)]) =
                    // (Output[(i-1)*W + (j+0)]) =
                    // (Output[(i-1)*W + (j+1)]) =

                    // (Output[(i+0)*W + (j-1)]) =
                    // // (Output[(i+0)*W + (j+0)]) =
                    // (Output[(i+0)*W + (j+1)]) =

                    // (Output[(i+1)*W + (j-1)]) =
                    // (Output[(i+1)*W + (j+0)]) =
                    // (Output[(i+1)*W + (j+1)]) = 80;
                }
            }
        }
    }
}