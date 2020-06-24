#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
#pragma once

#include <microgl/base_bitmap.h>

template <unsigned BPP, typename CODER, bool reverse_bits=false>
class PackedBitmap : public base_bitmap<PackedBitmap<BPP, CODER, reverse_bits>, uint8_t, CODER> {
    using base=base_bitmap<PackedBitmap<BPP, CODER, reverse_bits>, uint8_t, CODER>;
    using byte=unsigned char;
    static constexpr bool is_1_2_4_8 = BPP==1||BPP==2||BPP==4||BPP==8;
    typename std::enable_if<is_1_2_4_8, bool>::type fails_if_else;
    static constexpr byte M = 3;
    static constexpr byte BPE = 1u<<M;
    static constexpr byte K=(BPP==1 ? 0 : (BPP==2 ? 1 : (BPP==4 ? 2 : (BPP==8 ? 3 : 4))));
    static constexpr byte T=M-K;
    static constexpr byte MASK=(BPP==1 ? 0b00000001 : (BPP==2 ? 0b00000011 : (BPP==4 ? 0b00001111 : (BPP==8 ? 0b11111111 : 0b11111111))));
public:
    using base::pixelAt;
    using base::writeAt;

//    template<typename P2, typename CODER2>
//    Bitmap<P2, CODER2> * convertToBitmap() {
//        auto * bmp_2 = new Bitmap<P2, CODER2>(this->_width, this->_height);
//        copyToBitmap(*bmp_2);
//        return bmp_2;
//    }
//
//    template<typename P2, typename CODER2>
//    void copyToBitmap(Bitmap<P2, CODER2> & bmp) {
//        if(bmp.size()!=this->size()) return;
//        const int size = this->size();
//        microgl::color::color_t color_bmp_1, color_bmp_2;
//        for (int index = 0; index < size; ++index) {
//            this->decode(index, color_bmp_1);
//            this->coder().convert(color_bmp_1, color_bmp_2, bmp.coder());
//            bmp.writeColor(index, color_bmp_2);
//        }
//    }

    static
    int pad_to(int val, int bits, int align) {
        int I=(val*bits)%align;
        int R=8-I;
        int extra=R/bits;
        return val+extra;
    }

    PackedBitmap(int w, int h) : PackedBitmap{new uint8_t[(w*h)>>T], w, h} {};
//    PackedBitmap(int w, int h) : PackedBitmap{new uint8_t[(w*h)>>T], w, h} {};
    PackedBitmap(uint8_t* $pixels, int w, int h) : base {$pixels, (w*h)>>T, w, h} {};
    ~PackedBitmap() = default;

    uint8_t extract_pixel(unsigned int index1) const {
        byte mm=M, kk=K, tt=T, mask=MASK; // debug
        unsigned int idx2=(index1)>>T; // index inside the elements array
        byte element= this->_buffer._data[idx2]; // inside this element we need to extract the pixel
        unsigned int R=(index1<<K)-(idx2<<M); // compute distance to the beginning of the block
        element=reverse_bits ? (element)>>(BPE-BPP-R) : (element)>>(R); // move the element to the lower part
        byte masked=element&(MASK); // mask out the upper bits
        return masked<<7;
    }

    uint8_t pixelAt(int index) const {
        return extract_pixel(index);
    }

    void writeAt(int index, const int8_t &value) {
    }

    void fill(const int8_t &value) {
    }

};

#pragma clang diagnostic pop