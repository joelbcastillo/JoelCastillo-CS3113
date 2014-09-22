//
//  main.cpp
//  NYUCodebase
//
//  Created by Joel Castillo on 9/21/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#define PI 3.14159265

SDL_Window* displayWindow;

GLuint LoadTexture(const char* image_path, GLenum image_format = GL_BGRA) {
    
    SDL_Surface *surface = IMG_Load(image_path);
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, image_format, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}

// Paddle Size
float paddleWidth = 0.05;
float paddleHeight = .25;
float paddleSpeed = 0.5;

// Left Paddle
float leftPaddleX = -1.33f;
float leftPaddleY = 0.0f - paddleHeight/2;

// Left Paddle
float rightPaddleX = 1.28f;
float rightPaddleY = 0.0f - (paddleHeight / 2);

// Ball Size
float ballSize = .05;

// Ball Position
float ballPosY = 0;
float ballPosX = 0;

float ballXVel = .1;
float ballYVel = .1;

float bottomHeight, topHeight = 0.1f;

float topY = 0.1f;
float topX = 0.0f;
float bottomY = -0.1f;
float bottomX = 0.0f;


void drawRect( float x, float y, float width, float height) {
    GLfloat quad[] = {x, y, x+width, y, x+width, y+height, x, y+height};
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_QUADS, 0, 4);
}

// Keyboard Input
const Uint8* keys = SDL_GetKeyboardState(nullptr);

void Setup() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Joel Castillo - Assignment 2 - Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void resetGame( bool rightWon = true ) {
    // Reset Ball Position
    ballPosX = 0.0f;
    ballPosY = 0.0f;
    // Choose Ball Launch Direction
    if (rightWon) {
        ballXVel = 1.0f;
    }
    else {
        ballXVel = -1.0f;
    }
    ballYVel = 0.0f;
    
    // Reset Paddle Position
    leftPaddleX = 0.0f;
    leftPaddleY = 0.0f;
    rightPaddleX = 0.0f;
    rightPaddleY = 0.0f;
    
    // Reset Top and Bottom
    topY = 1.0f;
    topX = 0.0f;
    bottomY = -1.0f;
    bottomX = 0.0f;
    
}

bool ProcessEvents() {
    SDL_Event event;
    
    while( SDL_PollEvent(&event)) {
        if( event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            return false;
        }
    }
    
    // Move Paddles
    if( keys[SDL_SCANCODE_W] ) {
        leftPaddleY = 0.05f;
    }
    else if( keys[SDL_SCANCODE_S] ) {
        leftPaddleY = -0.05f;
    }
    
    if( keys[SDL_SCANCODE_UP] ) {
        rightPaddleY= 0.05f;
    }
    else if( keys[SDL_SCANCODE_DOWN] ) {
        rightPaddleY = -0.05f;
    }
    
    // Collisions - Right Side
    
    if(((ballPosX + ballSize * 0.5) > (rightPaddleX - paddleWidth * 0.5)) &&
       ((ballPosX + ballSize * 0.5) < (rightPaddleX)) &&
       ((ballPosY + ballSize * 0.5) > (rightPaddleY - paddleHeight * 0.5)) &&
       ((ballPosY + ballSize * 0.5) < (rightPaddleY + paddleHeight * 0.5)))
    {
        ballPosX = (rightPaddleX - paddleWidth * 0.5) - (ballSize * 0.5);
        ballXVel = -ballXVel;
        float speed = (ballPosY - rightPaddleY) / (paddleHeight * 0.5);
        ballYVel = speed;
    }
    
    // Collisions - Left Side
    if(((ballPosX - ballSize * 0.5) < (leftPaddleX + paddleWidth * 0.5)) &&
       ((ballPosX - ballSize * 0.5) > (leftPaddleX)) &&
       ((ballPosY - ballSize * 0.5) < (leftPaddleY - paddleHeight * 0.5)) &&
       ((ballPosY - ballSize * 0.5) > (leftPaddleY - paddleHeight * 0.5)))
    {
        ballPosX = (leftPaddleX + paddleWidth * 0.5) + (ballSize * 0.5);
        ballXVel = -ballXVel;
        float speed = (ballPosY - leftPaddleY) / (paddleHeight * 0.5);
        ballYVel = speed;
    }
    
    // Collision - Top
    // Ball
    if(( ballPosY + ballSize * 0.5) > (topY - topHeight * 0.5)) {
        ballPosY = (topY - topHeight * 0.5 ) + (ballSize * 0.5);
        ballYVel = -ballYVel;
    }
    // Right Paddle
    if(( rightPaddleY + paddleHeight * 0.5) > (topY - topHeight * 0.5)) {
        rightPaddleY = (topY - topHeight * 0.5 ) - (paddleHeight * 0.5);
    }
    // Left Paddle
    if(( leftPaddleY + paddleHeight * 0.5) > (topY - topHeight * 0.5)) {
        leftPaddleY = (topY - topHeight * 0.5 ) - (paddleHeight * 0.5);
    }
    
    // Collision - Bottom
    // Ball
    if(( ballPosY - ballSize * 0.5) < (bottomY + bottomHeight * 0.5)) {
        ballPosY = (bottomY + bottomHeight * 0.5 ) + (ballSize * 0.5);
        ballYVel = -ballYVel;
    }
    // Right Paddle
    if(( rightPaddleY - paddleHeight * 0.5) < (bottomY + bottomHeight * 0.5)) {
        rightPaddleY = (bottomY + bottomHeight * 0.5 ) + (paddleHeight * 0.5);
    }
    // Left Paddle
    if(( leftPaddleY - paddleHeight * 0.5) < (bottomY + bottomHeight * 0.5)) {
        leftPaddleY = (bottomY + bottomHeight * 0.5 ) + (paddleHeight * 0.5);
    }
    
    // Score Notification
    if( ballPosX > (rightPaddleX + paddleWidth) ) {
        resetGame(false);
    }
    
    if (ballPosX < (leftPaddleX - paddleWidth) )  {
        resetGame(false);
    }
    
    return true;
}

void update(float elapsed) {
    leftPaddleY += leftPaddleY * elapsed;
    rightPaddleY += rightPaddleY * elapsed;
    
    ballPosX = ballXVel * elapsed;
    ballPosY = ballYVel * elapsed;
}

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawRect(leftPaddleX, leftPaddleY, paddleWidth, paddleHeight);
    drawRect(rightPaddleX, rightPaddleY, paddleWidth, paddleHeight);
    
    drawRect(ballPosX - ballSize / 2, ballPosY - ballSize / 2, ballSize, ballSize);
    
    
    SDL_GL_SwapWindow(displayWindow);
}

void Cleanup() {
    
}

int main(int argc, char *argv[]) {
    Setup();
    
    float lastFrameTicks = 0.0f;
    
    resetGame();
    
    while( ProcessEvents() ) {
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        update(elapsed);
        
        render();
    }
    
    SDL_Quit();
    
    return 0;
}