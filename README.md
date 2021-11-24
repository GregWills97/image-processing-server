## Description
This project is the early stages of an image processing server for GNU/Linux operating systems. Currently, all that has been developed is a simple application to convert an image to grayscale using OpenCV.

## Compilation
First install [opencv](https://opencv.org/)

To compile, simply run:
```
make
```

## Testing
Execute the test binary with an test image from the test_image directory. Choose "grayscale" to return a grayscaled image.
```
./test test_images/test1.jpg grayscale
```

