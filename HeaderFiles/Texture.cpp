#include "Texture.h"

Texture::Texture(string textureFilePath)
{
    // This fails sometimes ermm...
    stbi_set_flip_vertically_on_load(true);

    //1st para: path of image
    texFilePath = textureFilePath;
    tex_bytes = stbi_load(
        texFilePath.c_str(), // Convert string into char*
        &img_w,
        &img_h,
        &color_channels,
        0
    );

    if (tex_bytes == nullptr)
    {
        std::cout << "Failed to load image" << std::endl;
    }
    else
    {
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Color channels are RGB/A
        // If int color channels == 3, RGB jpg
        // If int color channels == 4, RGBA png
        if (color_channels == 3)
        {
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGB,
                img_w,
                img_h,
                0, //border
                GL_RGB,
                GL_UNSIGNED_BYTE,
                tex_bytes
            );
        }
        else if (color_channels == 4)
        {
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA, //4 == rgba, 3 == rgb
                img_w,
                img_h,
                0, //border
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                tex_bytes
            );
        }
        else
        {
            std::cout << "  Color channels are not equal to neither 3 nor 4" << std::endl;
            std::cout << "  No Texture was generated" << std::endl;
        }

        //Mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(tex_bytes);
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture);
}

GLuint Texture::getTexture() {
    return texture;
}