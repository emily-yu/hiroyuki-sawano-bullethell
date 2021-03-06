//
//  Scene.hpp
//  SDLProject
//
//  Created by Emily Yu on 7/1/20.
//  Copyright © 2020 ctg. All rights reserved.
//

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
#include "Util.h"
#include "Entity.h"
//#include "Map.h"

struct GameState {
    Mix_Music *music;
    bool isWin = false;
    
    Entity *player;
    bool isShooting = false;
    float remainingTime = 0.0f;
    
    Entity *enemies;
    int nextScene;
    glm::mat4 backgroundMat;
    glm::vec3 backgroundPos;
    float accumulatedTime;
    
    // text accompanied with icons
    Entity *livesText;
    int lives;

    Entity *spellsText;
    int availSpells; // earned when killing enemies
    
    // straight text
    Entity *scoreText;
    float totalScore;
    
    Entity *playerPowerText;
    float powerLevel;
};
class Scene {
public:
    GameState state;
    virtual void Initialize(Scene *sceneList) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(ShaderProgram *program) = 0;
};
