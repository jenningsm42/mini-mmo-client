# mini-mmo-client
[![Build Status](https://travis-ci.com/jenningsm42/mini-mmo-client.svg?branch=master)](https://travis-ci.com/jenningsm42/mini-mmo-client)

The client for a very simple open world (not massively) multiplayer online game. The server can be found [here](https://github.com/jenningsm42/mini-mmo-server).

## Requirements
* Protobuf 3 compiler
* Protobuf 3 library
* SFML 2.5
* TGUI 0.8
* CMake 3.1+
* A C++14 compiler

## Usage
To build:

```
$ mkdir build && cd build
$ cmake ..
$ make
```

To run (after starting the server):

```
$ ./build/bin/client
```
