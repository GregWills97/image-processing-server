## Description
This project is the early stages of an image processing server for GNU/Linux operating systems. Currently, all that has been developed is opencv image manipulation functions and a simple http server.

## Compilation
First install [opencv](https://opencv.org/)

To compile, simply run:
```
make
```

## Testing
Test the webserver by executing the binary with a port number:
```
./server 1024
```
Use a browser to nagivate to the server and choose and image processing technique.
```
http://localhost:1024/grayscale
http://localhost:1024/edge-detect
http://localhost:1024/blur
```

