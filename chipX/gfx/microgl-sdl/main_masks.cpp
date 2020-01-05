#include <iostream>
#include <chrono>
#include "src/Resources.h"
#include <SDL2/SDL.h>
#include <microgl/Canvas.h>
#include <microgl/vec2.h>
#include <microgl/color.h>
#include <microgl/pixel_coders/RGB888_ARRAY.h>
#include <microgl/pixel_coders/RGB888_PACKED_32.h>
#include <microgl/porter_duff/SourceOverOnOpaque.h>
#include <microgl/blend_modes/Normal.h>
#include <microgl/blend_modes/Multiply.h>
#include <microgl/samplers/NearestNeighbor.h>
#include <microgl/samplers/Bilinear.h>
#include <microgl/Bitmap.h>

#define TEST_ITERATIONS 1
#define W 640*1
#define H 480*1

SDL_Window * window;
SDL_Renderer * renderer;
SDL_Texture * texture;

typedef Bitmap<uint32_t, coder::RGB888_PACKED_32> Bitmap24bit_Packed32;
typedef Canvas<uint32_t, coder::RGB888_PACKED_32> Canvas24Bit_Packed32;

Canvas24Bit_Packed32 * canvas;

Bitmap24bit_Packed32 * bmp_2;
Bitmap24bit_Packed32 * bmp_uv;

Resources resources{};
using namespace microgl::color;
void loop();
void init_sdl(int width, int height);

static float d = 0;

inline void render() {

    canvas->clear(microgl::color::colors::WHITE);
        // 100, aa, bilinear -> 420
        // 100, aa, nearest -> 240
        // 100, no aa, no opacity, nearest -> 125

//        canvas->drawTriangle<blendmode::Normal, porterduff::SourceOverOnOpaque, false, sampler::NearestNeighbor>(*bmp_2,
//                                                                             0.0f,     0.0f,       0.0f, 1.0f,
//                                                                             float(W/2),  0.0f,       1.0f, 1.0f,
//                                                                             float(W/2), float(H/2),   1.0f, 0.0f,
//                                                                             255,
//                                                                             true, true, true);

//        std::cout << coder::RGB888_PACKED_32::format();
        canvas->drawQuad<blendmode::Normal, porterduff::SourceOverOnOpaque, sampler::Bilinear>(
                *bmp_2, -300, -300, 300, 300);

//        canvas->drawQuad<blendmode::Normal, porterduff::None>(RED, 0, 0, W, H, 0, 255);
//
        // with O3 i get 28-32 for drawTriangle and 14-16 for drawTriangleFast
        // with Os i get 52 for drawTriangle and 24-26 for drawTriangleFast
//        canvas->drawTriangle<blendmode::Normal, porterduff::None, false>(RED,
//                                                                             0,0,
//                                                                             W, 0,
//                                                                             W, H,
//                                                                             255, 0,
//                                                                             false, false, false);

//        canvas->drawTriangle<blendmode::Normal, porterduff::SourceOverOnOpaque, true>(RED,
//                                                                             0,0,
//                                                                             W, 0,
//                                                                             W, H,
//                                                                             255, 0,
//                                                                             true, true, true);

//        canvas->drawTriangleFast<blendmode::Normal, porterduff::None, false>(RED,
//                                                                             0,0,
//                                                                             22, 0,
//                                                                             22, 22,
//                                                                             255, 0,
//                                                                             true, true, true);

//        canvas->drawTriangle<blendmode::Normal, porterduff::None, false>(RED,
//                                                                             0,0,
//                                                                             22, 0,
//                                                                             22, 22,
//                                                                             255, 0,
//                                                                             true, true, true);

//        d+=3.1;
//        int G = 256;
//        canvas->drawQuadrilateral<blendmode::Normal, porterduff::SourceOverOnOpaque, true, sampler::Bilinear>(
//                *bmp_uv,
//                0,          0,0.0, 1.0,
//                G + 100 + d,0,1.0,1.0,
//                G + 0,           G,1.0,0.0,
//                0,               G,0.0,0.0,
//                222);

}

int main() {
    init_sdl(W, H);
    loop();
}

void init_sdl(int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL2 Pixel Drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, width, height);

    canvas = new Canvas24Bit_Packed32(width, height);
//    canvas = new Canvas24BitU8(width, height, new RGB888_ARRAY());
    auto img_1 = resources.loadImageFromCompressedPath("charsprites.png");
    auto img_2 = resources.loadImageFromCompressedPath("uv_256.png");
//
    auto *bmp_1_native = new Bitmap<vec3<uint8_t>, coder::RGB888_ARRAY>(img_1.data, img_1.width, img_1.height);
    auto *bmp_2_native = new Bitmap<vec3<uint8_t>, coder::RGB888_ARRAY>(img_2.data, img_2.width, img_2.height);

    bmp_uv = bmp_1_native->convertToBitmap<uint32_t, coder::RGB888_PACKED_32>();
    bmp_2 = bmp_2_native->convertToBitmap<uint32_t, coder::RGB888_PACKED_32>();
    resources.init();
}

int render_test(int N) {
    auto ms = std::chrono::milliseconds(1);
    auto ns = std::chrono::nanoseconds(1);
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i) {
        render();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    return int_ms.count();
}

void loop() {
    bool quit = false;
    SDL_Event event;

    // 100 Quads
    int ms = render_test(TEST_ITERATIONS);
    cout << ms << endl;

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

//        render();

        SDL_UpdateTexture(texture, nullptr, canvas->pixels(), canvas->width() * canvas->sizeofPixel());
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}