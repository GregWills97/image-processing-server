#include <opencv2/opencv.hpp>

#include <vector>
#include "image_process.hpp"

void grayscale_file(const char* in_file_path, const char* out_file_path) {

    /* Create images */
    cv::Mat src;
    cv::Mat dst;

    /* Read image, convert to grayscale, write it out*/
    src = cv::imread(in_file_path, 1);
    cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
    cv::imwrite(out_file_path, dst);
    
    ///* show in window */
    //cv::namedWindow(in_file_path, cv::WINDOW_AUTOSIZE);
    //cv::namedWindow(out_file_path, cv::WINDOW_AUTOSIZE);
    //cv::imshow(in_file_path, src);
    //cv::imshow(out_file_path, dst);
    //
    ///* wait till button is pressed to close windows */
    //cv::waitKey(0);
}

void edge_detect_file(const char* in_file_path, const char* out_file_path, int low_threshold, int high_threshold, int kernel_size) {

    /* Create images */
    cv::Mat src;
    cv::Mat dst;

    /* Read in image, convert to gray scale */
    src = cv::imread(in_file_path, 1);
    cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
    cv::blur(dst, dst, cv::Size(3,3));
    cv::Canny(dst, dst, low_threshold, high_threshold, kernel_size);
    cv::imwrite(out_file_path, dst);
    
    ///* Show in window */
    //cv::namedWindow(in_file_path, cv::WINDOW_AUTOSIZE);
    //cv::namedWindow(out_file_path, cv::WINDOW_AUTOSIZE);
    //cv::imshow(in_file_path, src);
    //cv::imshow(out_file_path, dst);

    ///* wait till button is pressed to close windows */
    //cv::waitKey(0);
}

void blur_file(const char* in_file_path, const char* out_file_path, int kernel_size) {

    /* Create images */
    cv::Mat src;
    cv::Mat dst;

    /* Read in image, convert to gray scale */
    src = cv::imread(in_file_path, 1);
    dst.create(src.size(), src.type());
    cv::blur(src, dst, cv::Size(kernel_size,kernel_size));
    cv::imwrite(out_file_path, dst);
    
    /* Show in window */
    //cv::namedWindow(in_file_path, cv::WINDOW_AUTOSIZE);
    //cv::namedWindow(out_file_path, cv::WINDOW_AUTOSIZE);
    //cv::imshow(in_file_path, src);
    //cv::imshow(out_file_path, dst);

    ///* wait till button is pressed to close windows */
    //cv::waitKey(0);
}

void face_detect_file(const char* in_file_path, const char* out_file_path){
  /* Create images */
  cv::Mat src;
  cv::Mat dst;

  src = cv::imread(in_file_path, 1);
  dst = src;
  
  //Pre-built Face Detection Classifier found in OpenCV source.
  cv::CascadeClassifier faceCascade;
  faceCascade.load("/home/terrill/FinalProject/fork/image-processing-server/src/server/haarcascade_frontalface_default.xml");

  cv::Mat grayscale;
  cv::cvtColor(src, grayscale, cv::COLOR_BGR2GRAY);

  // Face Detection Bounding Rectangles.
  std::vector<cv::Rect> faces;
  faceCascade.detectMultiScale(grayscale, faces, 1.1, 3, 0, cv::Size(40,40));

  //Draw bounding rectangles in image.
  for(cv::Rect area : faces)
    {
      cv::Scalar drawColor = cv::Scalar(255, 0, 0);
      cv::rectangle(dst, cv::Point(cvRound(area.x), cvRound(area.y)), cv::Point(cvRound(area.x + area.width - 1), cvRound(area.y + area.height - 1)), drawColor);
    }

  //store image.
  cv::imwrite(out_file_path, dst);
}
  
