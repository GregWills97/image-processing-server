#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;

int main( int argc, char** argv )
{
 char* imageName = argv[1];
 char output_image[128];

 strcpy(output_image, OUTPUT_DIR);
 strcat(output_image, "Gray_Image.png");

 Mat image;
 image = imread( imageName, 1 );

 if( argc != 2 || !image.data )
 {
   printf( " No image data \n " );
   return -1;
 }

 Mat gray_image;
 cvtColor( image, gray_image, COLOR_BGR2GRAY );

 imwrite( output_image, gray_image );

 namedWindow( imageName, WINDOW_AUTOSIZE );
 namedWindow( "Gray_image", WINDOW_AUTOSIZE );

 imshow( imageName, image );
 imshow( "Gray_image", gray_image );

 waitKey(0);

 return 0;
}
