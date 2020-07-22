#pragma once

#include "common_types.h"
#include "FontRendererResult.h"
#include "LayoutResult.h"
#include "FontConfig.h"
#include "LayoutConfig.h"

struct FontConfig;
struct LayoutConfig;
struct FontRendererResult;
struct LayoutResult;

namespace fontium {
    struct Symbol {
        uint id;
        int placeX;
        int placeY;
        int placeW;
        int placeH;
        int offsetX;
        int offsetY;
        int advance;
        QMap<int32_t, int> kerning;
    };

    struct bitmap_font {
        Img *img;
        str image_file_name;
        str name;
        str family, style;
        int tex_width;
        int tex_height;
        const FontConfig *font_config;
        const LayoutConfig *layout_config;
        RenderedMetrics metrics;
        float scale;
        std::vector<Symbol> chars;

        static
        bitmap_font from(Img *img, str file,
                         const LayoutResult &layoutResult,
                         FontRendererResult &rendered,
                         const FontConfig &fontConfig,
                         const LayoutConfig &layoutConfig) {
            bitmap_font result{};

            result.img = img;
            result.image_file_name = std::move(file);
            result.metrics = rendered.metrics;
            result.font_config = &fontConfig;
            result.layout_config = &layoutConfig;
            result.metrics.height += fontConfig.line_spacing;

            for (const LayoutChar &lc : layoutResult.placed) {
                Symbol symb;
                symb.id = lc.symbol;
                symb.placeX = lc.x;
                symb.placeY = lc.y;
                symb.placeW = lc.w;
                symb.placeH = lc.h;

                const RenderedChar &rc = rendered.chars[symb.id];

                symb.offsetX = rc.offsetX - layoutConfig.offset_left;
                symb.offsetY = rc.offsetY + layoutConfig.offset_top;
                symb.advance = rc.advance + fontConfig.char_spacing;
                symb.kerning = rc.kerning;

                result.chars.push_back(symb);
            }
            result.tex_width = layoutResult.width;
            result.tex_height = layoutResult.height;

            return result;
        }

    };

}