
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

SDL_Window* displayWindow;

typedef struct {
    float x;
    float y;
    float r;
    float g;
    float b;
} Vertex2D;



GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE,
                 surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

void DrawSprite(GLint texture, float x, float y, float rotation) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    GLfloat quad[] = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
}

void drawRectangle(float xPos, float yPos) {
    // Rectangle
    GLfloat quad[] = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
    glVertexPointer(2, GL_FLOAT, 0, quad);
    
    // Colors
    GLfloat triangleColors[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0};
    glColorPointer(3, GL_FLOAT, 0, triangleColors);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glLoadIdentity();
    glScalef(xPos, yPos, 0);
    glDrawArrays(GL_QUADS, 0, 4);
}


int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
    
	bool done = false;
    float lastFrameTicks = 0.0f;
    float shipX = 0.0f;
    float asteroidX = -1.5f;
    float xPos, yPos = 0;
	SDL_Event event;
    
	while (!done) {
        
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
        }
        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        GLuint ship = LoadTexture("playerShip1_blue.png");
        GLuint asteroid = LoadTexture("meteorBrown_big1.png");
        
        float ticks = (float)SDL_GetTicks()/10000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        shipX += elapsed;
        asteroidX += elapsed*2;
        xPos += elapsed;
        yPos += elapsed;
        DrawSprite(ship, shipX, 0, 270);
        DrawSprite(asteroid, asteroidX, 0, 0);
        
        if ( shipX >= 0.8f){
            shipX = 500;
            asteroidX = 500;
            
            drawRectangle(xPos, yPos);
        }
        SDL_GL_SwapWindow(displayWindow);
	}
    
	SDL_Quit();
	return 0;
}