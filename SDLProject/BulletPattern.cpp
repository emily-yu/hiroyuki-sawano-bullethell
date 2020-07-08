//
//  BulletPattern.cpp
//  SDLProject
//
//  Created by Emily Yu on 7/5/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "BulletPattern.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include <SDL_image.h>
//#include "stb_image.h"
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

//#define BULLET_COUNT 5
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
    // Print this array.
    for(size_t i = 0; i < size; ++i)
       std::cout << radii[i] << std::endl;
    
    int new_size = size + 1;
    
    // Change the array size, add elements.
    radii = changeSize(radii, size, new_size);

    // Print the new array.
    std::cout << "new [";
    for(size_t i = 0; i < new_size; ++i) {
        if (i == new_size - 1) {
            // add new input to radii list
            radii[i] = radii_input;
        }
        std::cout << radii[i] << ", ";
    }
    std::cout << "]" << std::endl;
}
//bool secondWave = false;
void BulletPattern::Render(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);

    for (int iwave = 0; iwave < int(radiiCount); iwave++) {
        for (int i = 0; i <= waveCount; i++) {
            float radius = radii[iwave];
            
    //         2 * pi / #points = slice
            // slice * slice #, degree increment = angle
            float theta = 2.0f * 3.14 * float(i) / float(waveCount);//get the current angle

            // translate pivotPosition around 1 by 1 box
            float xPosition = radius * cosf(theta); // centerX + radius * cos(angle)
            float yPosition = radius * sinf(theta);

            DrawSpriteFromTextureAtlas(program, bulletTexture, glm::vec3(xPosition + xPivot, yPosition + yPivot, 0));
        }
    }

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
void BulletPattern::Update(float deltaTime) {
    if (isActive == false) return; // if not active, not rendered -> no updates made to rendering
    
    // hardcode first two updates for waves
    float change = deltaTime * speed;
    for (int iwave = 0; iwave < int(radiiCount); iwave++) {
        radii[iwave] += change;
    }
    
    // add new wave if time is right
    waveTime += deltaTime;
    if (waveTime >= 2.0) { // after 0.25 seconds...
        waveTime = 0.0f; // reset timer counter
        // TODO add iteration of wave
        Mix_PlayChannel(-1, jumpEffect, 0); // bounce effect
//        secondWave = true;
        
        // new wave addition
        radiiCount += 1;
        addWave(baseRadius, radiiCount);
    }
    
    // physics logic
    velocity.x = movement.x * speed; // instant velocity when start moving
    velocity += acceleration * deltaTime; // increase acceleration each second

    velocity += acceleration * deltaTime;

    // set up y velocity
    position.y += velocity.y * deltaTime;
////    CheckCollisionsY(platforms, platformCount); // adjust for collisions
//    CheckCollisionsY(map);
//    CheckCollisionsY(objects, objectCount); // Fix if needed

    // set up x velocity
    position.x += velocity.x * deltaTime;
////    CheckCollisionsX(platforms, platformCount); // adjust for collisions
//    CheckCollisionsX(map);
//    CheckCollisionsX(objects, objectCount); // Fix if needed

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}
