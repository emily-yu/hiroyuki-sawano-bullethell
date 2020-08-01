//
//  BulletPattern.cpp
//  SDLProject
//
//  Created by Emily Yu on 7/5/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "BulletPattern.h"

BulletPattern::BulletPattern() {
    position = glm::vec3(xPivot, yPivot, 0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    baseRadius = 0.5f;
    radii = new float[1];
    radiiCount = 0;
    
    waveTime = 0.0f;
    jumpEffect = Mix_LoadWAV("bounce.wav");

    modelMatrix = glm::mat4(1.0f);
}

void BulletPattern::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, glm::vec3 position) {
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
float* BulletPattern::changeSize(float *old, size_t old_size, size_t new_size) {
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
void BulletPattern::addWave(float radii_input, int size) {
    int new_size = size + 1;
    
    // Change the array size, add elements.
    radii = changeSize(radii, size, new_size);

    // Print the new array.
//    std::cout << "new [";
    for (size_t i = 0; i < new_size; ++i) {
        if (i == new_size - 1) {
            // add new input to radii list
            radii[i] = radii_input;
        }
//        std::cout << radii[i] << ", ";
    }
//    std::cout << "]" << std::endl;
}
//bool secondWave = false;
void BulletPattern::Render(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);

    for (int iwave = 0; iwave < int(radiiCount); iwave++) {
        if (patternType == CirclePulse) {
            for (int i = 0; i <= waveCount; i++) {
                float radius = radii[iwave];
                
                // slice * slice #, degree increment = angle
                float theta = 2.0f * 3.14 * float(i) / float(waveCount);//get the current angle

                // translate pivotPosition around 1 by 1 box
                float xPosition = radius * cosf(theta); // centerX + radius * cos(angle)
                float yPosition = radius * sinf(theta);

                DrawSpriteFromTextureAtlas(program, bulletTexture, glm::vec3(xPosition + xPivot, yPosition + yPivot, 0));
            }
        }
        else if (patternType == SingularSpiral) {
            float radius = radii[iwave];
            
            // slice * slice #, degree increment = angle
            float theta = 2.0f * 3.14 * float(iwave) / float(waveCount);//get the current angle

            // translate pivotPosition around 1 by 1 box
            float xPosition = radius * cosf(theta); // centerX + radius * cos(angle)
            float yPosition = radius * sinf(theta);

            DrawSpriteFromTextureAtlas(program, bulletTexture, glm::vec3(xPosition + xPivot, yPosition + yPivot, 0));
        }
        else if (patternType == Vertical) { // weird vertical cone pattern
            float radius = radii[iwave];
//            std::cout << radius << ", ";
            
            // waveCount - number of bullets in row
            float horizontalLength = 1.0f;
            // place this wave contents horizontally
            /*
            if odd, offset by 1:
                (number - 1) placed at center
                waveCount = waveCount - 1;
            */
            if (remainder(waveCount, 2) != 0) {
                DrawSpriteFromTextureAtlas(program, bulletTexture, glm::vec3(xPivot, yPivot, 0));
            }
            
            /*
            for (0 to waveCount / 2):
                // (horizontalLength / waveCount) * i is the difference to the right
                to right:
                    position = xPivot + (horizontalLength / waveCount) * i
                to left
                    position = xPivot - (horizontalLength / waveCount) * i
            */
            
            for (int i = 0; i < (waveCount - 1) / 2; i++) {
                float yPosition = radius * i;
                float horizontalOffset = (horizontalLength / waveCount) * i;
                DrawSpriteFromTextureAtlas(program, bulletTexture, glm::vec3(xPivot + horizontalOffset, yPosition + yPivot, 0));
                DrawSpriteFromTextureAtlas(program, bulletTexture, glm::vec3(xPivot - horizontalOffset, yPosition + yPivot, 0));
            }
        }
    }

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
void BulletPattern::Update(float deltaTime, glm::vec3 positionChange) {
    if (isActive == false) return; // if not active, not rendered -> no updates made to rendering
    xPivot = positionChange.x;
    yPivot = positionChange.y;
    
    // hardcode first two updates for waves
    float change = deltaTime * speed;
    for (int iwave = 0; iwave < int(radiiCount); iwave++) {
        radii[iwave] += change;
    }
    
    // add new wave if time is right
    waveTime += deltaTime;
    if (waveTime >= 1.0) { // after 0.25 seconds...
        waveTime = 0.0f; // reset timer counter
        Mix_PlayChannel(-1, jumpEffect, 0); // bounce effect
        
        // new wave addition
        radiiCount += 1;
        addWave(baseRadius, radiiCount);
    }
    
    // physics logic
    velocity.x = movement.x * speed; // instant velocity when start moving
    velocity += acceleration * deltaTime; // increase acceleration each second

    velocity += acceleration * deltaTime;

    // set up velocity
    position.y += velocity.y * deltaTime;
    position.x += velocity.x * deltaTime;

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}
void BulletPattern::CheckCollision(Entity *other) {
    for (int iwave = 0; iwave < int(radiiCount); iwave++) {
        if (patternType == CirclePulse) {
            for (int i = 0; i <= waveCount; i++) {
                    float radius = radii[iwave];
                    
            //         2 * pi / #points = slice
                    // slice * slice #, degree increment = angle
                    float theta = 2.0f * 3.14 * float(i) / float(waveCount);//get the current angle

                    // translate pivotPosition around 1 by 1 box
                    float xPosition = radius * cosf(theta); // centerX + radius * cos(angle)
                    float yPosition = radius * sinf(theta);
                    glm::vec3 bulletPosition = glm::vec3(xPosition + xPivot, yPosition + yPivot, 0);
                    
                    // if either is not active, then there won't be a collision
                    if (isActive == true && other->isActive == true) {
                        int width = 0.5;
                        int height = 0.5;
                        float xdist = fabs(bulletPosition.x - other->position.x) - ((width + other->width) / 2.0f);
                        float ydist = fabs(bulletPosition.y - other->position.y) - ((height + other->height) / 2.0f);
                        
                        if (xdist < 0 && ydist < 0) {
                            if (other->entityType == PLAYER) {
                                other->lastCollision = BULLET;
                            }
                            // text, pulse animations, power bullets aren't effected by bullets
                        }
                    }
                }
        }
        else if (patternType == SingularSpiral) {
            int iterations = 0;
            if (patternType == SingularSpiral) iterations = 1;
            if (patternType == CirclePulse) iterations = waveCount;
            
            for (int i = 0; i <= waveCount; i++) {
                float radius = radii[iwave];
                
        //         2 * pi / #points = slice
                // slice * slice #, degree increment = angle
                float theta = 2.0f * 3.14 * float(iwave) / float(waveCount);//get the current angle

                // translate pivotPosition around 1 by 1 box
                float xPosition = radius * cosf(theta); // centerX + radius * cos(angle)
                float yPosition = radius * sinf(theta);
                
                glm::vec3 bulletPosition = glm::vec3(xPosition + xPivot, yPosition + yPivot, 0);
                
                // if either is not active, then there won't be a collision
                if (isActive == true && other->isActive == true) {
                    int width = 0.5;
                    int height = 0.5;
                    float xdist = fabs(bulletPosition.x - other->position.x) - ((width + other->width) / 2.0f);
                    float ydist = fabs(bulletPosition.y - other->position.y) - ((height + other->height) / 2.0f);
                    
                    if (xdist < 0 && ydist < 0) {
                        if (other->entityType == PLAYER) {
                            other->lastCollision = BULLET;
                        }
                        // text, pulse animations, power bullets aren't effected by bullets
                    }
                }
            }
        }
        else if (patternType == Vertical) {
            // placeholder
        }
    }
}
