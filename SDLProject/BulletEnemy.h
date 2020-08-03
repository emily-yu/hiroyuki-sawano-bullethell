//
//  BulletEnemy.hpp
//  SDLProject
//
//  Created by Emily Yu on 7/21/20.
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
#include "BulletPattern.h"
#include <SDL_mixer.h>
#include <map>

class BulletEnemy {
public:
    EntityType entityType = ENEMY;
    bool isActive = false;
    float remainingHealth;
    glm::vec3 pivot;
    GLuint enemyTexture;
    
    Mix_Chunk *jumpEffect; // pointer for audio chunk (sound effect)
    
    glm::mat4 modelMatrix;
    glm::mat4 pivotMatrix;

    // bullet pattern pointer
    int bulletCount = 0;
    BulletPattern *patternList[2];
    std::map<float, BulletPattern*> bulletTable;

    float accumulatedTime = 0;
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float speed;
    
    float* changeSize(float *old, size_t old_size, size_t new_size);
    void addPattern(BulletPattern *pattern);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, glm::vec3 position);
    void DrawPivotFromTextureAtlas(ShaderProgram *program, GLuint textureID, glm::vec3 position);
    void Render(ShaderProgram *program);
    void Update(float deltaTime);
    void Move(glm::vec3 newPosition, float atTime);
    
    BulletEnemy();
};
