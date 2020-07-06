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
    radius = 1.0f;
    
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
void BulletPattern::Render(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);
    // draw each bullet
        for (int i = 0; i <= waveCount; i++) {
    //        if (i != 2) {
                // 2 * pi / #points = slice
                // slice * slice #, degree increment = angle
                float theta = 2.0f * 3.14 * float(i) / float(waveCount);//get the current angle
                
                // translate pivotPosition around 1 by 1 box
//                radius = sqrt(position.x * position.x + position.y * position.y);// a2+b2 = c2
                float xPosition = radius * cosf(theta); // centerX + radius * cos(angle)
                float yPosition = radius * sinf(theta);
                
//                if (!isActive) {
                    DrawSpriteFromTextureAtlas(program, bulletTexture, glm::vec3(xPosition + xPivot, yPosition + yPivot, 0));
//                    isActive = true;
//                }
                
                // calculate velocity direction as degree from translated position to pivot point
    //        }
        }
        glDisableVertexAttribArray(program->positionAttribute);
        glDisableVertexAttribArray(program->texCoordAttribute);
}
void BulletPattern::Update(float deltaTime) {
    if (isActive == false) return; // if not active, not rendered -> no updates made to rendering
//
//    // no collisions yet
//    collidedTop = false;
//    collidedBottom = false;
//    collidedLeft = false;
//    collidedRight = false;
//
//    // check if should execute behavior for ai
//    if (entityType == ENEMY) {
//        AI(player); // set movement for ai
//    }
//
//    if (animIndices != NULL) {
//        // animation block
//        if (glm::length(movement) != 0) { // only animate if there is player movement to process
//            animTime += deltaTime;
//            if (animTime >= 0.25f) { // after 0.25 seconds...
//                animTime = 0.0f; // reset timer counter
//                animIndex++; // change frames
//                if (animIndex >= animFrames) { // loop back to the beginning of the animation sequence
//                    animIndex = 0;
//                }
//            }
//        }
//        else {
//            animIndex = 0; // reset to resting index
//        }
//    }
//
//    if (jump) {
//        jump = false; // process jump and reset
//        velocity.y += jumpPower; // add velocity to y direction and move up
//
//        if (entityType == PLAYER) {
//            Mix_PlayChannel(-1, jumpEffect, 0); // bounce effect
//        }
//    }
    radius += deltaTime * speed;
    
    waveTime += deltaTime;
    if (waveTime >= 2.0) { // after 0.25 seconds...
        waveTime = 0.0f; // reset timer counter
        // TODO add iteration of wave
        Mix_PlayChannel(-1, jumpEffect, 0); // bounce effect
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
