//
//  Entity.cpp
//  SDLProject
//
//  Created by Emily Yu on 6/15/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Entity.h"

Entity::Entity() {
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    
    modelMatrix = glm::mat4(1.0f);
//    jumpEffect = Mix_LoadWAV("bullet_shoot.wav");
}
bool Entity::CheckCollision(Entity *other) {
    if (isActive == false || other->isActive == false) return false; // if either is not active, then there won't be a collision
    
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) {
        lastCollision = other->entityType;
        if (entityType == PLAYER && lastCollision == ENEMY) {
            // TODO minus one from lives
            isActive = false;
        }
        return true;
    }
    return false;
}
// for y direction
void Entity::CheckCollisionsY(Entity *objects, int objectCount) {

    for (int i = 0; i < objectCount; i++) {
        Entity *platform = &objects[i];

        if (CheckCollision(platform)) { // if collide
            float ydist = fabs(position.y - platform->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (platform->height / 2.0f)); // how too far the player went

            // if going up (jump and hit platform)
            if (velocity.y > 0) {
                position.y -= penetrationY; // push the player back
                velocity.y = 0; // stop the player from going farther
                collidedTop = true; // top flag triggered
            }
            // if falling through bottom platform
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true; // bottom flag triggered
            }
        }
    }
}

// for x direction
void Entity::CheckCollisionsX(Entity *objects, int objectCount) {

    for (int i = 0; i < objectCount; i++) {
        Entity *platform = &objects[i];

        if (CheckCollision(platform)) { // if collide
            float xdist = fabs(position.y - platform->position.y);
            float penetrationX = fabs(xdist - (width / 2.0f) - (platform->width / 2.0f)); // how too far the player went

            // if going up (jump and hit platform)
            if (velocity.y > 0) {
                position.x -= penetrationX; // push the player back
                velocity.x = 0; // stop the player from going farther
                collidedRight = true;
            }
            // if falling through bottom platform
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }
        }
    }
}

void Entity::Update(float deltaTime, Entity* player, Entity *objects, int objectCount) {
    if (isActive == false) return; // if not active, not rendered -> no updates made to rendering

    // no collisions yet
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    if (animIndices != NULL) {
        // animation block
        if (glm::length(movement) != 0) { // only animate if there is player movement to process
            animTime += deltaTime;
            if (animTime >= 0.25f) { // after 0.25 seconds...
                animTime = 0.0f; // reset timer counter
                animIndex++; // change frames
                if (animIndex >= animFrames) { // loop back to the beginning of the animation sequence
                    animIndex = 0;
                }
            }
        }
        else {
            animIndex = 0; // reset to resting index
        }
    }

    if (jump) {
        jump = false; // process jump and reset
        velocity.y += jumpPower; // add velocity to y direction and move up
    }
    // physics logic
    velocity.x = movement.x * speed; // instant velocity when start moving
    velocity += acceleration * deltaTime; // increase acceleration each second

    velocity += acceleration * deltaTime;

    // set up xy velocity
    position.y += velocity.y * deltaTime;
    position.x += velocity.x * deltaTime;

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);

    // reset velocity after movement is applied - temp since don't want drifting player
    velocity.x = 0;
    velocity.y = 0;
}

void Entity::Render(ShaderProgram *program) {
    if (isActive == false) return; // if not active don't need to render

    program->SetModelMatrix(modelMatrix);

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }
    if (entityType == TEXT) {
        DrawText(program, textureID, writeText, 0.5f, -0.35f, glm::vec3(position));
        return;
    }
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
Entity* Entity::CheckCollisionEntity(Entity *other) {
    if (isActive == false || other->isActive == false) return NULL; // if either is not active, then there won't be a collision
    
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    if (xdist < 0 && ydist < 0) {
        lastCollision = other->entityType;
        return other;
    }
    return NULL;
}
void Entity::DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position) {

    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;
    std::vector<float> vertices;
    std::vector<float> texCoords;
    
    for (int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;
        
        vertices.insert(vertices.end(), {
            offset + (-0.25f * size), 0.25f * size, offset + (-0.25f * size), -0.25f * size, offset + (0.25f * size), 0.25f * size, offset + (0.25f * size), -0.25f * size, offset + (0.25f * size), 0.25f * size, offset + (-0.25f * size), -0.25f * size,
        });
            
        texCoords.insert(texCoords.end(), { u, v,
                u, v + height,
                u + width, v,
                u + width, v + height,
                u + width, v,
                u, v + height,
        });
    } // end of for loop
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index) {
    // get the col and row for top left corner of index 7, starting counting from leftmost corner of sprite 0
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    // get uniform width/height of each sprite
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    // texture coordinates
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
    u, v + height, u + width, v, u, v};

    // verticies being mapped to
//    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float vertices[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };

    // classic drawing for 2d
    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);
 
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
