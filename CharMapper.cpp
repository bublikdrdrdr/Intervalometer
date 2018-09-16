//
// Created by bublik on 11-Aug-18.
//

#include "CharMapper.h"

int CharMapper::aliasArraySize = 0;

Alias CharMapper::aliases[ALIAS_BUFFER];

void CharMapper::setAlias(char alias, uint8_t symbol) {
    if (aliasArraySize >= ALIAS_BUFFER) return;
    for (int i = 0; i < aliasArraySize; i++) {
        if (CharMapper::aliases[i].alias == alias) {
            CharMapper::aliases[i].symbol = symbol;
            return;
        }
    }
    CharMapper::aliases[aliasArraySize].alias = alias;
    CharMapper::aliases[aliasArraySize].symbol = symbol;
    aliasArraySize++;
}

void CharMapper::removeAlias(char alias) {
    for (int i = 0; i < aliasArraySize; i++) {
        if (CharMapper::aliases[i].alias == alias) {
            if (i != aliasArraySize - 1) {
                CharMapper::aliases[i].alias = CharMapper::aliases[aliasArraySize - 1].alias;
                CharMapper::aliases[i].symbol = CharMapper::aliases[aliasArraySize - 1].symbol;
            }
            aliasArraySize--;
            return;
        }
    }
}

uint8_t CharMapper::mapToSymbol(char c) {
    uint8_t fromAlias = getAliasSymbol(c);
    if (fromAlias != unsupported) {
        return fromAlias;
    }
    if (c >= 32) {
        if (c < 65) {
            return mappings[c - 32];
        } else if (c < 90) {
            return letterMappings[c - 65];
        } else if (c < 97) {
            return endSymbolsMappings[c - 91];
        } else if (c < 123) {
            return letterMappings[c - 97];
        } else {
            return unsupported;
        }
    }
    return unsupported;
}

void CharMapper::writeLabelToSymbols(const char *label, uint8_t *symbols, int size) {
    for (int i = 0; size < 0 ? label[i] != '\0' : i < size; i++) {
        symbols[i] = mapToSymbol(label[i]);
    }
}

void CharMapper::setDotsVisible(uint8_t *symbols, bool visible, int size) {
    for (int i = 0; i < size; i++) {
        if (visible) {
            symbols[i] |= 0b10000000;
        } else {
            symbols[i] &= 0b01111111;
        }
    }
}

uint8_t CharMapper::getAliasSymbol(char alias) {
    for (int i = 0; i < aliasArraySize; i++) {
        if (CharMapper::aliases[i].alias == alias) {
            return CharMapper::aliases[i].symbol;
        }
    }
    return unsupported;
}
