//
//  Level1.cpp
//  SDLProject
//
//  Created by Emily Yu on 7/1/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Level1.h"
#include <map>
#include "BulletPattern.h"
#include "BulletEnemy.h"

#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
//#define LEVEL1_ENEMY_COUNT 1
#define LEVEL1_ENEMY_COUNT 2

BulletPattern *patternList_LEVEL1[4];
std::map<float, BulletPattern*> bulletTable;
float bulletCount_LEVEL1 = 4;
BulletEnemy *enemy_LEVEL1;
BulletEnemy *enemies[2];

BulletPattern *player_LEVEL1;
BulletPattern *playerBullet_LEVEL1;

void ConstructEnemy() {
    // construct patterns for level
    GLuint circleBulletTexture = Util::LoadTexture("circle.png");
    for (int i = 0; i < bulletCount_LEVEL1; i++) {
        patternList_LEVEL1[i] = new BulletPattern();
        patternList_LEVEL1[i]->bulletTexture = circleBulletTexture; // test bullet image
        patternList_LEVEL1[i]->speed = 0.5f;
        patternList_LEVEL1[i]->movement = glm::vec3(1, 0, 0);
        patternList_LEVEL1[i]->velocity = glm::vec3(1, 0, 0);
        patternList_LEVEL1[i]->acceleration = glm::vec3(0, -9.81f, 0);
    }
    // enemy 1's bullets
    patternList_LEVEL1[0]->waveCount = 20;
    patternList_LEVEL1[0]->patternType = Vertical;
    patternList_LEVEL1[1]->waveCount = 20;
    patternList_LEVEL1[1]->patternType = CirclePulse;
    
    // enemy 2's bullets
    patternList_LEVEL1[2]->waveCount = 20;
    patternList_LEVEL1[2]->patternType = Vertical;
    patternList_LEVEL1[3]->waveCount = 20;
    patternList_LEVEL1[3]->patternType = CirclePulse;

    // construct enemies with bulletpatterns
    GLuint enemyTexture = Util::LoadTexture("girl.png");
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        enemies[i] = new BulletEnemy();
        enemies[i]->enemyTexture = enemyTexture;
    }
    
    // enemy 1
    enemies[0]->remainingHealth = 20.0f;
    enemies[0]->position = glm::vec3(-2, 0, 0);
    enemies[0]->bulletTable = {
        { 0.2, patternList_LEVEL1[0] }, // { deltaTime, BulletPattern to be displayed }
        { 10.4, patternList_LEVEL1[1] }
    };
    enemies[0]->movementLocations[0] = glm::vec3(0.5, 0, 0); // process enemy movements
    enemies[0]->movementTiming[0] = 3.0;
    enemies[0]->movementLocations[1] = glm::vec3(2.5, -1, 0);
    enemies[0]->movementTiming[1] = 5.0;
    enemies[0]->movementCount = 2;
    enemies[0]->isActive = true;
    
    // enemy 2
    enemies[1]->remainingHealth = 20.0f;
    enemies[1]->position = glm::vec3(-3, 0, 0);
    enemies[1]->bulletTable = {
        { 0.2, patternList_LEVEL1[2] }, // { deltaTime, BulletPattern to be displayed }
        { 10.4, patternList_LEVEL1[3] }
    };
    enemies[1]->movementLocations[0] = glm::vec3(1.5, 0, 0); // process enemy movements
    enemies[1]->movementTiming[0] = 1.0;
    enemies[1]->movementLocations[1] = glm::vec3(3.5, -1, 0);
    enemies[1]->movementTiming[1] = 5.0;
    enemies[1]->movementCount = 2;
    enemies[1]->isActive = true;
}
void Level1::Initialize(Scene *sceneList) {
    
    state.music = Mix_LoadMUS("Gameplay - Boukyaku Keikoku.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME / 16); // cut volume by 1/4
    Mix_PlayMusic(state.music, -1); // Play Audio
    
    // game stats
    state.accumulatedTime = 0.0f;
    state.nextScene = -1; // main.cpp will not switch to nextscene yet, <= 0
    state.powerLevel = 4.0f;

    GLuint circleBulletTexture = Util::LoadTexture("circle.png");
    ConstructEnemy();

    // player ui
    player_LEVEL1 = new BulletPattern();
    player_LEVEL1->bulletTexture = circleBulletTexture;
    player_LEVEL1->speed = 0.5f;
    player_LEVEL1->movement = glm::vec3(1, 0, 0);
    player_LEVEL1->velocity = glm::vec3(1, 0, 0);
    player_LEVEL1->acceleration = glm::vec3(0, -9.81f, 0);
    player_LEVEL1->xPivot = -2;
    player_LEVEL1->yPivot = 0;
    player_LEVEL1->waveCount = 20;
    player_LEVEL1->patternType = Vertical;
    player_LEVEL1->isActive = true;
    playerBullet_LEVEL1 = new BulletPattern(); // additional shooting
    playerBullet_LEVEL1->bulletTexture = circleBulletTexture;
    playerBullet_LEVEL1->speed = 0.5f;
    playerBullet_LEVEL1->movement = glm::vec3(1, 0, 0);
    playerBullet_LEVEL1->velocity = glm::vec3(1, 0, 0);
    playerBullet_LEVEL1->acceleration = glm::vec3(0, -9.81f, 0);
    playerBullet_LEVEL1->xPivot = -2;
    playerBullet_LEVEL1->yPivot = 0;
    playerBullet_LEVEL1->waveCount = 20;
    playerBullet_LEVEL1->patternType = Vertical;
    playerBullet_LEVEL1->isActive = false; // for clarity
    
    state.player = new Entity();
    state.player->position = glm::vec3(0, -2, 0); // start near left of screen
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, 0.0f, 0);
    state.player->velocity = glm::vec3(0, 0.0f, 0);
    state.player->speed = 2.5f;
    state.player->textureID = Util::LoadTexture("george_0.png");
    state.player->animRight = new int[4] {3, 7, 11, 15};
    state.player->animLeft = new int[4] {1, 5, 9, 13};
    state.player->animUp = new int[4] {2, 6, 10, 14};
    state.player->animDown = new int[4] {0, 4, 8, 12};
    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    state.player->height = 0.8f; // change bounding box for sprite so isn't floating on platform
    state.player->width = 0.8f;
    state.player->jumpPower = 10.0f;
    state.player->entityType = PLAYER;
    
    // background ui
    state.backgroundPos = glm::vec3(0,0,0);
    
    // game stats ui [left]
    GLuint fontTextureID = Util::LoadTexture("sprite_texture2.png");
    state.livesText = new Entity();
    state.livesText->textureID = fontTextureID;
    state.livesText->entityType = TEXT;
    state.livesText->animIndices = NULL;
    state.livesText->writeText = "Lives: 3";
    state.livesText->position = glm::vec3(-4.5, 3.25, 0);
    state.livesText->acceleration = glm::vec3(0, 0, 0);
    
    state.spellsText = new Entity();
    state.spellsText->textureID = fontTextureID;
    state.spellsText->entityType = TEXT;
    state.spellsText->animIndices = NULL;
    state.spellsText->writeText = "Spells: 0";
    state.spellsText->position = glm::vec3(-4.5, 3.00, 0);
    state.spellsText->acceleration = glm::vec3(0, 0, 0);
    
    state.scoreText = new Entity();
    state.scoreText->textureID = fontTextureID;
    state.scoreText->entityType = TEXT;
    state.scoreText->animIndices = NULL;
    state.scoreText->writeText = "Score: 0";
    state.scoreText->position = glm::vec3(-4.5, 2.75, 0);
    state.scoreText->acceleration = glm::vec3(0, 0, 0);
    
    state.playerPowerText = new Entity();
    state.playerPowerText->textureID = fontTextureID;
    state.playerPowerText->entityType = TEXT;
    state.playerPowerText->animIndices = NULL;
    state.playerPowerText->writeText = "Power: " + std::to_string(state.powerLevel) + " / 4.0";;
    state.playerPowerText->position = glm::vec3(-4.5, 2.5, 0);
    state.playerPowerText->acceleration = glm::vec3(0, 0, 0);
}
void Level1::Update(float deltaTime) {
//    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT);

    // check if aligned with any enemies, subtract from them health
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) { // update all positions of enemies
        if (fabs(state.player->position.x - enemies[i]->position.x) <= 0.5) { // hitbox of 0.25 more on each side
            if (state.player->position.y < enemies[i]->position.y) { // in front of player, can be hit
                if (enemies[i]->remainingHealth <= 0.0f) { // enemy is killed
                    // add power level
                    state.powerLevel += 1.0f;
                    
                    std::cout << "TODO: spawn next set of boonies booney" << std::endl;
                    std::cout << "TODO: fix malloc errors when enemy is trying to be deleted" << std::endl;
                    if (enemies[i]->isActive) {
                        for (auto x : enemies[i]->bulletTable) {
                            x.second->isActive = false;
                            
//                            // clear all bulletpatterns radii inside it
//                            delete[] x.second->radii;
//                            x.second->radiiCount = 0;
                        };
                    }
                    enemies[i]->isActive = false;
                }
                else {
                    enemies[i]->remainingHealth -= deltaTime * state.powerLevel;
                    std::cout << enemies[i]->remainingHealth << std::endl;
                }
            }
        }
    }
    
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) { // update all positions of enemies
        for (auto x : enemies[i]->bulletTable) {
            x.second->CheckCollision(state.player);
        };
    }
    
    if (state.player->position.x >= 12) { // if player moves far enough past x = 12...
        state.nextScene = 1; // set nextScene to be >= 0, aka main.cpp catches that need to switch to nextScene
    }
    
    if (state.player->lastCollision == BULLET) {
        std::cout << state.lives << std::endl;
        state.lives -= 1;
        state.player->lastCollision = NONE; // subtract from lives, then reset tracker
        state.nextScene = 0;
    }
    if (state.lives <= 0) {
        // end the game
        state.player->isActive = false;
        // isOver == true
    }
    
    
    state.backgroundPos.y -= 0.25;
    state.backgroundMat = glm::mat4(1.0f); // base matrix value
//    state.backgroundMat = glm::translate(state.backgroundMat, state.backgroundPos); // translate by new position
//    program.SetModelMatrix(backgroundMat);
    
    if (state.backgroundPos.y < -4.0f) {
        state.backgroundPos.y = 4;
    }
    
//    ITERATE THROUGH BULLETPATTERN_MAPPING:
//        IF DELTATIME == MAP.FIRST (BULLET SPAWN TIME):
//            SET BULLETPATTERN (MAP.SECOND) = ACTIVE SO CAN DRAW
//
    state.accumulatedTime += deltaTime;
    for (auto x : bulletTable) {
        std::cout << x.first  // string (key)
                  << ':'
        << x.second->isActive // string's value
                  << std::endl ;
        if (state.accumulatedTime > x.first && x.second->isActive == false) {
            x.second->isActive = true;
        }
    };
    
    player_LEVEL1->Update(deltaTime, state.player->position);
    
    // shoot by pressing v
    if (state.isShooting) { // start shooting
        playerBullet_LEVEL1->isActive = true;
        state.isShooting = false;
        
        // subtract from power level + update text
        if (state.powerLevel > 0.0f) {
            state.powerLevel -= 1.0f;
            state.playerPowerText->writeText = "Power: " + std::to_string(state.powerLevel) + " / 4.0";
        }
    }
    else if (state.remainingTime > 0.0f) { // currently using shooting time
        state.remainingTime -= deltaTime;
    }
    else if (playerBullet_LEVEL1->isActive){ // done shooting need to reset
        playerBullet_LEVEL1->isActive = false;
        
        // clear all bulletpatterns radii inside it
        delete[] playerBullet_LEVEL1->radii;
        playerBullet_LEVEL1->radiiCount = 0;
        
        state.remainingTime = 0.0f;
    }
    playerBullet_LEVEL1->Update(deltaTime, state.player->position);
    
    for (int i = 0; i < bulletCount_LEVEL1; i++) {
        for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) { // update all positions of enemies
            patternList_LEVEL1[i]->Update(deltaTime, enemies[i]->position);
        }
    }
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        enemies[i]->Update(deltaTime);
    }
    
    // text updates
    state.livesText->Update(deltaTime, state.player, NULL, 0);
    state.spellsText->Update(deltaTime, state.player, NULL, 0);
    state.scoreText->Update(deltaTime, state.player, NULL, 0);
    state.playerPowerText->Update(deltaTime, state.player, NULL, 0);
}
void Level1::Render(ShaderProgram *program) {
    
    // ui tile rendering
    GLuint backgroundID = Util::LoadTexture("purpletempbackground1.png");
    Util::DrawBackground(program, backgroundID, state.backgroundMat, state.backgroundPos);
    program->SetModelMatrix(state.backgroundMat);

    state.livesText->Render(program);
    state.spellsText->Render(program);
    state.scoreText->Render(program);
    state.playerPowerText->Render(program);
    
    // render bullet bases
    for (int i = 0; i < bulletCount_LEVEL1; i++) {
        patternList_LEVEL1[i]->Render(program);
    }
    
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        enemies[i]->Render(program);
    }

    state.player->Render(program);
    player_LEVEL1->Render(program);
    playerBullet_LEVEL1->Render(program);
}
