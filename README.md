# editor4d

work in progress!

Current build:
```bash
# Active emsdk environment. TODO replace with docker container.
source ./emsdk/emsdk_env.sh
rm -rf ./build; mkdir build; cd build; 
cmake ../src/ -DCMAKE_TOOLCHAIN_FILE=${EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake;
make;
```

After that open simple http server for e.g:
```bash
python -m SimpleHTTPServer
```

Test page will be under:
```
http://localhost:8000/src/frontend/
```

# TODO
* Fix some of the CMake issues and wire GTests framework.
* Start implementing first stage from [design doc](https://docs.google.com/document/d/1NeKbrI7UfwVtlnNUE7rIlt4I5ooexyFDAwSt2KfBqTY/edit?usp=sharing)
* Set proper Docker container with **third_party** libraries installation and emscripten setup. For Dockerfile reference you can take emsdk as an example. Create simple script which will start server on exposed Docker port.
