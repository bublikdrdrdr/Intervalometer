//
// Created by bublik on 11-Aug-18.
//

#ifndef INTERVALOMETER_CHARMAPPER_H
#define INTERVALOMETER_CHARMAPPER_H

#include <stdint-gcc.h>

#define ALIAS_BUFFER 20
#define unsupported 0b01001001

const uint8_t mappings[] = {
        // XGFEDCBA
        0b00000000,  // 32 ->
        0b00110000,  // 33 -> !
        0b00100010,  // 34 -> "
        unsupported, // 35 -> #
        0b01101101,  // 36 -> $
        unsupported, // 37 -> %
        unsupported, // 38 -> &
        0b00100000,  // 39 -> '
        0b00111001,  // 40 -> (
        0b00001111,  // 41 -> )
        0b01100011,  // 42 -> *
        0b01000110,  // 43 -> +
        0b00000100,  // 44 -> ,
        0b01000000,  // 45 -> -
        0b00001000,  // 46 -> .
        0b01010010,  // 47 -> /
        0b00111111,  // 48 -> 0
        0b00000110,  // 49 -> 1
        0b01011011,  // 50 -> 2
        0b01001111,  // 51 -> 3
        0b01100110,  // 52 -> 4
        0b01101101,  // 53 -> 5
        0b01111101,  // 54 -> 6
        0b00000111,  // 55 -> 7
        0b01111111,  // 56 -> 8
        0b01101111,  // 57 -> 9
        unsupported, // 58 -> :
        unsupported, // 59 -> ;
        unsupported, // 60 -> <
        0b01001000,  // 61 -> =
        unsupported, // 62 -> >
        0b01010011,  // 63 -> ?
        unsupported  // 64 -> @
        // 97 -> a
        // 98 -> b
        // 99 -> c
        // 100 -> d
        // 101 -> e
        // 102 -> f
        // 103 -> g
        // 104 -> h
        // 105 -> i
        // 106 -> j
        // 107 -> k
        // 108 -> l
        // 109 -> m
        // 110 -> n
        // 111 -> o
        // 112 -> p
        // 113 -> q
        // 114 -> r
        // 115 -> s
        // 116 -> t
        // 117 -> u
        // 118 -> v
        // 119 -> w
        // 120 -> x
        // 121 -> y
        // 122 -> z
        // 123 -> {
        // 124 -> |
        // 125 -> }
        // 126 -> ~
};

const uint8_t letterMappings[] = {
        0b01110111, // 65 -> A
        0b01111100, // 66 -> B
        0b00111001, // 67 -> C
        0b01011110, // 68 -> D
        0b01111001, // 69 -> E
        0b01110001, // 70 -> F
        0b00111101, // 71 -> G
        0b01110100, // 72 -> H
        0b00110000, // 73 -> I
        0b00011110, // 74 -> J
        0b01110110, // 75 -> K
        0b00111000, // 76 -> L
        0b00010101, // 77 -> M
        0b01010100, // 78 -> N
        0b00111111, // 79 -> O
        0b01110011, // 80 -> P
        0b01100111, // 81 -> Q
        0b01010000, // 82 -> R
        0b01101101, // 83 -> S
        0b01111000, // 84 -> T
        0b00111110, // 85 -> U
        0b00011100, // 86 -> V
        0b00101010, // 87 -> W
        0b01110110, // 88 -> X
        0b01101110, // 89 -> Y
        0b01011011  // 90 -> Z
};

const uint8_t endSymbolsMappings[] = {
        0b00111001, // 91 -> [
        0b01100100, // 92 -> ///
        0b00001111, // 93 -> ]
        0b00100011, // 94 -> ^
        0b00001000, // 95 -> _
        0b00100000  // 96 -> `
};

struct Alias{
    char alias;
    uint8_t symbol;
};

class CharMapper {
public:
    static void setAlias(char alias, uint8_t symbol);
    static void removeAlias(char alias);
    static uint8_t mapToSymbol(char c);
    static void writeLabelToSymbols(const char *label, uint8_t *symbols, int size = -1);
    static void setDotsVisible(uint8_t *symbols, bool visible, int size = 4);

private:
    static Alias aliases[ALIAS_BUFFER];
    static uint8_t getAliasSymbol(char alias);
    static int aliasArraySize;
};




#endif //INTERVALOMETER_CHARMAPPER_H
