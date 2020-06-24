//
// Created by tapanthaker on 6/23/20.
//

#include "ObjectFile.h"

template<typename _Predicate>
std::unordered_set<std::shared_ptr<Symbol>>
filter_symbols(std::unordered_set<std::shared_ptr<Symbol>> symbols, _Predicate p) {
    std::unordered_set<std::shared_ptr<Symbol>> filtered_symbols;
    std::copy_if(symbols.begin(), symbols.end(), std::inserter(filtered_symbols, filtered_symbols.end()), p);
    return filtered_symbols;
}

ObjectFile::ObjectFile(std::string filename, std::shared_ptr<ELFIO::elfio> elf_file) {

    auto symbols = findSymbols(elf_file);
    auto relocations = findRelocationEntries(elf_file);

    this->filename = filename;
    this->elf_file = elf_file;
    this->symbols = symbols;
    this->relocations = relocations;
}

std::unordered_set<std::shared_ptr<Symbol>> ObjectFile::declared_symbols() {
    return filter_symbols(symbols, [](std::shared_ptr<Symbol> symbol) {
        return symbol->is_global() && !symbol->is_undefined();
    });
}

std::unordered_set<std::shared_ptr<Symbol>> ObjectFile::required_symbols() {
    return filter_symbols(symbols, [](std::shared_ptr<Symbol> symbol) {
        return symbol->is_global() && symbol->is_undefined();
    });
}

std::shared_ptr<RelocationEntry> ObjectFile::find_relocation(std::shared_ptr<Symbol> symbol) {
    for (auto relocation_entry : relocations) {
        if (relocation_entry->getSymbolName().compare(symbol->symbol_name()) == 0) {
            return relocation_entry;
        }
    }
    return nullptr;
}
