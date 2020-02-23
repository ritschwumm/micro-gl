#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>
#include <microgl/Canvas.h>
#include <microgl/pixel_coders/RGB888_PACKED_32.h>
#include <microgl/samplers/angular_linear_gradient.h>

#define TEST_ITERATIONS 100
#define W 640*1
#define H 640*1
SDL_Window * sdl_window;
SDL_Renderer * sdl_renderer;
SDL_Texture * sdl_texture;

using namespace microgl;
using namespace microgl::sampling;
using index_t = unsigned int;
using Canvas24= Canvas<uint32_t, coder::RGB888_PACKED_32>;

Canvas24 * canvas;
angular_linear_gradient<float> gradient{45};

void loop();
void init_sdl(int width, int height);
float t=0;

template <typename number>
void test_continous() {
    t-=0.1;
    gradient.setAngle(t);
    gradient.addStop(0.0f, {255,0,0});
    gradient.addStop(0.3f, {0,255,0});
    gradient.addStop(1.f, {0,0,255});
    canvas->drawQuad<blendmode::Normal, porterduff::None, number>(gradient, 0, 0, 400, 400);
}

template <typename number>
void test_once() {
    canvas->drawQuad<blendmode::Normal, porterduff::None, number>(gradient, 0, 0, 400, 400);
//    canvas->drawQuadrilateral<blendmode::Normal, porterduff::SourceOverOnOpaque, true, float>(
//            gradient,
//            0.0f,               0.0f,     0.0f, 1.0f,
//            256 + 100.0f + t,     0.0f,       1.0f, 1.0f,
//            256 + 0.0f,           256,         1.0f, 0.0f,
//            0.0f,                 256,         0.0f, 0.0f,
//            255);
//    canvas->drawQuad<blendmode::Normal, porterduff::None>(color::colors::RED, 0, 0, 400, 400);
}

void render() {
    canvas->clear(color::colors::WHITE);

//    test_continous<float>();
    test_continous<Q<10>>();

}

int main() {
    init_sdl(W, H);
    loop();
}

void init_sdl(int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);

    sdl_window = SDL_CreateWindow("SDL2 Pixel Drawing", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, 0);
    sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGB888,
                                    SDL_TEXTUREACCESS_STATIC, width, height);

    gradient.addStop(0.0f, {255,0,0});
    gradient.addStop(0.3f, {0,255,0});
    gradient.addStop(1.f, {0,0,255});
    canvas = new Canvas24(width, height);
}

int render_test(int N) {
    auto ms = std::chrono::milliseconds(1);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i)
        render();
    auto end = std::chrono::high_resolution_clock::now();
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    return int_ms.count();}

void loop() {
    bool quit = false;
    SDL_Event event;

    // 100 Quads
    int ms = render_test(TEST_ITERATIONS);
    std::cout << ms << std::endl;

    while (!quit)
    {
        SDL_PollEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYUP:
                if( event.key.keysym.sym == SDLK_ESCAPE )
                    quit = true;
                break;
        }

        render();

        SDL_UpdateTexture(sdl_texture, nullptr, canvas->pixels(),
                canvas->width() * canvas->sizeofPixel());
        SDL_RenderClear(sdl_renderer);
        SDL_RenderCopy(sdl_renderer, sdl_texture, nullptr, nullptr);
        SDL_RenderPresent(sdl_renderer);
    }

    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}
