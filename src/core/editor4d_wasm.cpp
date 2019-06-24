#include <iostream>
#include <vector>

#include <emscripten.h>

#include "geometry.h"

// the function called by the javascript code
extern "C" void EMSCRIPTEN_KEEPALIVE HelloWorld() {
  std::vector<float> vertices = {
     0.0f, 0.0f, 0.0f, 0.0f,
     0.0f, 1.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 1.0f, 0.0f
  };
  std::vector<int> indices = {0, 1, 2};
  editor4d::Geometry geometry(vertices, indices);
  std::cout << "Hello world kuddai" << std::endl;
  for (int index : geometry.GetIndices()) {
    std::cout << index << " ";
  }
  std::cout << std::endl;
}

