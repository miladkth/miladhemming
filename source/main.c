#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SPEED 300
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WALL_WIDTH 20
#define WALL_PADDING 10
#define PROJECTILE_SPEED 600
#define PROJECTILE_SIZE 10

typedef struct {
    SDL_Rect rect;
    bool active;
} Projectile;

int main(int argv, char** args) {
    printf("Hej Milad");

     printf("hej hemming ");
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* pWindow = SDL_CreateWindow("Enkelt exempel 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!pWindow) {
        printf("Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!pRenderer) {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

    SDL_Surface* pSurface = IMG_Load("resources/ship.png");
    if (!pSurface) {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if (!pTexture) {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

    SDL_Rect shipRect, secondShipRect;
    SDL_QueryTexture(pTexture, NULL, NULL, &shipRect.w, &shipRect.h);
    shipRect.w /= 4;
    shipRect.h /= 4;
    shipRect.x = (WINDOW_WIDTH - shipRect.w) / 2;
    shipRect.y = (WINDOW_HEIGHT - shipRect.h) / 2;

    secondShipRect = shipRect;
    secondShipRect.x = (WINDOW_WIDTH - secondShipRect.w) / 4; // Startposition för andra skeppet
    secondShipRect.y = (WINDOW_HEIGHT - secondShipRect.h) / 4;

    Projectile projectiles[2]; // En för varje skepp
    for (int i = 0; i < 2; i++) {
        projectiles[i].rect.w = PROJECTILE_SIZE;
        projectiles[i].rect.h = PROJECTILE_SIZE;
        projectiles[i].active = false;
    }

    bool closeWindow = false;
    float shipVelocityX = 0, shipVelocityY = 0;
    float secondShipVelocityX = 0, secondShipVelocityY = 0;

    while (!closeWindow) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    closeWindow = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_UP:
                            shipVelocityY = -SPEED;
                            break;
                        case SDL_SCANCODE_DOWN:
                            shipVelocityY = SPEED;
                            break;
                        case SDL_SCANCODE_LEFT:
                            shipVelocityX = -SPEED;
                            break;
                        case SDL_SCANCODE_RIGHT:
                            shipVelocityX = SPEED;
                            break;
                        case SDL_SCANCODE_SPACE:
                            if (!projectiles[0].active) {
                                projectiles[0].active = true;
                                projectiles[0].rect.x = shipRect.x + shipRect.w / 2 - PROJECTILE_SIZE / 2;
                                projectiles[0].rect.y = shipRect.y + shipRect.h / 2 - PROJECTILE_SIZE / 2;
                            }
                            break;
                        case SDL_SCANCODE_W:
                            secondShipVelocityY = -SPEED;
                            break;
                        case SDL_SCANCODE_S:
                            secondShipVelocityY = SPEED;
                            break;
                        case SDL_SCANCODE_A:
                            secondShipVelocityX = -SPEED;
                            break;
                        case SDL_SCANCODE_D:
                            secondShipVelocityX = SPEED;
                            break;
                        case SDL_SCANCODE_LSHIFT:
                            if (!projectiles[1].active) {
                                projectiles[1].active = true;
                                projectiles[1].rect.x = secondShipRect.x + secondShipRect.w / 2 - PROJECTILE_SIZE / 2;
                                projectiles[1].rect.y =                                 projectiles[1].rect.y = secondShipRect.y + secondShipRect.h / 2 - PROJECTILE_SIZE / 2;
                            }
                            break;
                            ////kjrkejgkjerk
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_UP:
                        case SDL_SCANCODE_DOWN:
                            shipVelocityY = 0;
                            break;
                        case SDL_SCANCODE_LEFT:
                        case SDL_SCANCODE_RIGHT:
                            shipVelocityX = 0;
                            break;
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_S:
                            secondShipVelocityY = 0;
                            break;
                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_D:
                            secondShipVelocityX = 0;
                            break;
                    }
                    break;
            }
        }

        // Uppdatera positioner
        shipRect.x += shipVelocityX / 60;
        shipRect.y += shipVelocityY / 60;
        secondShipRect.x += secondShipVelocityX / 60;
        secondShipRect.y += secondShipVelocityY / 60;

        // Kolla kollision mellan skeppen
        if (SDL_HasIntersection(&shipRect, &secondShipRect)) {
            shipRect.x -= shipVelocityX / 60;
            shipRect.y -= shipVelocityY / 60;
            secondShipRect.x -= secondShipVelocityX / 60;
            secondShipRect.y -= secondShipVelocityY / 60;
        }

        // Uppdatera och rita projektiler
        for (int i = 0; i < 2; ++i) {
            if (projectiles[i].active) {
                projectiles[i].rect.x += (i == 0 ? PROJECTILE_SPEED : -PROJECTILE_SPEED) / 60;
                if (projectiles[i].rect.x < 0 || projectiles[i].rect.x > WINDOW_WIDTH) {
                    projectiles[i].active = false; // Inaktivera när den lämnar skärmen
                }
            }
        }

        // Rensa och rita allt
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255); // Svart bakgrund
        SDL_RenderClear(pRenderer);

        // Rendrera skeppen
        SDL_RenderCopy(pRenderer, pTexture, NULL, &shipRect);
        SDL_RenderCopy(pRenderer, pTexture, NULL, &secondShipRect);

        // Rendrera projektilerna
        for (int i = 0; i < 2; ++i) {
            if (projectiles[i].active) {
                SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255); // röd färg för projektilerna
                SDL_RenderFillRect(pRenderer, &projectiles[i].rect);
            }
        }

        // Rendrera väggen
        SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255); // Röd färg för väggen
        SDL_Rect wallRect = {WINDOW_WIDTH - WALL_WIDTH - WALL_PADDING, 0, WALL_WIDTH, WINDOW_HEIGHT};
        SDL_RenderFillRect(pRenderer, &wallRect);

        // Visa det uppdaterade innehållet
        SDL_RenderPresent(pRenderer);

        // Vänta lite för att säkerställa att vi håller rätt framerate
        SDL_Delay(1000 / 60);
    }

    // Städa upp
    SDL_DestroyTexture(pTexture);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}

