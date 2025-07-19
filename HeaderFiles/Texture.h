#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include <iostream>
#include <string>

using namespace std;

class Texture {
private:
    //Texture Data
    int img_w, img_h, color_channels; //3 color channels (RGB) or 4 for RGBA
    unsigned char* tex_bytes;
    string texFilePath;
    GLuint texture;

public:
    Texture(string textureFilePath);
    ~Texture();
    GLuint getTexture();
};

#endif