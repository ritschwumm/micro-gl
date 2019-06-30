#include "../include/microgl/Bitmap.h"

template<typename P, typename CODER>
Bitmap<P, CODER>::Bitmap(P* $pixels, int w, int h, PixelCoder<P, CODER> * $coder) :
        FrameBuffer<P>($pixels, w * h), _width{w}, _height{h}, _coder{$coder}, _format{$coder->format()} {
    _hi = new RGB888_PACKED_32();
}

template<typename P, typename CODER>
Bitmap<P, CODER>::Bitmap(int w, int h, PixelCoder<P, CODER> * $coder) :
                        Bitmap<P, CODER>::Bitmap(new P[w * h], w, h, $coder) {

}

template<typename P, typename CODER>
Bitmap<P, CODER>::Bitmap(uint8_t *$pixels, int w, int h, PixelCoder<P, CODER> * $coder) :
                        Bitmap<P, CODER>::Bitmap(reinterpret_cast<P *>($pixels), w, h, $coder) {

}

template<typename P, typename CODER>
Bitmap<P, CODER>::~Bitmap() {
    _width = 0;
    _height = 0;
}

template<typename P, typename CODER>
int Bitmap<P, CODER>::width() {
    return _width;
}

template<typename P, typename CODER>
int Bitmap<P, CODER>::height() {
    return _height;
}

template<typename P, typename CODER>
PixelFormat Bitmap<P, CODER>::format() {
    return _coder->format();
}

template<typename P, typename CODER>
P Bitmap<P, CODER>::pixelAt(int x, int y) {
    return this->_data[y*_width + x];
}

template<typename P, typename CODER>
PixelCoder<P, CODER> *Bitmap<P, CODER>::coder() {
    return _coder;
}

template<typename P, typename CODER>
P Bitmap<P, CODER>::pixelAt(int index) {
    return this->_data[index];
}

template<typename P, typename CODER>
void Bitmap<P, CODER>::decodePixelColorAt(int x, int y, color_t &output) {
    _coder->decode(pixelAt(x, y), output);
}

template<typename P, typename CODER>
void Bitmap<P, CODER>::decodePixelColorAt(int index, color_t &output) {
    _coder->decode(pixelAt(index), output);
}

template<typename P, typename CODER>
void Bitmap<P, CODER>::decodePixelToNormalizedColorAt(int x, int y, color_f_t &output) {
    _coder->decode_to_normalized(pixelAt(x, y), output);
}

template<typename P, typename CODER>
void Bitmap<P, CODER>::decodePixelToNormalizedColorAt(int index, color_f_t &output) {
    _coder->decode_to_normalized(pixelAt(index), output);
}


