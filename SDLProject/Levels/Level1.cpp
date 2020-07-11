//
//  Level1.cpp
//  SDLProject
//
//  Created by Emily Yu on 7/1/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Level1.h"
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 1

//glm::mat4 backgroundMat;
//glm::vec3 backgroundPos;
unsigned int level1_data[] = {
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level1::Initialize(Scene *sceneList) {
    state.nextScene = -1; // main.cpp will not switch to nextscene yet, <= 0

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
//    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);

    // Initialize Player
    state.player = new Entity();
    state.player->position = glm::vec3(5, -2, 0); // start near left of screen
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, 0.0f, 0);
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

    // construct a singular enemy
//    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
//    GLuint enemyTextureID = Util::LoadTexture("circle.png");
//
//    state.enemies[0].entityType = ENEMY;
//    state.enemies[0].textureID = enemyTextureID;
//    state.enemies[0].position = glm::vec3(2, -5, 0);
//    state.enemies[0].speed = 0.5;
//    state.enemies[0].aiType = WALKER;
//    state.enemies[0].aiState = IDLE;
    
    state.backgroundPos = glm::vec3(0,0,0);
}
void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    
//    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) { // update all positions of enemies
//        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
//    }
    
    if (state.player->position.x >= 12) { // if player moves far enough past x = 12...
        state.nextScene = 1; // set nextScene to be >= 0, aka main.cpp catches that need to switch to nextScene
    }
    
//    if (state.player->lastCollision == ENEMY) {
//        std::cout << state.lives << std::endl;
//        state.lives -= 1;
//        state.player->lastCollision = NONE; // subtract from lives, then reset tracker
//        state.nextScene = 0;
//    }
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
}
void Level1::Render(ShaderProgram *program) {
//    state.map->Render(program); // render tiles
    GLuint backgroundID = Util::LoadTexture("purpletempbackground1.png");
    Util::DrawBackground(program, backgroundID, state.backgroundMat, state.backgroundPos);
    program->SetModelMatrix(state.backgroundMat);

    // move to update @ deltaTime = x
//    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) { // update all positions of enemies
//        state.enemies[i].Render(program);
//    }
    
    state.player->Render(program);
}
