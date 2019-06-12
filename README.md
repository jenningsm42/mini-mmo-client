# mini-mmo-client
[![Build Status](https://travis-ci.com/jenningsm42/mini-mmo-client.svg?branch=master)](https://travis-ci.com/jenningsm42/mini-mmo-client)

The client for a very simple open world (not massively) multiplayer online game. The server can be found [here](https://github.com/jenningsm42/mini-mmo-server).

## Requirements
* Protobuf 3 compiler
* Protobuf 3 library
* SFML 2.5
* TGUI 0.8
* CMake 3.1+
* A C++17 compiler
* Spine 3.7 SFML Runtime\*

\* *You may remove the Spine runtime dependency yourself to have the repository fall back onto the MIT license.*

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

## License
The Spine runtime is used to handle animations, so the repository falls under the Spine Runtime License by default. However, you are free to remove the Spine runtime yourself, in which case the repository will fall under the MIT license.

Under the Spine Runtime License, you can compile this repository and run it for personal use, but you may not modify the animations without a [Spine](http://esotericsoftware.com/) license. A copy of the Spine Runtime License has been provided for your convenience, dated at the time of integration.
