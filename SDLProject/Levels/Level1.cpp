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
#define LEVEL1_MAX_POWERLEVEL 4
#define LEVEL1_MIN_POWERLEVEL 1

BulletPattern *patternList_LEVEL1[11];
std::map<float, BulletPattern*> bulletTable;
float bulletCount_LEVEL1 = 11;
BulletEnemy *enemy_LEVEL1;

BulletEnemy *enemies[6];
#define LEVEL1_ENEMY_COUNT 6

BulletPattern *player_LEVEL1;
BulletPattern *playerBullet_LEVEL1;

Entity *gameWinText;

Mix_Chunk *LEVEL1_Music; // pointer for main audio

void ConstructEnemy() {
    // construct patterns for level
    GLuint circleBulletTexture = Util::LoadTexture("hex.png");
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
    patternList_LEVEL1[0]->patternType = SingularSpiral;
    patternList_LEVEL1[0]->startTime = 0.5;
    patternList_LEVEL1[0]->endTime = 2.2;
//    patternList_LEVEL1[0]->endTime = 15.2;
    
    patternList_LEVEL1[1]->waveCount = 20;
    patternList_LEVEL1[1]->patternType = CirclePulse;
    patternList_LEVEL1[1]->startTime = 30.2;
    patternList_LEVEL1[1]->endTime = 45.2;
    
    // enemy 2's bullets
    patternList_LEVEL1[2]->waveCount = 20;
    patternList_LEVEL1[2]->patternType = Vertical;
    patternList_LEVEL1[2]->startTime = 0.2;
    patternList_LEVEL1[2]->endTime = 1.2;
    
    patternList_LEVEL1[3]->waveCount = 20;
    patternList_LEVEL1[3]->patternType = SingularSpiral;
    patternList_LEVEL1[3]->startTime = 8.2;
    patternList_LEVEL1[3]->endTime = 9.2;
    
    // random flow er patterns for time with enemy 2
    patternList_LEVEL1[4]->waveCount = 20;
    patternList_LEVEL1[4]->patternType = CirclePulse;
    patternList_LEVEL1[4]->startTime = 16.2;
    patternList_LEVEL1[4]->endTime = 20.2;
    
    patternList_LEVEL1[5]->waveCount = 20;
    patternList_LEVEL1[5]->patternType = CirclePulse;
    patternList_LEVEL1[5]->startTime = 18.2;
    patternList_LEVEL1[5]->endTime = 22.2;
    
    patternList_LEVEL1[6]->waveCount = 20;
    patternList_LEVEL1[6]->patternType = CirclePulse;
    patternList_LEVEL1[6]->startTime = 24.2;
    patternList_LEVEL1[6]->endTime = 32.2;
    
    // in line bois
    patternList_LEVEL1[7]->waveCount = 40;
    patternList_LEVEL1[7]->patternType = SingularSpiral;
    patternList_LEVEL1[7]->startTime = 53;
    patternList_LEVEL1[7]->endTime = 100;
    
    patternList_LEVEL1[8]->waveCount = 10;
    patternList_LEVEL1[8]->patternType = CirclePulse;
    patternList_LEVEL1[8]->startTime = 55;
    patternList_LEVEL1[8]->endTime = 100;
    
    patternList_LEVEL1[9]->waveCount = 40;
    patternList_LEVEL1[9]->patternType = SingularSpiral;
    patternList_LEVEL1[9]->startTime = 60;
    patternList_LEVEL1[9]->endTime = 100;
    
    patternList_LEVEL1[10]->waveCount = 10;
    patternList_LEVEL1[10]->patternType = CirclePulse;
    patternList_LEVEL1[10]->startTime = 65;
    patternList_LEVEL1[10]->endTime = 100;
    
    // construct enemies with bulletpatterns
    GLuint enemyTexture = Util::LoadTexture("girl.png");
    enemies[0] = new BulletEnemy(5, 2); // movementCount, bulletCount
    enemies[1] = new BulletEnemy(8, 5);
    enemies[2] = new BulletEnemy(5, 1);
    enemies[3] = new BulletEnemy(5, 1);
    enemies[4] = new BulletEnemy(5, 1);
    enemies[5] = new BulletEnemy(5, 1);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        enemies[i]->enemyTexture = enemyTexture;
    }
    
    // enemy 1
    enemies[0]->remainingHealth = 5.0f;
    enemies[0]->position = glm::vec3(1.5, 2, 0);
    enemies[0]->bulletTable = {
        { 0.2, patternList_LEVEL1[0] }, // { deltaTime, BulletPattern to be displayed }
        { 10.4, patternList_LEVEL1[1] }
    };
    // stationary for the first 15 seconds with spiral
    // move side to side for the next 15 seconds with sprial
    enemies[0]->movementLocations[0] = glm::vec3(1.5, 2, 0); // process enemy movements
    enemies[0]->movementTiming[0] = 15.0;
    enemies[0]->movementLocations[1] = glm::vec3(-1.5, 2, 0);
    enemies[0]->movementTiming[1] = 23.0;
    enemies[0]->movementLocations[2] = glm::vec3(1.5, 2, 0);
    enemies[0]->movementTiming[2] = 25.0;
    enemies[0]->movementLocations[3] = glm::vec3(-1.5, 2, 0);
    enemies[0]->movementTiming[3] = 27.0;
    enemies[0]->movementLocations[4] = glm::vec3(1.5, 2, 0);
    enemies[0]->movementTiming[4] = 30.0;
    // circular: 7.2 to 14.2 - stationary and pulses of singular from random places

    enemies[0]->movementCount = 5;
    enemies[0]->isActiveStart = 0.0;
    enemies[0]->isActiveEnd = 45.2;
//    enemies[0]->isActive = true;
    
    // enemy 2
    enemies[1]->remainingHealth = 5.0f;
    enemies[1]->position = glm::vec3(-3, 0, 0);
    enemies[1]->bulletTable = {
        { 0.2, patternList_LEVEL1[2] }, // { deltaTime, BulletPattern to be displayed }
        { 10.4, patternList_LEVEL1[3] },
        { 18.2, patternList_LEVEL1[4] },
        { 22.2, patternList_LEVEL1[5] },
        { 24.2, patternList_LEVEL1[6] }
    };
    enemies[1]->movementLocations[0] = glm::vec3(1.5, 0.5, 0); // process enemy movements
    enemies[1]->movementTiming[0] = 11.0;
    enemies[1]->movementLocations[1] = glm::vec3(3.5, 1, 0);
    enemies[1]->movementTiming[1] = 12.0;
    enemies[1]->movementLocations[2] = glm::vec3(4.5, 1, 0);
    enemies[1]->movementTiming[2] = 14.0;
    enemies[1]->movementLocations[3] = glm::vec3(6.5, 0.5, 0); // process enemy movements
    enemies[1]->movementTiming[3] = 15.0;
    
    // for flowers
    enemies[1]->movementLocations[4] = glm::vec3(6.5, 0.5, 0); // process enemy movements
    enemies[1]->movementTiming[4] = 16.0;
    enemies[1]->movementLocations[5] = glm::vec3(4.5, -0.5, 0); // process enemy movements
    enemies[1]->movementTiming[5] = 20.0;
    enemies[1]->movementLocations[6] = glm::vec3(-0.5, -0.5, 0); // process enemy movements
    enemies[1]->movementTiming[6] = 24.0;
    enemies[1]->movementLocations[7] = glm::vec3(-2.5, 0.5, 0); // process enemy movements
    enemies[1]->movementTiming[7] = 28.0;
    
    enemies[1]->movementCount = 8;
    enemies[1]->isActiveStart = 11.0;
    enemies[1]->isActiveEnd = 45.0;
//    enemies[1]->isActive = true;
    
    // enemy 3
    enemies[2]->remainingHealth = 5.0f;
    enemies[2]->position = glm::vec3(-6.5, 2, 0);
    enemies[2]->bulletTable = {
        { 0.0, patternList_LEVEL1[7] }
    };
    // stationary for the first 15 seconds with spiral
    // move side to side for the next 15 seconds with sprial
    enemies[2]->movementLocations[0] = glm::vec3(1.5, -1, 0); // process enemy movements
    enemies[2]->movementTiming[0] = 50.0;
    enemies[2]->movementLocations[1] = glm::vec3(1.5, -1, 0);
    enemies[2]->movementTiming[1] = 53.0;
    enemies[2]->movementLocations[2] = glm::vec3(1.5, -1, 0);
    enemies[2]->movementTiming[2] = 55.0;
    enemies[2]->movementLocations[3] = glm::vec3(1.5, -1, 0);
    enemies[2]->movementTiming[3] = 57.0;
    enemies[2]->movementLocations[4] = glm::vec3(1.5, -1, 0);
    enemies[2]->movementTiming[4] = 60.0;
    // circular: 7.2 to 14.2 - stationary and pulses of singular from random places

    enemies[2]->movementCount = 5;
    enemies[2]->isActiveStart = 50.0;
    enemies[2]->isActiveEnd = 100.2;
    
    // enemy 4
    enemies[3]->remainingHealth = 5.0f;
    enemies[3]->position = glm::vec3(-6.5, 2, 0);
    enemies[3]->bulletTable = {
        { 0.0, patternList_LEVEL1[8] }
    };
    enemies[3]->movementLocations[0] = glm::vec3(1.5, -1, 0); // process enemy movements
    enemies[3]->movementTiming[0] = 55.0;
    enemies[3]->movementLocations[1] = glm::vec3(1.5, -1, 0);
    enemies[3]->movementTiming[1] = 58.0;
    enemies[3]->movementLocations[2] = glm::vec3(1.5, -1, 0);
    enemies[3]->movementTiming[2] = 61.0;
    enemies[3]->movementLocations[3] = glm::vec3(1.5, -1, 0);
    enemies[3]->movementTiming[3] = 64.0;
    enemies[3]->movementLocations[4] = glm::vec3(1.5, -1, 0);
    enemies[3]->movementTiming[4] = 67.0;
    enemies[3]->movementCount = 5;
    enemies[3]->isActiveStart = 55.0;
    enemies[3]->isActiveEnd = 100.2;
    
    // enemy 5
    enemies[4]->remainingHealth = 5.0f;
    enemies[4]->position = glm::vec3(-6.5, 2, 0);
    enemies[4]->bulletTable = {{ 0.0, patternList_LEVEL1[9] }};
    enemies[4]->movementLocations[0] = glm::vec3(1.5, -1, 0); // process enemy movements
    enemies[4]->movementTiming[0] = 60.0;
    enemies[4]->movementLocations[1] = glm::vec3(1.5, -1, 0);
    enemies[4]->movementTiming[1] = 63.0;
    enemies[4]->movementLocations[2] = glm::vec3(1.5, -1, 0);
    enemies[4]->movementTiming[2] = 66.0;
    enemies[4]->movementLocations[3] = glm::vec3(1.5, -1, 0);
    enemies[4]->movementTiming[3] = 69.0;
    enemies[4]->movementLocations[4] = glm::vec3(1.5, -1, 0);
    enemies[4]->movementTiming[4] = 72.0;
    enemies[4]->movementCount = 5;
    enemies[4]->isActiveStart = 60.0;
    enemies[4]->isActiveEnd = 100.2;
    
    // enemy 6
    enemies[5]->remainingHealth = 5.0f;
    enemies[5]->position = glm::vec3(-6.5, 2, 0);
    enemies[5]->bulletTable = {{ 0.0, patternList_LEVEL1[10] }};
    enemies[5]->movementLocations[0] = glm::vec3(1.5, -1, 0); // process enemy movements
    enemies[5]->movementTiming[0] = 65.0;
    enemies[5]->movementLocations[1] = glm::vec3(1.5, -1, 0);
    enemies[5]->movementTiming[1] = 68.0;
    enemies[5]->movementLocations[2] = glm::vec3(1.5, -1, 0);
    enemies[5]->movementTiming[2] = 71.0;
    enemies[5]->movementLocations[3] = glm::vec3(1.5, -1, 0);
    enemies[5]->movementTiming[3] = 74.0;
    enemies[5]->movementLocations[4] = glm::vec3(1.5, -1, 0);
    enemies[5]->movementTiming[4] = 77.0;
    enemies[5]->movementCount = 5;
    enemies[5]->isActiveStart = 65.0;
    enemies[5]->isActiveEnd = 100.2;
}
void Level1::Initialize(Scene *sceneList) {
    LEVEL1_Music = Mix_LoadWAV("bullet_shoot.wav");
    state.music = Mix_LoadMUS("Gameplay - Boukyaku Keikoku.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME); // cut volume by 1/4
    Mix_PlayMusic(state.music, -1); // Play Audio
    
    // game stats
    state.accumulatedTime = 0.0f;
    state.nextScene = -1; // main.cpp will not switch to nextscene yet, <= 0
    state.powerLevel = 1.0f;

    GLuint circleBulletTexture = Util::LoadTexture("circle.png");
    ConstructEnemy();

    // player ui
    GLuint circleBulletTexture2 = Util::LoadTexture("circle.png");
    player_LEVEL1 = new BulletPattern();
    player_LEVEL1->bulletTexture = circleBulletTexture2;
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
    playerBullet_LEVEL1->bulletTexture = circleBulletTexture2;
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
    
    // reimu credit
//    https://tenor.com/view/touhou-anime-girl-dancing-smiling-gif-15035422
    state.player->textureID = Util::LoadTexture("reimu-sprite.png");
//    state.player->animRight = new int[4] {3, 7, 11, 15};
//    state.player->animLeft = new int[4] {1, 5, 9, 13};
//    state.player->animUp = new int[4] {2, 6, 10, 14};
//    state.player->animDown = new int[4] {0, 4, 8, 12};
//    state.player->animIndices = state.player->animRight;
    state.player->animIndices = NULL;
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
    state.livesText->writeText = "Lives: " + std::to_string(state.lives);
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
    state.scoreText->writeText = "Score: " + std::to_string(state.totalScore);
    state.scoreText->position = glm::vec3(-4.5, 2.75, 0);
    state.scoreText->acceleration = glm::vec3(0, 0, 0);
    
    state.playerPowerText = new Entity();
    state.playerPowerText->textureID = fontTextureID;
    state.playerPowerText->entityType = TEXT;
    state.playerPowerText->animIndices = NULL;
    state.playerPowerText->writeText = "Power: " + std::to_string(state.powerLevel) + " / " + std::to_string(LEVEL1_MAX_POWERLEVEL);
    state.playerPowerText->position = glm::vec3(-4.5, 2.5, 0);
    state.playerPowerText->acceleration = glm::vec3(0, 0, 0);
    
    gameWinText = new Entity();
    gameWinText->textureID = fontTextureID;
    gameWinText->entityType = TEXT;
    gameWinText->animIndices = NULL;
    gameWinText->writeText = "You Win, screen will reset in 30 seconds.";
    gameWinText->position = glm::vec3(2, 0, 0);
    gameWinText->acceleration = glm::vec3(0, 0, 0);
    gameWinText->isActive = false;
}
void Level1::Update(float deltaTime) {
//    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT);

    // check if aligned with any enemies, subtract from them health
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) { // update all positions of enemies
        if (fabs(state.player->position.x - enemies[i]->position.x) <= 0.5) { // hitbox of 0.25 more on each side
            if (state.player->position.y < enemies[i]->position.y) { // in front of player, can be hit
                if (enemies[i]->remainingHealth == 0.0f && enemies[i]->isActive) { // enemy is killed and active
                    // add power level
//                    if (state.powerLevel < LEVEL1_MAX_POWERLEVEL) {
                    if (state.powerLevel < 4) { // TODO: this part doesn't work for some reason??????
                        state.powerLevel += 1.0f;
                        state.playerPowerText->writeText = "Power: " + std::to_string(state.powerLevel) + " / " + std::to_string(LEVEL1_MAX_POWERLEVEL);
                        state.playerPowerText->Update(deltaTime, state.player, NULL, 0);
                    }

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
                else if (enemies[i]->isActive) { // only bonus if enemy is alive
                    enemies[i]->remainingHealth -= deltaTime * state.powerLevel;
                    std::cout << enemies[i]->remainingHealth << std::endl;
                    
                    // add bonus points for hitting
                    state.totalScore += 2.0f;
                    state.scoreText->writeText = "Score: " + std::to_string(state.totalScore);
                    state.scoreText->Update(deltaTime, state.player, NULL, 0);
                }
            }
        }
    }

    bool isKilled = false;
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) { // update all positions of enemies
        if (isKilled == false) { // if not already killed
            for (auto x : enemies[i]->bulletTable) { // check if player has been killed
                x.second->CheckCollision(state.player);
            };
        }
    }
    
//    if (state.player->position.x >= 12) { // if player moves far enough past x = 12...
//        state.nextScene = 1; // set nextScene to be >= 0, aka main.cpp catches that need to switch to nextScene
//    }
    
    // for debugging
    std::cout << state.accumulatedTime << std::endl;
    if (state.accumulatedTime <= 15.0) {
        std::cout << "still spiral" << std::endl;
    }
    else if (state.accumulatedTime > 15.0 && state.accumulatedTime <= 30.0) {
        std::cout << "moving spiral" << std::endl;
    }
    else if (state.accumulatedTime > 30.0 && state.accumulatedTime <= 40.0) {
        std::cout << "still circle" << std::endl;
    }
    
    // 157 is 2 min, 195 is 2:30 min
    if (state.accumulatedTime >= 157 && state.accumulatedTime <= 195) { // 2 minutes passed
        gameWinText->isActive = true;
    }
    else if (state.accumulatedTime >= 200) {
        state.nextScene = 0;
    }
    
    if (state.player->lastCollision == BULLET) {
        std::cout << state.lives << std::endl;
        state.lives -= 1;
        state.livesText->writeText = "Lives: " + std::to_string(state.lives);

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
        Mix_PlayChannel(-1, LEVEL1_Music, 0); // play effect

        // subtract from power level + update text
        if (state.powerLevel > LEVEL1_MIN_POWERLEVEL) {
            state.powerLevel -= 1.0f;
            state.playerPowerText->writeText = "Power: " + std::to_string(state.powerLevel) + " / " + std::to_string(LEVEL1_MAX_POWERLEVEL);
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
    
    // update score - time
    state.totalScore += deltaTime;
    state.scoreText->writeText = "Score: " + std::to_string(state.totalScore);

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
    gameWinText->Render(program);
    
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
