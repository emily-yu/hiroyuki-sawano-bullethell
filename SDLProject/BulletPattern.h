//
//  BulletPattern.hpp
//  SDLProject
//
//  Created by Emily Yu on 7/5/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#pragma once
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
//#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

class BulletPattern {
public:
//    static GLuint LoadTexture(const char* filePath);
//    static void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position);
    GLuint bulletTexture;
    glm::mat4 modelMatrix;
    
    float waveCount; // number per circle
//    glm::vec3 pivot;
    float xPivot;
    float yPivot;
    
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, glm::vec3 position);
    void Render(ShaderProgram *program);
};

