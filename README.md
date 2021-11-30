## Description
This project is an image processing server for GNU/Linux operating systems.

## Compilation
First install [opencv](https://opencv.org/)

To compile, simply run:
```
make
```

## Testing
Test the webserver by executing the binary with a port number:
```
./ips-server 1024
```
Use a browser to nagivate to the server and choose and image processing technique.
```
http://localhost:1024/grayscale
http://localhost:1024/edge-detect
http://localhost:1024/blur
```

Test the client by executing the binary with a port number and an image. The server should print the clients request and will download the image from the client in the downloads directory, process it, and send it back to the client. The client should print out the response from the server and download the image next to the input file. 
```
./ips-client 1024 test_images/test1.jpg grayscale
./ips-client 1024 test_images/test1.jpg edge-detect
./ips-client 1024 test_images/test1.jpg blur
```
