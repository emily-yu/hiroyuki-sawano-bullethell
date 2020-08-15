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

//platforms are the possible collisions
//void Entity::Update(float deltaTime, Entity* player, Map *map) {
//void Entity::Update(float deltaTime, Entity* player, Entity *objects, int objectCount, Map *map) {
//void Entity::UpdateText(float newPower) {
//
//}
void Entity::Update(float deltaTime, Entity* player, Entity *objects, int objectCount) {
    if (isActive == false) return; // if not active, not rendered -> no updates made to rendering

    // no collisions yet
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    // check if should execute behavior for ai
//    if (entityType == ENEMY) {
//        AI(player); // set movement for ai
//    }
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

//        if (entityType == PLAYER) {
//            Mix_PlayChannel(-1, jumpEffect, 0); // bounce effect
//        }
    }
    // physics logic
    velocity.x = movement.x * speed; // instant velocity when start moving
    velocity += acceleration * deltaTime; // increase acceleration each second

    velocity += acceleration * deltaTime;

    // set up y velocity
    position.y += velocity.y * deltaTime;
//    CheckCollisionsY(platforms, platformCount); // adjust for collisions
//    CheckCollisionsY(map);
//    CheckCollisionsY(objects, objectCount); // Fix if needed

    // set up x velocity
    position.x += velocity.x * deltaTime;
//    CheckCollisionsX(platforms, platformCount); // adjust for collisions
//    CheckCollisionsX(map);
//    CheckCollisionsX(objects, objectCount); // Fix if needed

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
//void Entity::CheckCollisionsY(Map *map) {
//    // checks to make sure that entity doesn't fall off edges of tiles
//    // middle of top edge
//    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
//    // left of top edge
//    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
//    // right of top edge
//    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
//
//    // three sensors to check to make sure entity doesn't fall off edge of bottom tiles (standing on tile)
//    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
//    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
//    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
//
//    float penetration_x = 0;
//    float penetration_y = 0;
//
//    // is there something solid @ that position, for each of the 6 sensors above
//    // if not something solid (water, grass), can walk through that position and doesn't stop (rock/walls)
//    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
//        position.y -= penetration_y;
//        velocity.y = 0;
//        collidedTop = true;
//    }
//    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
//        position.y -= penetration_y;
//        velocity.y = 0;
//        collidedTop = true;
//    }
//    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
//        position.y -= penetration_y;
//        velocity.y = 0;
//        collidedTop = true;
//    }
//    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
//         position.y += penetration_y;
//         velocity.y = 0;
//         collidedBottom = true;
//     }
//     else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
//         position.y += penetration_y;
//         velocity.y = 0;
//         collidedBottom = true;
//     }
//     else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
//         position.y += penetration_y;
//         velocity.y = 0;
//         collidedBottom = true;
//     }
//}
//void Entity::CheckCollisionsX(Map *map) {
//    // two sensors, each in middle of left and right side
//    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
//    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);
//
//    float penetration_x = 0;
//    float penetration_y = 0;
//
//    // check if there is something solid on the left/right side
//    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
//        position.x += penetration_x;
//        velocity.x = 0;
//        collidedLeft = true;
//    }
//
//    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
//        position.x -= penetration_x;
//        velocity.x = 0;
//        collidedRight = true;
//    }
//}
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
//            offset + (-0.5f * size), 0.5f * size, offset + (-0.5f * size), -0.5f * size, offset + (0.5f * size), 0.5f * size, offset + (0.5f * size), -0.5f * size, offset + (0.5f * size), 0.5f * size, offset + (-0.5f * size), -0.5f * size,
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

// decision tree
void Entity::AI(Entity *player) {
    switch(aiType) {
        case WALKER:
            AIWalker();
            break;
        case WAITANDGO:
            AIWaitAndGo(player, player->textureID);
            break;
        case JUMPER:
            AIJumper();
            break;
    }
}
// behavior functions
// for assignment hints (1) need to add gravity (2) add collisions between player and ai
void Entity::AIWalker() {
    movement = glm::vec3(1, 0, 0); // set movement to move right
}
void Entity::AIWaitAndGo(Entity *player, GLuint newTexture) {
    switch (aiState) {
        case IDLE: // know if player is close enough
            if (glm::distance(position, player->position) < 1.5f) { // change state if player is near
                aiState = WALKING;
            }
            break;
        case WALKING:
            if (player->position.x > position.x) { // if player is to the left
//                movement = glm::vec3(-1, 0, 0); // start walking
                acceleration = glm::vec3(-1, 0, 0); // start walking
            }
            else {
//                movement = glm::vec3(1, 0, 0); // start walking
                acceleration = glm::vec3 (1, 0, 0);
            }
            break;
        case ATTACKING:
            break;
    }
}
void Entity::AIJumper() {
    if (position.y < 2) { // if player is to the left
//                movement = glm::vec3(-1, 0, 0); // start walking
        acceleration = glm::vec3(0, -2, 0); // start walking
    }
    else {
//                movement = glm::vec3(1, 0, 0); // start walking
        acceleration = glm::vec3 (0, 2, 0);
    }
}
