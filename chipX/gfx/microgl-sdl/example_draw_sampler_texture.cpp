#include "src/Resources.h"
#include "src/example.h"
#include <microgl/Canvas.h>
#include <microgl/pixel_coders/RGB888_PACKED_32.h>
#include <microgl/pixel_coders/RGB888_ARRAY.h>

#define TEST_ITERATIONS 100
#define W 640*1
#define H 640*1

using namespace microgl;
using namespace microgl::sampling;
using index_t = unsigned int;
float t=0;

int main() {
    using Canvas24= Canvas<Bitmap<coder::RGB888_PACKED_32>, CANVAS_OPT_2d_raster_FORCE_32_BIT>;
    using Texture24= sampling::texture<Bitmap<coder::RGB888_ARRAY>, sampling::texture_filter::NearestNeighboor>;
//    using number = Q<12>;
    using number = float;

    Resources resources{};
    auto img_2 = resources.loadImageFromCompressedPath("images/uv_256.png");
    auto * canvas = new Canvas24(W, H);;
    Texture24 tex_uv{new Bitmap<coder::RGB888_ARRAY>(img_2.data, img_2.width, img_2.height)};

    auto render = [&]() -> void {
        canvas->clear({255,255,255,255});
        canvas->drawRect<blendmode::Normal, porterduff::None<>, false, number>(tex_uv, 0, 0, 400, 400);
    };

    example_run(canvas, TEST_ITERATIONS, render);
}