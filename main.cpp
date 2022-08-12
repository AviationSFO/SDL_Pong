#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#define WIDTH 800
#define HEIGHT 600

#define FPS 60
#define DELAY 1000 / FPS

Uint32 frameStart;
int frameTime;

int ball_Vx;
int ball_Vy;

int score_a;
int score_b;

SDL_Texture *fishTex;

SDL_Rect destL_R;

SDL_Rect destR_R;

SDL_Rect ballR;

SDL_Rect score_a_rect;
SDL_Rect score_b_rect;

void reset_ball()
{
    ballR.x = WIDTH / 2 - ballR.w / 2;
    ballR.y = 150;

    destL_R.x = 0;
    destL_R.y = 300 - 24;

    destR_R.x = WIDTH - destR_R.w;
    destR_R.y = 300 - 24;

    ball_Vx = -5;
    ball_Vy = rand() % 8 - 4;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow(
        "SDL2 Pong",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Event event;

    int blue = 0;
    int frameCount = 0;
    int fps = 0;
    int lastTime = SDL_GetTicks();
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    // load bg
    SDL_Surface *bg = IMG_Load("assets/bg.png");
    SDL_Texture *bgTex = SDL_CreateTextureFromSurface(renderer, bg);

    // loading fishes
    SDL_Surface *surface = IMG_Load("assets/helo-fish-.png");
    fishTex = SDL_CreateTextureFromSurface(renderer, surface);

    // loading ball
    SDL_Surface *surface2 = IMG_Load("assets/ball.png");
    SDL_Texture *ballTex = SDL_CreateTextureFromSurface(renderer, surface2);

    // loading font
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("assets/NovaSquare-Regular.ttf", 24);
    SDL_Color White = {255, 255, 255};
    SDL_Surface *fontSurf = TTF_RenderText_Solid(font, "0", White);
    SDL_Texture *fontTex_a = SDL_CreateTextureFromSurface(renderer, fontSurf);
    SDL_Surface *fontSurf2 = TTF_RenderText_Solid(font, "0", White);
    SDL_Texture *fontTex_b = SDL_CreateTextureFromSurface(renderer, fontSurf2);

    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(bg);
    SDL_FreeSurface(fontSurf);
    SDL_FreeSurface(fontSurf2);

    destL_R.y = 300 - 24;

    destL_R.x = 0;

    destL_R.h = 64;
    destL_R.w = 24;

    destR_R.h = 64;
    destR_R.w = 24;

    destR_R.y = 300 - 24;
    destR_R.x = WIDTH - 24;

    ballR.h = 24;
    ballR.w = 24;
    reset_ball();

    score_a_rect.x = 200;
    score_a_rect.y = 100;
    score_a_rect.w = 48;
    score_a_rect.h = 48;

    score_b_rect.x = 600;
    score_b_rect.y = 100;
    score_b_rect.w = 48;
    score_b_rect.h = 48;




    // main loop
    while (true)
    {
        frameStart = SDL_GetTicks();

        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return 1;
            }

            if (event.type == SDL_KEYDOWN)
            {
                // std::cout << "Key down" << std::endl;
                if (SDLK_UP == event.key.keysym.sym)
                {
                    if (destL_R.y > 0)
                    {
                        destL_R.y -= 10;
                    }
                }
                else if (SDLK_DOWN == event.key.keysym.sym)
                {
                    if (destL_R.y < HEIGHT - destL_R.h)
                    {
                        destL_R.y += 10;
                    }
                }
                else if (SDLK_ESCAPE == event.key.keysym.sym)
                {
                    return 0;
                }
            }
        }
        frameCount++;

        if (SDL_GetTicks() - lastTime > 1000)
        {
            lastTime = SDL_GetTicks();

            fps = frameCount;
            frameCount = 0;
            system("cls");
            std::cout << "FPS: " << fps << std::endl;
        }

        // update ball
        ballR.x += ball_Vx;
        ballR.y += ball_Vy;
        if (ballR.x < 0)
        {
            score_b++;
            reset_ball();
            SDL_Delay(1000);
        }
        else if (ballR.x > WIDTH - ballR.w)
        {
            score_a++;
            reset_ball();
            SDL_Delay(1000);
        }
        if (ballR.y < 0)
        {
            ball_Vy = -ball_Vy;
        }
        else if (ballR.y > HEIGHT - ballR.h)
        {
            ball_Vy = -ball_Vy;
        }

        // detect collision
        if (SDL_HasIntersection(&ballR, &destL_R))
        {
            ball_Vx = -ball_Vx;
        }
        else if (SDL_HasIntersection(&ballR, &destR_R))
        {
            ball_Vx = -ball_Vx;
        }

        // opponent AI match y with ball
        if (destR_R.y < ballR.y)
        {
            destR_R.y += 3;
        }
        else if (destR_R.y > ballR.y)
        {
            destR_R.y -= 3;
        }

        // update finish/render
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, bgTex, NULL, NULL);

        // update scores
        SDL_Surface *fontSurf = TTF_RenderText_Solid(font, std::to_string(score_a).c_str(), White);
        SDL_Texture *fontTex_a = SDL_CreateTextureFromSurface(renderer, fontSurf);
        SDL_FreeSurface(fontSurf);
        
        SDL_Surface *fontSurf2 = TTF_RenderText_Solid(font, std::to_string(score_b).c_str(), White);
        SDL_Texture *fontTex_b = SDL_CreateTextureFromSurface(renderer, fontSurf2);
        SDL_FreeSurface(fontSurf2);



        SDL_RenderCopy(renderer, fontTex_a, NULL, &score_a_rect);

        SDL_RenderCopy(renderer, fontTex_b, NULL, &score_b_rect);

        SDL_RenderCopy(renderer, fishTex, NULL, &destL_R);

        SDL_RenderCopy(renderer, fishTex, NULL, &destR_R);

        SDL_RenderCopy(renderer, ballTex, NULL, &ballR);

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (DELAY > frameTime)
        {
            SDL_Delay(DELAY - frameTime);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
