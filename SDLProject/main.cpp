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
#include "Map.h"
#include "Util.h"
#include "Scene.h"
#include "BulletPattern.h"
#include "Levels/Level1.h"
#include "Levels/Level2.h"
#include "Levels/Level3.h"

Scene *currentScene;
//Level1 *level1;
Scene *sceneList[3]; // array containing all the scenes
Entity *gameWinText;
Entity *gameLoseText;
Entity *gameStartText;
BulletPattern *pattern;
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

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    // Audio Loading
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096); // Start Audio
    music = Mix_LoadMUS("dooblydoo.mp3"); // Load Audio
    Mix_VolumeMusic(MIX_MAX_VOLUME / 16); // cut volume by 1/4
    Mix_PlayMusic(music, -1); // Play Audio
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
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
//    GLuint fontTextureID = Util::LoadTexture("sprite_texture2.png");
    gameLoseText->textureID = fontTextureID;
    gameLoseText->entityType = TEXT;
    gameLoseText->animIndices = NULL;
    gameLoseText->writeText = "Mission Failed";
    gameLoseText->position = glm::vec3(12, -4, 0);

    gameStartText = new Entity();
    //    GLuint fontTextureID = Util::LoadTexture("sprite_texture2.png");
    gameStartText->textureID = fontTextureID;
    gameStartText->entityType = TEXT;
    gameStartText->animIndices = NULL;
    gameStartText->writeText = "Jumper: Press Enter to Start";
    gameStartText->position = glm::vec3(-2, 0, 0);
    gameStartText->acceleration = glm::vec3(0, 0, 0);
    
    pattern = new BulletPattern();
    pattern->bulletTexture = Util::LoadTexture("circle.png"); // test bullet image
    pattern->xPivot = -2;
    pattern->yPivot = 0;
    pattern->waveCount = 5;
    pattern->Render(&program);
    
    // Initialize Game Objects
//    level1 = new Level1();
    sceneList[0] = new Level1();
    sceneList[1] = new Level2();
    sceneList[2] = new Level3();
    SwitchToScene(sceneList[0], sceneList[0]); // switch to first element in scenelist = level 1
    currentScene->state.lives = 3;
//    SwitchToScene(level1); // switch to level 1
}
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (gameStarted) {
        currentScene->Render(&program);
    }
    else {
        gameStartText->Render(&program);
        pattern->Render(&program);
    }
    // update viewMatrix with translation for sliding
    program.SetViewMatrix(viewMatrix);
    
//    gameWinText->Render(&program);
//    if (currentScene->state.player->position.x >= 12) {
    if (isWin) {
        gameWinText->Render(&program);
    }
    else if (isOver) {
        gameLoseText->position = currentScene->state.player->position;
        gameLoseText->Render(&program);
    }
    
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
//                    case SDLK_LEFT:
//                        // Move the player left
//                        break;
//
//                    case SDLK_RIGHT:
//                        // Move the player right
//                        break;
//
                    case SDLK_SPACE: // JUMPING
                        if (gameStarted) {
                            // prevent infinite jumping
                            if (currentScene->state.player->collidedBottom) { // only if reached ground again, can rejump
                                currentScene->state.player->jump = true;
                            }
                        }
                        break;
//                    case SDL_SCANCODE_UP:
//                        std::cout << "asdf" << std::endl;
//                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (gameStarted) {
        if (keys[SDL_SCANCODE_LEFT]) {
            currentScene->state.player->movement.x = -1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animLeft;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            currentScene->state.player->movement.x = 1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animRight;
        }
    }
    if (keys[SDL_SCANCODE_RETURN]) {
        gameStarted = true;
        Render();
    }
}
//void Render() {
//    glClear(GL_COLOR_BUFFER_BIT);
//    if (gameStarted) {
//        currentScene->Render(&program);
//    }
//    else {
//        gameStartText->Render(&program);
//    }
//    // update viewMatrix with translation for sliding
//    program.SetViewMatrix(viewMatrix);
//
////    gameWinText->Render(&program);
////    if (currentScene->state.player->position.x >= 12) {
//    if (isWin) {
//        gameWinText->Render(&program);
//    }
//    else if (isOver) {
//        gameLoseText->position = currentScene->state.player->position;
//        gameLoseText->Render(&program);
//    }
//
//    SDL_GL_SwapWindow(displayWindow);
//}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update() {
    if (gameStarted) {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

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

    // stop scrolling @ a point
    if (currentScene->state.player->position.x > 5) { // if x-coord is past 5, don't scroll further left (past wall)
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }
    
    if (currentScene->state.player->position.x >= 12) { // completed all three levels
        if (currentScene == sceneList[2]) {
            isOver = true;
            isWin = true;
            Render();
        }
    }
//    if (currentScene->state.player->isActive == false) { // once the player is dunzoed, render game over text
    if (currentScene->state.lives == 0) { // once the player is dunzoed, render game over text
        isOver = true;
        isWin = false;
        Render();
    }
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
