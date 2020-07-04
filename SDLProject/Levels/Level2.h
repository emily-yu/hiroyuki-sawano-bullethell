//
//  Level2.hpp
//  SDLProject
//
//  Created by Emily Yu on 7/2/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#pragma once
#include "Scene.h"
class Level2 : public Scene {

public:
    void Initialize(Scene *sceneList) override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override; // override old methods
};
