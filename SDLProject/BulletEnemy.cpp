//
//  BulletEnemy.cpp
//  SDLProject
//
//  Created by Emily Yu on 7/21/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "BulletEnemy.h"
// TODO: correct into more standard/clean class code... https://www.cs.fsu.edu/~myers/cop3330/examples/phonebook/directory.cpp
BulletEnemy::BulletEnemy(int movementCount, int bulletCount) {
//    jumpEffect = Mix_LoadWAV("bullet_shoot.wav");
    modelMatrix = glm::mat4(1.0f);
    pivotMatrix = glm::mat4(1.0f);
    position = pivot;
    
    patternList = new BulletPattern[bulletCount];
    movementLocations = new glm::vec3[movementCount];
    movementTiming = new float[movementCount];
}
BulletEnemy::~BulletEnemy() {
    delete [] patternList;
    delete [] movementLocations;
    delete [] movementTiming;
}
float* BulletEnemy::changeSize(float *old, size_t old_size, size_t new_size) {
   // Create a new larger array.
   float *new_array = new float[new_size];
   // Copy the old array to the new array.
   memcpy(new_array, old, old_size*sizeof(float));
   delete[] old;
   // Just populate the array for demonstration.
//   for(size_t i = old_size; i < new_size; ++i)
//       new_array[i] = (i+1) * 10.0;
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
    if (isActive == false) return;
    
    // draw single sprite
    program->SetModelMatrix(pivotMatrix);
    DrawSpriteFromTextureAtlas(program, enemyTexture, position);
    
    program->SetModelMatrix(modelMatrix);
    for (int i = 0; i < bulletCount; i++) {
        patternList[i].Render(program);
    }
}
void BulletEnemy::Move(glm::vec3 newPosition, float atTime) {
    // movement times cannot have less than 0.25s between them
    if (accumulatedTime > atTime && accumulatedTime < atTime + 0.25) {
        std::cout << "moved at " << atTime << std::endl;
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
    std::cout << "enemy accumulated time " << accumulatedTime << std::endl;

    // SPRITE activation
    if (accumulatedTime > isActiveStart && accumulatedTime < isActiveEnd && !isActive) { // if in correct range & SPRITE is not active
        isActive = true;
    }
    else if (accumulatedTime > isActiveEnd && isActive) { // need to remove if still active
        isActive = false;
    }
        
    // make BULLETS active if not already active
    for (auto x : bulletTable) {
        if (accumulatedTime > x.second->startTime && accumulatedTime < x.second->endTime ) { // if in correct range & not active
            x.second->isActive = true; // shoot bullets
        }
        else if (x.second->isActive && (accumulatedTime < x.second->startTime || accumulatedTime > x.second->endTime)) { // if out of correct range & active
            x.second->isActive = false; // stop shooting bullets
            
            // delete all bullets on screen
            delete [] x.second->radii;
            x.second->radiiCount = 0;
        }
        else if (remainingHealth <= 0.0f) { // if dead
            isActive = false;
            x.second->isActive = false; // stop shooting bullets
            
            // delete any bullets that are on the screen
            if (x.second->radiiCount > 0) {
                delete [] x.second->radii;
                x.second->radiiCount = 0;
            }
        }
        // else: if not active
        x.second->Update(deltaTime, position);
    };
    
    for (int i = 0; i < movementCount; i++) {
        Move(movementLocations[i], movementTiming[i]); // select the correct movement to execute
    }
    
    for (int i = 0; i < bulletCount; i++) {
        // update with change in position optional
        patternList[i].Update(deltaTime, position);
    }
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    pivotMatrix = glm::mat4(1.0f);
    pivotMatrix = glm::translate(pivotMatrix, position);
}
