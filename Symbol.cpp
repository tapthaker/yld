//
// Created by tapanthaker on 6/22/20.
//

#include "Symbol.h"
#include <elfio/elfio_symbols.hpp>
#include <unordered_set>
#include <memory>

section *get_symbol_section(std::shared_ptr<elfio> reader) {
    for (auto section : reader->sections) {
        if (section->get_type() == SHT_SYMTAB) {
            return section;
        }
    }
    return nullptr;
}

std::unordered_set<std::shared_ptr<Symbol>> findSymbols(std::shared_ptr<elfio> elf_file) {
    std::unordered_set<std::shared_ptr<Symbol>> symbols;
    section *sym_section = get_symbol_section(elf_file);
    const symbol_section_accessor symbols_accessor(*elf_file, sym_section);
    for (unsigned int j = 0; j < symbols_accessor.get_symbols_num(); ++j) {
        std::string name;
        Elf64_Addr value;
        Elf_Xword size;
        unsigned char bind;
        unsigned char type;
        Elf_Half section_index;
        unsigned char other;
        symbols_accessor.get_symbol(j, name, value, size, bind, type, section_index, other);
        std::shared_ptr<Symbol> symbol = std::make_shared<Symbol>(name, value, size, bind, type, section_index, other);
        symbol->describe();
        symbols.insert(symbol);
    }
    return symbols;
}
