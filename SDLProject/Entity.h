//
//  Entity.hpp
//  SDLProject
//
//  Created by Emily Yu on 6/15/20.
//  Copyright © 2020 ctg. All rights reserved.
//

//#ifndef Entity_hpp
//#define Entity_hpp
#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
//#include "Map.h"

#include <SDL_mixer.h>
#include <stdio.h>
#include <vector>

enum EntityType {
    PLAYER,
    PLATFORM,
    COIN,
    ENEMY,
    TEXT,
    BULLET, // assigned internally in BulletPattern.cpp
    NONE
};

/*
 ====== 3 enemy definitions ======
 jumper definition [new]
 (1) kill when jumped on, when not jumping AIState = IDLE
 (2) kills player if touch player while jumping AIState = JUMPING
 
 walker definition
 (1) kill when jumped on @ AIState = Any
 (2) kills player if touch player while walk left and right AIState = WALKING
 
 waitandgo definition
 (1) kill when jumped on @ AIState = Any
 (2) kills player if touch player when move when player is close enough AISate = ATTACKING
*/
enum AIType { WALKER, WAITANDGO, JUMPER };
enum AIState { IDLE, WALKING, ATTACKING, JUMPING };

class Entity {
public:
    EntityType entityType;
    EntityType lastCollision;
    std::string writeText;
    int lives;
        
    // ai
    AIType aiType;
    AIState aiState;
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    
    float width = 0.5; // dimensions of Entity
    float height = 0.5;
    
    bool jump = false;
    float jumpPower = 0;
//    Mix_Chunk *jumpEffect; // pointer for audio chunk (sound effect)
    
    float speed;
    GLuint textureID;

    glm::mat4 modelMatrix;
    
    // frames to choose
    int *animRight = NULL; // pointer to right animation frames
    int *animLeft = NULL; // pointer to left animation frames
    int *animUp = NULL;
    int *animDown = NULL;
    int *animIndices = NULL; // set default to right animation frames

    int animFrames = 4;
    int animIndex = 0; // which frame animation is at in animIndices
    
    float animTime = 0;

    int animCols = 0;
    int animRows = 0;
    
    bool isActive = true;
    bool collidedTop = false; // collision flags
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    
    Entity();

    bool CheckCollision(Entity *other);
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void Update(float deltaTime, Entity* player, Entity *objects, int objectCount);

    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position);
    
    Entity* CheckCollisionEntity(Entity *other);
};

//#endif /* Entity_hpp */


