#include <stdio.h>
#include <stdlib.h>

// Structure to hold grayscale pixel data
typedef struct {
    unsigned char value;
} PixelGray;

// Function to read a PGM image into a 2D array
PixelGray** readPGM(const char* filename, int* width, int* height);

// Function to write a 2D matrix as a PGM image
void writePGM(const char* filename, PixelGray** matrix, int* width, int* height);

// Function to threshold the image matrix
PixelGray** threshold(PixelGray** matrix, int* width, int* height);

// Function to rotate the image matrix
PixelGray** rotate(PixelGray** matrix, int* width, int* height);


int main() {
    int width, height;  // variable to hold width and height. Use reference in other functions
    char* fileName = malloc(sizeof(char) * 20);
    printf("Enter the name of your pgm file: \n");
    scanf("%s", fileName);

    PixelGray** image_original = readPGM(fileName, &width, &height);
    // Now you have the grayscale image data in the 'image_original' 2D array

    // Access pixel data using image[row][col].value
    // For example, to access the pixel at row=2, col=3:
    // unsigned char pixel_value = image[2][3].value;

    // Create a new 2D array 'image_thresh' to store the threshold image
    PixelGray** image_thresh = threshold(image_original, &width, &height);
    //write the image data as "threshold.pgm"
    writePGM("threshold.pgm", image_thresh, &width, &height);

    // Create a new 2D array 'image_rotate' to store the rotated image
    PixelGray** image_rotate = rotate(image_original, &width, &height);
    //write the image data as "rotate.pgm"
    writePGM("rotate.pgm", image_rotate, &width, &height);

    image_rotate = rotate(image_rotate, &width, &height);
    //write the image data as "rotate_again.pgm"
    writePGM("rotate_again.pgm", image_rotate, &width, &height);

    // Free the allocated memory when you're done
    for (int i = 0; i < height; ++i) {
        free(image_original[i]);
        free(image_thresh[i]);
        free(image_rotate[i]);
    }
    free(image_original);
    free(image_thresh);
    free(image_rotate);
    return 0;
}

PixelGray** readPGM(const char* filename, int* width, int* height) {
    FILE * imageFile = NULL;
    imageFile = fopen(filename, "rb");
    char magicNumber[3];
    if(imageFile == NULL) {                 //check that the file was successfully opened
        printf("Error: unable to open image file.");
        exit(0);
    }
    else {
        fscanf(imageFile, "%s", magicNumber);  //magicNumber should be P5, specifies the file is in binary format
        fscanf(imageFile, "%d %d", width, height);
        if (*height != *width)
            *height = *width;   //if image is not square, make it square
        int maxVal;
        fscanf(imageFile, "%d", &maxVal);      //maxVal holds the maximum pixel value, which is 255
        //allocates memory for an array of pointers to rows
        PixelGray **matrix = (int **) malloc(*height * sizeof(int*));
        if (matrix == NULL) {
            printf("Error: Unable to allocate memory for rows\n");
            return NULL;
        }
        //initialize matrix pointers to columns within each row
        for (int i = 0; i < *width; i++) {
            matrix[i] = (int*) malloc(*width * sizeof(int));
            if (matrix[i] == NULL) {
                printf("Error: Unable to allocate memory for columns in row %d\n", i);
            }
        }
        fgetc(imageFile);
        for (int i = 0; i < *height; i++) {
            for (int j = 0; j < *width; j++) {
                fread(&matrix[i][j].value, 1, 1, imageFile); //get one byte worth of data and store it in matrix
            }
        }
        fclose(imageFile);
        return matrix;
    }
}

PixelGray** threshold(PixelGray** matrix, int* width, int* height) {
    //allocate memory for array of pointers to rows
    PixelGray **thresholdMatrix = (int **) malloc(*height * sizeof(int*));
    if (thresholdMatrix == NULL) { //check that the row pointers were intialized successfully
        printf("Error: Unable to allocate memory for rows\n");
        return NULL;
    }
    //initialize matrix pointers to columns within each row
    for (int i = 0; i < *width; i++) {
        thresholdMatrix[i] = (int*) malloc(*width * sizeof(int));
        if (thresholdMatrix[i] == NULL) {
            printf("Error: Unable to allocate memory for columns in row %d\n", i);
        }
    }
    for (int i = 0; i < *height; i++) {
        //printf("%d: ", i);
        for (int j = 0; j < *width; j++) {
            if (matrix[i][j].value > 80)            //assign pixel values. either black(255) or white(0)
                thresholdMatrix[i][j].value = 255;
            else
                thresholdMatrix[i][j].value = 0;
            //printf("%d ", thresholdMatrix[i][j]);
        }
        //printf("\n");
    }

    return thresholdMatrix;
}

void writePGM(const char* filename, PixelGray** matrix, int* width, int* height) {
    FILE* outputFile = fopen(filename, "wb");
    if (outputFile == NULL) {
        printf("Failed to create file.");
        exit(1);
    }
    else {
        //print the magic number, dimensions, and max pixel value to file
        fprintf(outputFile, "%s\n%d %d\n%d\n", "P5", *width, *height, 255);
        for (int i = 0; i < *width; i++) {
            for (int j = 0; j < *height; j++) {
                fprintf(outputFile, "%c", matrix[i][j].value);
            }
        }
        fclose(outputFile);
    }
}

PixelGray** rotate(PixelGray** matrix, int* width, int* height) {
    //allocate memory for array of pointers to rows
    PixelGray **rotatedMatrix = (int **) malloc(*height * sizeof(int*));
    if (rotatedMatrix == NULL) { //check that the row pointers were intialized successfully
        printf("Error: Unable to allocate memory for rows\n");
        return NULL;
    }
    //initialize matrix pointers to columns within each row
    for (int i = 0; i < *width; i++) {
        rotatedMatrix[i] = (int*) malloc(*width * sizeof(int));
        if (rotatedMatrix[i] == NULL) {
            printf("Error: Unable to allocate memory for columns in row %d\n", i);
        }
    }
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            rotatedMatrix[j][i].value = matrix[i][j].value; //swap i and j to rotate image
        }
    }
    return rotatedMatrix;
}