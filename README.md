# Image drawer 2D

Library allowing to draw simple 2D shapes and store it as image. Main purpose of the library is to visualise and test data in unit tests while avoiding use of additional libraries.


## Features

- drawing shapes: line, circle, rectangle
- storing as PNG
- comparing images
- visualising difference between images


## Example of shapes

![Shapes image](doc/shapes_example.png "Shapes image")


## Example of generated diff image

![Diff image](doc/diff_example.png "Diff image")

Example presents difference of two images. Result consists of four *subimages*:
- on upper left corner: test image
- upper right corner: reference image
- lower right corner: difference between two images
- lower left corner: mask of differences


## Code examples

Code snipets and examples can be found in unit testing direcotry: ```src/imgdraw2d/tests```


## Building library

```
cd <path_to_build_dir>
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=<path_to_install_dir> <path_to_src_dir>
make install
```
where:
- ```path_to_src_dir``` is directory where library sources (content of *src* dir)
- ```path_to_build_dir``` is directory where sources will be build
- ```path_to_install_dir``` is directory where compilation artefacts (binaries and headers) will be deployed


## ToDo

- write text on image
- draw scale/measure


## Dependencies

Library depends on following components:
- libpng++
