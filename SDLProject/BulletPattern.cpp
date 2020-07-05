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
                float xPosition = 1 * cosf(theta); // centerX + radius * cos(angle)
                float yPosition = 1 * sinf(theta);
                
                DrawSpriteFromTextureAtlas(program, bulletTexture, glm::vec3(xPosition + xPivot, yPosition + yPivot, 0));
                
                // calculate velocity direction as degree from translated position to pivot point
    //        }
        }
        glDisableVertexAttribArray(program->positionAttribute);
        glDisableVertexAttribArray(program->texCoordAttribute);
}
