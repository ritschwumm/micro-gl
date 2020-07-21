#pragma once

#include "common_types.h"

struct FontConfig
{
    static FontConfig getDefault() {
        FontConfig config;
        config.size=0;
        config.hinting = HintingMethod::HintingDefault;
        config.render_missing = false;
        config.antialiased = true;
        config.aamethod = AAMethod::AAliasingNormal;
        config.bold = 0;
        config.size=0;
        config.width=100.0f;
        config.height=100.0f;
        config.line_spacing=0;
        config.char_spacing=0;
        config.dpi=96;
        config.characters= " !\"#$%&'()*+,-./0123456789:;<=>?@"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
                            "abcdefghijklmnopqrstuvwxyz{|}~";
        return config;
    }

    enum HintingMethod {
        HintingDisable,
        HintingDefault,
        HintingForceFreetypeAuto,
        HintingDisableFreetypeAuto
    };

    enum AAMethod {
        AAliasingNormal,
        AAliasingLight,
        AAliasingLCDhor,
        AAliasingLCDvert
    };

    str path;
    str filename;
    str family;
    str style;
    str characters;
    int face_index;
    int size;
    HintingMethod    hinting;
    bool    render_missing;
    bool    antialiased;
    AAMethod     aamethod;
    int    bold;
    int    italic;
    float   width;
    float   height;
    int char_spacing;
    int line_spacing;
    int dpi;
};