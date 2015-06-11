//
//  MainGame.cpp
//  ShaderTesting
//
//  Created by Christian Cosgrove on 5/12/15.
//  Copyright (c) 2015 Christian Cosgrove. All rights reserved.
//

#include "MainGame.h"
#include <iostream>
#include <vector>

int main()
{
    MainGame game(720,720);
}

static const GLfloat g_quad_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f,
};

MainGame::MainGame(int windowWidth, int windowHeight) :
    width(windowWidth), height(windowHeight)
{
    width = windowWidth; height = windowHeight;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    window = SDL_CreateWindow("Shader Testing", 0, 0, width, height, SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(window);
    
    
    GLManager glManager("fragmentShaderReactionDiffusion.glsl","vertexShader.glsl");
    
    glManager.AddProgram("fragmentShader2.glsl", "vertexShader2.glsl");
    
    glManager.AddProgram("fragmentShaderDraw.glsl", "vertexShaderDraw.glsl");
    
    glManager.Programs[0].Use();
    
    GLuint vbo,vao;
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
    initFrame(glManager);
    
    glManager.Programs[0].Use();
    glManager.Programs[0].SetTexture("tex0", 0);
    glManager.Programs[0].SetTexture("tex1", 1);
    glManager.Programs[0].SetFloat("refractorStartPos", refractorStartPos);
    glManager.Programs[0].SetFloat("refractorEndPos", refractorEndPos);
    glManager.Programs[1].Use();
    glManager.Programs[1].SetTexture("tex0", 0);
    glManager.Programs[1].SetTexture("tex1", 1);
    glManager.Programs[2].Use();
    glManager.Programs[2].SetTexture("tex", 1);
    glManager.Programs[2].SetFloat("refractorStartPos", refractorStartPos);
    glManager.Programs[2].SetFloat("refractorEndPos", refractorEndPos);
    
    const int iterations = 1;
    
    int permutation=0;
    
    std::vector<float> buffer(width*height*3);
    while (open)
    {
        handleEvents();
        
        for (int it = 0; it<iterations; it++)
        {
            time++;
            
            {
                //glManager.Programs[0].SetFloat("time",time);
                glBindFramebuffer(GL_FRAMEBUFFER, fbo);
                glManager.Programs[0].Use();
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[(permutation+2)%3], 0);
                glManager.Programs[0].SetTexture("tex", (permutation+1)%3);
                glManager.Programs[0].SetTexture("texPrev", permutation%3);
                
                const int w=1;
                
                
//                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, &buffer[0]);
                
//                for (int i = 0; i<width;i++)
//                    for (int j = 0; j<height;j++)
//                    {
//                        float dx = (float)i/width - 0.5;
//                        float dy = (float)j/height - 0.5;
//                        
//                        if (dx*dx + dy*dy>0.1)
//                        {
//                            buffer[3*(i+width*j)+0]=0;
//                            buffer[3*(i+width*j)+1]=0;
//                            buffer[3*(i+width*j)+2]=0;
//                        }
//                        if (dx*dx + dy*dy>0.09)
//                        {
//                            const float mul=0.5;
//                            buffer[3*(i+width*j)+0]*=mul;
//                            buffer[3*(i+width*j)+1]*=mul;
//                            buffer[3*(i+width*j)+2]*=mul;
//                        }
//                    }
                
                
//                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,width, height, GL_RGB, GL_FLOAT, &buffer[0]);
                glClear(GL_COLOR_BUFFER_BIT);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glDisableVertexAttribArray(0);
                
            }
            
            
//            glBindFramebuffer(GL_FRAMEBUFFER, fbo2);
//            glManager.Programs[1].Use();
//            glClear(GL_COLOR_BUFFER_BIT);
//            glEnableVertexAttribArray(0);
//            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//            glDrawArrays(GL_TRIANGLES, 0, 6);
//            glDisableVertexAttribArray(0);
            
            
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glManager.Programs[2].Use();
            glManager.Programs[2].SetTexture("tex", textures[(-permutation)%3]);
            glClear(GL_COLOR_BUFFER_BIT);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glDisableVertexAttribArray(0);
            
            permutation++;
        }
        SDL_GL_SwapWindow(window);
        
    }
    
    
    
}

void createWavePacket(std::vector<float>& pixels, std::vector<float>& pixelsPrev, int width, int height, float x, float y, float radius, float frequency, float theta)
{
    for (int i = 0; i<width; i++)
    {
        for (int j = 0; j<height;j++)
        {
            float dx = (float)i/width-x;
            float dy = (float)j/height-y;
            
            
            const float high = 0.2;
            const float low = 0.1;
            const float n = frequency;
            
            
            const float st = sin(theta);
            const float ct = cos(theta);
            
            float xdisp = dx*ct+dy*st;
            float ydisp = sqrt((dx - xdisp * ct)*(dx - xdisp * ct)+(dy - xdisp * st)*(dy - xdisp * st));
            float f = 0.1*exp(-(xdisp*xdisp+ydisp*ydisp)/2/(radius*radius))/radius/(sqrt(2*M_PI));
            
            
            pixels[3*(i+j*width)+0]+=f*(1+cos(n*(dy*st + dx * ct)*M_2_PI));
            pixelsPrev[3*(i+j*width)+0]+=f*(1+cos(n*(dy*st + dx * ct)*M_2_PI +0.3*sqrt(0.4)/sqrt(0.4)));
            pixels[3*(i+j*width)+1]+=f*(1+cos(n*(dy*st + dx * ct)*M_2_PI));
            pixelsPrev[3*(i+j*width)+1]+=f*(1+cos(n*(dy*st + dx * ct)*M_2_PI +0.3*sqrt(0.4)/sqrt(0.4)));
        }
    }
}

void MainGame::initFrame(GLManager& glManager){
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
//    glGenTextures(1, &textures[0]);
//    glBindTexture(GL_TEXTURE_2D, textures[0]);
//    glActiveTexture(GL_TEXTURE0);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    
    
    std::vector<float> pixels(3*width*height);
    std::vector<float> pixelsPrev(3*width*height);
    
    createWavePacket(pixels, pixelsPrev, width, height, 0.3, 0.5, 0.02, 200, -M_PI/3);
//    createWavePacket(pixels, pixelsPrev, width, height, 0.7, 0.5, 0.03, 500, 4*M_PI/3);
    
    for (int i = 0; i<width; i++)
    {
        for (int j = 0; j<height;j++)
        {
            float dx = (float)i/width-0.3;
            float dy = (float)j/height-0.5;
            
            const float k = 1000;
            
            const float high = 0.2;
            const float low = 0.1;
            
            
//            pixels[3*(i+j*width)+0]=f < 0.1 ? high : low;
//            pixels[3*(i+j*width)+1]=f < 0.1 ? high : low;
//            pixels[3*(i+j*width)+2]=f < 0.1 ? high : low;
//            
//            pixelsPrev[3*(i+j*width)+0]=f < 0.099 ? high : low;
//            pixelsPrev[3*(i+j*width)+1]=f < 0.099 ? high : low;
//            pixelsPrev[3*(i+j*width)+2]=f < 0.099 ? high : low;
            
            float dt = atan2(dy,dx)/(M_PI_2);
            float dr = sqrt(dx*dx+dy*dy);
            
//            pixels[3*(i+j*width)+0]*=(float)rand()/RAND_MAX*1.;
//            pixels[3*(i+j*width)+1]*=(float)rand()/RAND_MAX*1.;
//            pixels[3*(i+j*width)+0]=sin(n*(dt)*2*M_PI) * sin(n*(dr)*2*M_PI);
//            pixels[3*(i+j*width)+1]=sin(n*(dt)*2*M_PI) * sin(n*(dr)*2*M_PI);
//            pixels[3*(i+j*width)+2]=sin(n*(dt)*2*M_PI) * sin(n*(dr)*2*M_PI);
        }
    }
//    pixelsPrev=pixels;
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, pixels);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);
    glGenFramebuffers(1,&fbo2);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo2);
    glViewport(0,0,width,height);
    
//    glGenTextures(1, &textures[1]);
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, textures[1]);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, pixels);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[1], 0);
    
    for (int i = 0; i<3;i++)
    {
        glGenTextures(1, &textures[i]);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, i==1 ? &pixels[0] : &pixelsPrev[0]);
    }
    
    
    
    
//    glUniform1i(glGetUniformLocation(glManager.Programs[1].programID, "tex"), texture);
//    glManager.Programs[0].Use();
//    glUniform1i(glGetUniformLocation(glManager.Programs[0].programID, "tex"), texture);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
}

void MainGame::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                open=false;
                break;
            case SDL_KEYDOWN:
                open=false;
                break;
        }
    }
}