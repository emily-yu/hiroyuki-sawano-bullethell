//
//  BulletEnemy.cpp
//  SDLProject
//
//  Created by Emily Yu on 7/21/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "BulletEnemy.h"
BulletEnemy::BulletEnemy() {
    jumpEffect = Mix_LoadWAV("bounce.wav");
    modelMatrix = glm::mat4(1.0f);
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

void BulletEnemy::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, glm::vec3 position) {
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
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    
    // set new dimensions for orange
    program->SetModelMatrix(modelMatrix);
    glUseProgram(program->programID);
}

void BulletEnemy::Render(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);
    
    // draw single sprite
    DrawSpriteFromTextureAtlas(program, enemyTexture, pivot);

    for (int i = 0; i < bulletCount; i++) {
        patternList[i]->Render(program);
    }
}
void BulletEnemy::Update(float deltaTime) {
    accumulatedTime += deltaTime;
    for (auto x : bulletTable) {
//        std::cout << x.first  // string (key)
//                  << ':'
//        << x.second->isActive // string's value
//                  << std::endl ;
        if (accumulatedTime > x.first && x.second->isActive == false) {
            x.second->isActive = true;
        }
        x.second->Update(deltaTime);
    };
    for (int i = 0; i < bulletCount; i++) {
        patternList[i]->Update(deltaTime);
    }
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}
