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
#include <SDL_mixer.h>

enum BulletPatternType {
    CirclePulse,
    SingularSpiral
};
class BulletPattern {
public:
//    static GLuint LoadTexture(const char* filePath);
//    static void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position);
    bool isActive = true;
    
    GLuint bulletTexture;
    Mix_Chunk *jumpEffect; // pointer for audio chunk (sound effect)
    glm::mat4 modelMatrix;
    BulletPatternType patternType;
    
    float waveCount; // number per circle
    float waveTime; // time between waves
    float baseRadius;
//    glm::vec3 pivot;
    float xPivot;
    float yPivot;
    
    float *radii; // pointer to array of radiuses for bulletSpawn
    int radiiCount;
    int maxRadii;
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float speed;
    
    float* changeSize(float *old, size_t old_size, size_t new_size);
    void addWave(float radii, int size);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, glm::vec3 position);
    void Render(ShaderProgram *program);
    void Update(float deltaTime);
    
    BulletPattern();
};
