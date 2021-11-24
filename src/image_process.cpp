#include <opencv2/opencv.hpp>

#include "image_process.h"

void grayscale_image(const char* in_file_path, const char* out_file_path) {

    /* Create images */
    cv::Mat image;
    cv::Mat gray_image;

    /* Read image, convert to grayscale, write it out*/
    image = cv::imread(in_file_path, 1);
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    cv::imwrite(out_file_path, gray_image);
    
    /* show in window */
    cv::namedWindow(in_file_path, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(out_file_path, cv::WINDOW_AUTOSIZE);
    cv::imshow(in_file_path, image);
    cv::imshow(out_file_path, gray_image);
    
    /* wait till button is pressed to close windows */
    cv::waitKey(0);
}
