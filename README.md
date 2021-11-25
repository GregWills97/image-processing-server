## Description
This project is the early stages of an image processing server for GNU/Linux operating systems. Currently, all that has been developed is a simple application to convert an image to grayscale using OpenCV.

## Compilation
First install [opencv](https://opencv.org/)

To compile, simply run:
```
make
```

## Testing
Execute the test binary with an test image from the test_image directory. Choose "grayscale" or "blur" to return a grayscaled or blurred image.
```
./test test_images/test1.jpg grayscale
./test test_images/test1.jpg blur
```
Test edge detection with the command below, where 0 and 100 will be the low and high thresholds respectively.
```
./test test_images/test1.jpg edge-detect 0 100
```

