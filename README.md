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
Use a browser to nagivate to http://localhost:1024 to view the error message sent from the http server.

