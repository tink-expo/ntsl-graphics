#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <math.h>
#include <stdlib.h>

// Picking Pass Rendering
GLuint picking_fbo;
GLuint picking_tex;
GLuint picking_depth;

inline void pickingInitialize(int frameBufferWidth, int frameBufferHeight)
{
    glGenFramebuffers(1, &picking_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, picking_fbo);

    // Create the texture object for the primitive information buffer
    glGenTextures(1, &picking_tex);
    glBindTexture(GL_TEXTURE_2D, picking_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, frameBufferWidth, frameBufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Attach to primitive texture image to a framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, picking_tex, 0);

    glGenRenderbuffers(1, &picking_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, picking_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, frameBufferWidth, frameBufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, picking_depth);

    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR: Framebuffer is not complete" << std::endl;
        std::cin.get();
        std::terminate();
    }

    // Unbind this framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

inline void reallocatePickingTexture(int frameBufferWidth, int frameBufferHeight)
{
    glDeleteTextures(1, &picking_tex);
    glDeleteTextures(1, &picking_depth);

    glBindFramebuffer(GL_FRAMEBUFFER, picking_fbo);
    glGenTextures(1, &picking_tex);
    glBindTexture(GL_TEXTURE_2D, picking_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, frameBufferWidth, frameBufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    // Attach to primitive texture image to a framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, picking_tex, 0);

    glGenRenderbuffers(1, &picking_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, picking_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, frameBufferWidth, frameBufferHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, picking_depth);

    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR: Framebuffer is not complete" << std::endl;
        std::cin.get();
        std::terminate();
    }

    // Unbind this framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

inline int pick(int xpos, int ypos, int frameBufferWidth, int frameBufferHeight)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, picking_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    unsigned char pixel[3];
    glReadPixels(xpos, frameBufferHeight - ypos - 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);

    int r = (int)pixel[0];
    int g = (int)pixel[1];
    int b = (int)pixel[2];

    int targetID = ((r << 16) & 0xFF0000) + ((g << 8) & 0x00FF00) + b & 0xFF;

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    return targetID;
}

inline void deletePickingResources()
{
    glDeleteTextures(1, &picking_tex);
    glDeleteRenderbuffers(1, &picking_depth);
    glDeleteFramebuffers(1, &picking_fbo);
}