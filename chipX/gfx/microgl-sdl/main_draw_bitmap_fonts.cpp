#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>
#include "src/Resources.h"
#include <microgl/Canvas.h>
#include <microgl/pixel_coders/RGB888_PACKED_32.h>
#include <microgl/pixel_coders/RGBA8888_ARRAY.h>
#include <microgl/pixel_coders/RGB888_ARRAY.h>
#include <microgl/samplers/texture.h>

#define TEST_ITERATIONS 100
#define W 640*1
#define H 640*1
SDL_Window * sdl_window;
SDL_Renderer * sdl_renderer;
SDL_Texture * sdl_texture;
Resources resources{};

using namespace microgl;
using namespace microgl::sampling;
using index_t = unsigned int;
using Bitmap24= Bitmap<uint32_t, coder::RGB888_PACKED_32>;
using Canvas24= Canvas<uint32_t, coder::RGB888_PACKED_32>;
using Texture24= sampling::texture<uint32_t, coder::RGB888_PACKED_32, sampling::texture_filter::NearestNeighboor>;
using font32= microgl::text::bitmap_font<vec4<uint8_t>, coder::RGBA8888_ARRAY>;
Canvas24 * canvas;
Texture24 tex_uv;
font32 font;

void loop();
void init_sdl(int width, int height);
float t=0;

template <typename number>
void test_texture() {
//    t+=-0.1;//-0.01;
//    canvas->drawQuad<blendmode::Normal, porterduff::None<>, false, number>(tex_uv,
//                                                                           t,10, 400, 400);

    text::text_format format;
    format.fontSize=30;
    canvas->drawText("hello", font, format, 10, 10, 200, 200, 255);
}

void render() {
    canvas->clear({111,111,111,255});
    test_texture<float>();

}

int main() {
    init_sdl(W, H);
    loop();
}

template <typename P, typename CODER>
void loadFont(const std::string & name, microgl::text::bitmap_font<P, CODER> &font) {
    rapidxml::xml_document<> d;
    resources.loadXML("fonts/"+name+"/font.fnt", d);
    auto * f= d.first_node("font");
    auto * f_info= f->first_node("info");
    auto * f_common= f->first_node("common");
    auto * f_chars= f->first_node("chars");
    strncpy(font.name, f_info->first_attribute("face")->value(), 10);
    font.nativeSize=atoi(f_info->first_attribute("size")->value());
    font.lineHeight=atoi(f_common->first_attribute("lineHeight")->value());
    font.baseline=atoi(f_common->first_attribute("base")->value());
    font.width=atoi(f_common->first_attribute("scaleW")->value());
    font.height=atoi(f_common->first_attribute("scaleH")->value());
    font.gylphs_count=atoi(f_chars->first_attribute("count")->value());
    auto * iter= f_chars->first_node("char");
    do {
        int id=atoi(iter->first_attribute("id")->value());
        int x=atoi(iter->first_attribute("x")->value());
        int y=atoi(iter->first_attribute("y")->value());
        int w=atoi(iter->first_attribute("width")->value());
        int h=atoi(iter->first_attribute("height")->value());
        int xoffset=atoi(iter->first_attribute("xoffset")->value());
        int yoffset=atoi(iter->first_attribute("yoffset")->value());
        int xadvance=atoi(iter->first_attribute("xadvance")->value());
        font.addChar(id, x, y, w, h, xoffset, yoffset, xadvance);
        iter = iter->next_sibling();
    } while (iter);
    // load bitmap
    auto img_font = resources.loadImageFromCompressedPath("fonts/"+name+"/font.png");
    auto *bmp_font = new Bitmap<vec4<uint8_t>, coder::RGBA8888_ARRAY>(img_font.data, img_font.width, img_font.height);
    font._bitmap=bmp_font;
}

void init_sdl(int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);

    sdl_window = SDL_CreateWindow("SDL2 Pixel Drawing", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, 0);
    sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_RGB888,
                                    SDL_TEXTUREACCESS_STATIC, width, height);
    auto img_2 = resources.loadImageFromCompressedPath("uv_256.png");
    auto bmp_uv_U8 = new Bitmap<vec3<uint8_t>, coder::RGB888_ARRAY>(img_2.data, img_2.width, img_2.height);
    tex_uv.updateBitmap(bmp_uv_U8->convertToBitmap<uint32_t , coder::RGB888_PACKED_32>());
    loadFont("digital_7", font);
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
//
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