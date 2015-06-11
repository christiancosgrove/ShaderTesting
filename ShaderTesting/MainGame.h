//
//  MainGame.h
//  ShaderTesting
//
//  Created by Christian Cosgrove on 5/12/15.
//  Copyright (c) 2015 Christian Cosgrove. All rights reserved.
//
#pragma once
#include <SDL2/SDL.h>
#include "GLManager.h"


class MainGame
{
public:
    MainGame(int windowWidth, int windowHeight);
    
    
    
private:
    
    void initFrame(GLManager& glManager);
    void handleEvents();
    
    GLuint fbo,fbo2;
    GLuint textures[3];
    
    int width,height;
    SDL_Window* window;
    bool open = true;
    
    const float refractorStartPos = -0.2, refractorEndPos = -0.5;
    
    float time = 0;
};
