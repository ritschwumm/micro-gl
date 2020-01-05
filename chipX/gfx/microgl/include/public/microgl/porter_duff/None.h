#pragma once

#include <microgl/PorterDuff.h>

namespace porterduff {

    class None : public PorterDuffBase<None> {
    public:
        inline static const char * type() {
            return "None";
        }

        inline static void composite(const color_t & b,
                                     const color_t & s,
                                     color_t &output,
                                     const unsigned int alpha_bits) {

            output.r=s.r;output.g=s.g;output.b=s.b;output.a=s.a;
        }

        inline static void composite(const color_f_t & b,
                                     const color_f_t & s,
                                     color_f_t &output) {
            output.r=s.r;output.g=s.g;output.b=s.b;output.a=s.a;
        }

    };

}