#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <SDL_mixer.h>

#include "Entity.h"
//#include "Map.h"
#include "Util.h"
#include "Scene.h"
#include "BulletPattern.h"
#include "BulletEnemy.h"
#include "Levels/Level1.h"
#include "Levels/Level2.h"
#include "Levels/Level3.h"

Scene *currentScene;
//Level1 *level1;
Scene *sceneList[3]; // array containing all the scenes
Entity *gameWinText;
Entity *gameLoseText;
Entity *gameStartText;
Entity *gameStartText2;

// bulletEnemy
BulletPattern *patternList_LEVEL12[2];
BulletPattern *playerPatternList[2]; // for player shoot
std::map<char, BulletPattern*> playerBulletTable;
float bulletCount = 2;
BulletEnemy *enemy;

bool isWin = false;
bool isOver = false;

void SwitchToScene(Scene *scene, Scene *oldScene) {
    currentScene = scene;
    currentScene->Initialize(oldScene);
}

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool gameStarted = false;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
Mix_Music *music; // pointer for main audio

glm::mat4 backgroundMatrix;
glm::vec3 backgroundPosition = glm::vec3(0,0,0);
void DrawBackground(GLuint backgroundImage) {
    float vertices[] = { -5.5, -6.5, 5.5, -6.5, 5.5, 6.5, -5.5, -6.5, 5.5, 6.5, -5.5, 6.5 };
    float texCoords[] = { 0.0, 5.0, 5.0, 5.0, 5.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0 };
    
    // draw
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, backgroundImage); glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    // move pink paddle to leftmost side of screen
    backgroundMatrix = glm::translate(backgroundMatrix, backgroundPosition);
    
    // set new dimensions for pink
    program.SetModelMatrix(backgroundMatrix);
}
void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    // Audio Loading
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096); // Start Audio
    music = Mix_LoadMUS("dooblydoo.mp3"); // Load Audio
    Mix_VolumeMusic(MIX_MAX_VOLUME / 16); // cut volume by 1/4
    Mix_PlayMusic(music, -1); // Play Audio
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    backgroundPosition = glm::vec3(0, 0, 0);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    backgroundMatrix = glm::mat4(1.0f);
    GLuint backgroundID = Util::LoadTexture("purpletempbackground1.png");
    DrawBackground(backgroundID);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    gameWinText = new Entity();
    GLuint fontTextureID = Util::LoadTexture("sprite_texture2.png");
    gameWinText->textureID = fontTextureID;
    gameWinText->entityType = TEXT;
    gameWinText->animIndices = NULL;
    gameWinText->writeText = "Mission Successful";
    gameWinText->position = glm::vec3(12, -4, 0);
    
    gameLoseText = new Entity();
    gameLoseText->textureID = fontTextureID;
    gameLoseText->entityType = TEXT;
    gameLoseText->animIndices = NULL;
    gameLoseText->writeText = "Mission Failed";
    gameLoseText->position = glm::vec3(12, -4, 0);

    // title text
    gameStartText = new Entity();
    gameStartText->textureID = fontTextureID;
    gameStartText->entityType = TEXT;
    gameStartText->animIndices = NULL;
    gameStartText->writeText = "BulletHell Practice";
    gameStartText->position = glm::vec3(-2, 0, 0);
    gameStartText->acceleration = glm::vec3(0, 0, 0);
    
    gameStartText2 = new Entity();
    gameStartText2->textureID = fontTextureID;
    gameStartText2->entityType = TEXT;
    gameStartText2->animIndices = NULL;
    gameStartText2->writeText = "- Press Enter to Start -";
    gameStartText2->position = glm::vec3(-2.6, -0.5, 0);
    gameStartText2->acceleration = glm::vec3(0, 0, 0);
    
    // Initialize Game Objects
    sceneList[0] = new Level1();
    sceneList[1] = new Level2();
    sceneList[2] = new Level3();
    SwitchToScene(sceneList[0], sceneList[0]); // switch to first element in scenelist = level 1
    currentScene->state.lives = 3;
//    SwitchToScene(level1); // switch to level 1
    
    // construct patterns for level
    GLuint circleBulletTexture = Util::LoadTexture("circle.png");
    for (int i = 0; i < 2; i++) {
        playerPatternList[i] = new BulletPattern();
        playerPatternList[i]->bulletTexture = circleBulletTexture; // test bullet image
        playerPatternList[i]->speed = 0.5f;
        playerPatternList[i]->movement = glm::vec3(1, 0, 0);
        playerPatternList[i]->velocity = glm::vec3(1, 0, 0);
        playerPatternList[i]->acceleration = glm::vec3(0, -9.81f, 0);
    }
    playerPatternList[0]->isActive = true;
    playerPatternList[0]->xPivot = -2;
    playerPatternList[0]->yPivot = 0;
    playerPatternList[0]->waveCount = 20;
    playerPatternList[0]->patternType = SingularSpiral;
//    playerPatternList[0]->patternType = Vertical;

    playerPatternList[1]->xPivot = -3;
    playerPatternList[1]->yPivot = 3;
    playerPatternList[1]->waveCount = 20;
//    playerPatternList->patternType = CirclePulse;
    playerPatternList[1]->patternType = Vertical;

    playerBulletTable = {
        // { timeLeft, BulletPattern to be displayed }
        { 'd', playerPatternList[0] }, // default - always 1.0... TODO: set as isActive always
        { 's', playerPatternList[1] } // add time when pressing space
    };
}
bool backgroundDrawn = false;
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (gameStarted) {
        currentScene->Render(&program);
        for (auto& x: playerBulletTable) {
            x.second->Render(&program);
        }
    }
    else {
        GLuint backgroundID = Util::LoadTexture("purpletempbackground1.png");
        DrawBackground(backgroundID);
        gameStartText->Render(&program);
        gameStartText2->Render(&program);
    }
    // update viewMatrix with translation for sliding
    program.SetViewMatrix(viewMatrix);
    
    if (isWin) {
        gameWinText->Render(&program);
    }
    else if (isOver) {
        gameLoseText->position = currentScene->state.player->position;
        gameLoseText->Render(&program);
    }
    
//    // render bullet bases
//    for (int i = 0; i < bulletCount; i++) {
//        patternList_LEVEL12[i]->Render(&program);
//    }
//    enemy->Render(&program);
    
    SDL_GL_SwapWindow(displayWindow);
}
void ProcessInput() {
    
    if (gameStarted) {
        //    state.player->movement = glm::vec3(0);
        currentScene->state.player->movement = glm::vec3(0);
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN: // for one click events
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE: // JUMPING
                        if (gameStarted) {
                            // prevent infinite jumping
                            if (currentScene->state.player->collidedBottom) { // only if reached ground again, can rejump
                                currentScene->state.player->jump = true;
                            }
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (gameStarted) {
        if (keys[SDL_SCANCODE_LEFT]) {
            currentScene->state.player->movement.x = -1.0f;
            currentScene->state.player->velocity.x = -2.0f;
            currentScene->state.player->velocity.y = 0.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animLeft;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            currentScene->state.player->movement.x = 1.0f;
            currentScene->state.player->velocity.x = 2.0f;
            currentScene->state.player->velocity.y = 0.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animRight;
        }
        else if (keys[SDL_SCANCODE_DOWN]) {
            currentScene->state.player->movement.y = -1.0f;
            currentScene->state.player->velocity.y = -2.0f;
            currentScene->state.player->velocity.x = 0.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animDown;
        }
        else if (keys[SDL_SCANCODE_UP]) {
            currentScene->state.player->movement.y = 1.0f;
            currentScene->state.player->velocity.y = 2.0f;
            currentScene->state.player->velocity.x = 0.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animUp;
        }
        else if (keys[SDL_SCANCODE_V]) { // for casting a spell
            // TODO: check if powerlevel is enough
            playerBulletTable['s']->remainingDuration += 1.0;
            std::cout << playerBulletTable['s']->remainingDuration << std::endl;
        }
    }
    if (keys[SDL_SCANCODE_RETURN]) {
        gameStarted = true;
        Render();
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
float backgroundScrollSpeed = 0.0f;
void Update() {
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    if (gameStarted) {
        deltaTime += accumulator;
        if (deltaTime < FIXED_TIMESTEP) { // when total change between updates is less than timestep...
            accumulator = deltaTime; // store the change in time until the next update
            return;
        }

        while (deltaTime >= FIXED_TIMESTEP) {
            currentScene->Update(FIXED_TIMESTEP);
            deltaTime -= FIXED_TIMESTEP; // keep updating until frames are all processed
        }

        accumulator = deltaTime;
        
        viewMatrix = glm::mat4(1.0f);

        currentScene->state.backgroundPos.y -= 0.25;
        currentScene->state.backgroundMat = glm::mat4(1.0f); // base matrix value
        currentScene->state.backgroundMat = glm::translate(currentScene->state.backgroundMat, currentScene->state.backgroundPos); // translate by new position
        program.SetModelMatrix(currentScene->state.backgroundMat);
        
        if (currentScene->state.backgroundPos.y < -4.0f) {
            currentScene->state.backgroundPos.y = 4;
        }
        
        currentScene->state.accumulatedTime += deltaTime;
        
        // for firing spell bullets specifically / not default bullets / additional bullets
        for (auto& x : playerBulletTable) {
            if (x.second->remainingDuration > 0.0 && x.second->isActive == false) {
                if (x.first != 'd') { // is not the first element/default
//                    x.second->remainingDuration += 1.0; // add 1.0 seconds to firing time
                    x.second->isActive = true;
                    std::cout << "remaning time: " << x.second->remainingDuration << std::endl;
                }
            }
//            else if (x.second->remainingDuration <= 0.0 && x.first != 'd') {
//                x.second->isActive = false;
//            }
            x.second->remainingDuration -= deltaTime; // subtract amount of time passed to update remainingTime
            x.second->Update(deltaTime, currentScene->state.player->position);
        };
    }
    else {
        backgroundPosition.y -= 0.25;
        backgroundMatrix = glm::mat4(1.0f); // base matrix value
        backgroundMatrix = glm::translate(backgroundMatrix, backgroundPosition); // translate by new position
        program.SetModelMatrix(backgroundMatrix);
        
        if (backgroundPosition.y < -4.0f) {
            backgroundPosition.y = 4;
        }
//        enemy->Update(deltaTime);
    }
}
void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        
        if (currentScene->state.nextScene >= 0) { // if next scene is changed to something else
            SwitchToScene(sceneList[currentScene->state.nextScene], sceneList[currentScene->state.nextScene - 1]); // switch to nextscene
        }
        Render();
    }
    
    Shutdown();
    return 0;
}
