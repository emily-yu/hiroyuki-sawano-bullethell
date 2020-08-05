//
//  BulletEnemy.cpp
//  SDLProject
//
//  Created by Emily Yu on 7/21/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "BulletEnemy.h"
BulletEnemy::BulletEnemy() {
    jumpEffect = Mix_LoadWAV("bullet_shoot.wav");
    modelMatrix = glm::mat4(1.0f);
    pivotMatrix = glm::mat4(1.0f);
    position = pivot;
}

float* BulletEnemy::changeSize(float *old, size_t old_size, size_t new_size) {
   // Create a new larger array.
   float *new_array = new float[new_size];
   // Copy the old array to the new array.
   memcpy(new_array, old, old_size*sizeof(float));
   delete[] old;
   // Just populate the array for demonstration.
   for(size_t i = old_size; i < new_size; ++i)
       new_array[i] = (i+1) * 10.0;
   return(new_array);
}

void addPattern(BulletPattern *pattern);
void BulletEnemy::DrawPivotFromTextureAtlas(ShaderProgram *program, GLuint textureID, glm::vec3 position) {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    // draw
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
    // move orange paddle to other side of the screen
    pivotMatrix = glm::mat4(1.0f);
    pivotMatrix = glm::translate(pivotMatrix, position);
    
    // set new dimensions for orange
    program->SetModelMatrix(pivotMatrix);
    glUseProgram(program->programID);
}
void BulletEnemy::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, glm::vec3 position) {
//    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float vertices[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    // draw
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
    // move orange paddle to other side of the screen
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    
    // set new dimensions for orange
    program->SetModelMatrix(modelMatrix);
    glUseProgram(program->programID);
}

void BulletEnemy::Render(ShaderProgram *program) {
    // draw single sprite
    program->SetModelMatrix(pivotMatrix);
    DrawSpriteFromTextureAtlas(program, enemyTexture, position);
    
    program->SetModelMatrix(modelMatrix);
    for (int i = 0; i < bulletCount; i++) {
        patternList[i]->Render(program);
    }
}
void BulletEnemy::Move(glm::vec3 newPosition, float atTime) {
    // movement times cannot have less than 0.25s between them
    if (accumulatedTime > atTime && accumulatedTime < atTime + 0.25) {
        // for slight shaking in enemy
        if (position.x != newPosition.x) {
            if (position.x >= newPosition.x) {
                position.x -= 0.1;
            }
            else {
                position.x += 0.1;
            }
        }
        if (position.y != newPosition.y) {
            if (position.y >= newPosition.y) {
                position.y -= 0.1;
            }
            else {
                position.y += 0.1;
            }
        }
    }
}
void BulletEnemy::Update(float deltaTime) {
    accumulatedTime += deltaTime;
    
    // make active if not already active
    for (auto x : bulletTable) {
        if (accumulatedTime > x.first && x.second->isActive == false) {
            x.second->isActive = true;
        }
        x.second->Update(deltaTime, position);
    };
    
    for (int i = 0; i < movementCount; i++) {
        Move(movementLocations[i], movementTiming[i]); // select the correct movement to execute
    }
    
    for (int i = 0; i < bulletCount; i++) {
        // update with change in position optional
        patternList[i]->Update(deltaTime, position);
    }
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    pivotMatrix = glm::mat4(1.0f);
    pivotMatrix = glm::translate(pivotMatrix, position);
}
