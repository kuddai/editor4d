#include <functional>
#include <iostream>

#include <emscripten.h>
#include <SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

#include "nlohmann/json.hpp"

using json = nlohmann::json;
// Shader sources

const GLchar* vertexSource = R"SNAKE_EATER(
    varying vec3 vBC;
    attribute vec3 position, barycentric;
    void main()
    {
      vBC = barycentric;
      gl_Position = vec4(position.xyz, 1.0);
    }
)SNAKE_EATER";

const GLchar* fragmentSource = R"SNAKE_EATER(
    #extension GL_OES_standard_derivatives : enable
    precision mediump float;
    varying vec3 vBC;

    const float LINE_WIDTH = 5.5;  // in pixels

    float edgeFactor(){
        vec3 d = fwidth(vBC);
        vec3 a3 = smoothstep(vec3(0.0), d*LINE_WIDTH, vBC);
        return min(min(a3.x, a3.y), a3.z);
    }

    void main()                                  
    {
      float f = edgeFactor();

      if (f > 0.5) {
        discard;
      } else {
        gl_FragColor = vec4(0.0, 1.0, 0.0, (1.0 - f));
      }
    }
)SNAKE_EATER";

// an example of something we will control from he javascript side
bool background_is_black = true;

// the function called by the javascript code
extern "C" void EMSCRIPTEN_KEEPALIVE toggle_background_color() { background_is_black = !background_is_black; }
extern "C" void EMSCRIPTEN_KEEPALIVE pass_geometry(char *geom_json) {
    try {
        json j = json::parse(geom_json);
        std::cout << "got: " << j << std::endl;
    } catch (json::parse_error &e) {
        std::cerr << e.what() << std::endl;
    }
}


std::function<void()> loop;
void main_loop() { loop(); }

// somehow after switching to CMakeLists during linkage SDL_CreateWindowAndRenderer
// can't be found anymore. I tried to fix this in different ways,
// but eventually I had to copy paste it from original library
// https://github.com/emscripten-ports/SDL2/blob/1d03c6824752d71d44c7e7ea3dd28909deda9659/src/render/SDL_render.c#L254
int SDL_CreateWindowAndRenderer(int width, int height, Uint32 window_flags,
                            SDL_Window **window, SDL_Renderer **renderer)
{
    *window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     width, height, window_flags);
    if (!*window) {
        *renderer = NULL;
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, 0);
    if (!*renderer) {
        return -1;
    }

    return 0;
}

int main()
{
    std::cout << "hello, world 2!\n" << std::endl;
    SDL_Window *window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED,
                                                SDL_WINDOWPOS_UNDEFINED,
                                                640, 480, 0);
    SDL_CreateRenderer(window, -1, 0);
    // SDL_CreateWindowAndRenderer(640, 480, 0, &window, nullptr);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};
    GLfloat barycentric[] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint pos_vbo, bar_vbo;
    glGenBuffers(1, &pos_vbo);
    glGenBuffers(1, &bar_vbo);

    // Specify the layout of the vertex data
    GLint pos_attr = glGetAttribLocation(shaderProgram, "position");
    GLint bar_attr = glGetAttribLocation(shaderProgram, "barycentric");


    loop = [&]
    {
        // Clear the screen
        if(background_is_black) {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }
        else {
            glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        }
        glClear(GL_COLOR_BUFFER_BIT);

        // move a vertex
        const uint32_t milliseconds_since_start = SDL_GetTicks();
        const uint32_t milliseconds_per_loop = 3000;
        vertices[0] = ( milliseconds_since_start % milliseconds_per_loop ) / float(milliseconds_per_loop) - 0.5f;

        glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(pos_attr, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(pos_attr);

        glBindBuffer(GL_ARRAY_BUFFER, bar_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(barycentric), barycentric, GL_STATIC_DRAW);
        glVertexAttribPointer(bar_attr, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(bar_attr);

        // Draw a triangle from the 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);
        SDL_GL_SwapWindow(window);
    };

    emscripten_set_main_loop(main_loop, 0, true);

    return EXIT_SUCCESS;
}
