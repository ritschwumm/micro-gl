#pragma once

#include <microgl/pixel_coder.h>
#include <microgl/micro_gl_traits.h>

namespace microgl {
    namespace coder {
        using u8 = uint8_t;

        /**
         * create a pixel coder, that packs color channels at the smallest possible 2^N int.
         * this coder calculates all of this config at compile-time, and also supports different
         * layout schemes for packing the channels in any order possible.
         * zero channels will be discarded
         *
         * @tparam r red channel bits <=8
         * @tparam g green channel bits <=8
         * @tparam b blue channel bits <=8
         * @tparam a alpha channel bits <=8
         * @tparam ri position of red channel
         * @tparam gi position of green channel
         * @tparam bi position of blue channel
         * @tparam ai position of alpha channel
         */
        template<u8 r, u8 g, u8 b, u8 a=0, u8 ri=0, u8 gi=1, u8 bi=2, u8 ai=3>
        struct RGBA_PACKED {
            using rgba = rgba_t<r,g,b,a>;
            using pixel = uint_t<r+g+b+a>;
            static constexpr u8 size_pixel = sizeof (pixel);

        private:
            // compile time
            static constexpr u8 shift_3= 0;
            static constexpr u8 shift_2= ri==3 ? r : (gi==3?g: (bi==3? b : (ai==3 ? a : 0)));
            static constexpr u8 shift_1= shift_2+(ri==2 ? r : (gi==2?g: (bi==2? b : (ai==2 ? a : 0))));
            static constexpr u8 shift_0= shift_1+(ri==1 ? r : (gi==1?g: (bi==1? b : (ai==1 ? a : 0))));
            static constexpr u8 shift[4] = {shift_0, shift_1, shift_2, shift_3};
//            static constexpr u8 shift[4] = {g + b + a, b+a, a, 0};

            static constexpr pixel r_mask_1 = ((1u<<r)-1), r_mask_2 = r_mask_1<<shift[ri];
            static constexpr pixel g_mask_1 = ((1u<<g)-1), g_mask_2 = g_mask_1<<shift[gi];
            static constexpr pixel b_mask_1 = ((1u<<b)-1), b_mask_2 = b_mask_1<<shift[bi];
            static constexpr pixel a_mask_1 = ((1u<<a)-1), a_mask_2 = a_mask_1<<shift[ai];

        public:
            inline void static
            encode(const color_t &input, pixel &output) {
                output = 0;
                if(r!=0) output += ((input.r & r_mask_1)<<shift[ri]);
                if(g!=0) output += ((input.g & g_mask_1)<<shift[gi]);
                if(b!=0) output += ((input.b & b_mask_1)<<shift[bi]);
                if(a!=0) output += ((input.a & a_mask_1)<<shift[ai]);
            }

            inline void static
            decode(const pixel &input, color_t &output) {
                if(r!=0) output.r = (input & r_mask_2) >> shift[ri];
                if(g!=0) output.g = (input & g_mask_2) >> shift[gi];
                if(b!=0) output.b = (input & b_mask_2) >> shift[bi];
                if(a!=0) output.a = (input & a_mask_2) >> shift[ai];
            };

        };

    }

}