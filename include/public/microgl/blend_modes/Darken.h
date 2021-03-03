#pragma once

#include <microgl/BlendMode.h>

namespace microgl {
    namespace blendmode {

        class Darken : public BlendModeBase<Darken> {
        public:

            template<uint8_t R, uint8_t G, uint8_t B>
            static inline void blend(const color_t &b,
                                     const color_t &s,
                                     color_t &output) {

                output.r = b.r<s.r ? b.r : s.r;
                output.g = b.g<s.g ? b.g : s.g;
                output.b = b.b<s.b ? b.b : s.b;
            }

        };

    }
}