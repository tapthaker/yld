//
// Created by tapanthaker on 6/23/20.
//

#include "ObjectFile.h"

template<typename Condition>
std::unordered_set<Symbol *> *
filter_symbols(const std::unordered_set<std::shared_ptr<Symbol>> &symbols, Condition condition) {
    std::unordered_set<Symbol*> *filtered_symbols = new std::unordered_set<Symbol *>();
    for (const auto &symbol : symbols) {
        if (condition(symbol) == true) {
            filtered_symbols->insert(symbol.get());
        }
    }
    return filtered_symbols;
}

ObjectFile::ObjectFile(std::string filename, std::shared_ptr<ELFIO::elfio> elf_file) {

    auto found_symbols = findSymbols(elf_file);
    std::unordered_map<std::string, std::unordered_set<std::shared_ptr<RelocationEntry>> *> *generated_relocations = generateRelocationEntries(
            elf_file, this);
    this->filename = filename;
    this->elf_file = elf_file;
    this->symbols = found_symbols;
    this->relocations = generated_relocations;
}

const std::unordered_set<Symbol *> *ObjectFile::declared_symbols() const {
    return filter_symbols(symbols, [](const std::shared_ptr<Symbol> &symbol) {
        return symbol->is_global() && !symbol->is_undefined();
    });
}

const std::unordered_set<Symbol *> *ObjectFile::required_symbols() const {
    return filter_symbols(symbols, [](const std::shared_ptr<Symbol> &symbol) {
        return symbol->is_global() && symbol->is_undefined();
    });
}

std::unordered_set<std::shared_ptr<RelocationEntry>> *ObjectFile::find_relocations(Symbol *const symbol) {
    auto relocs = relocations->find(*symbol->symbol_name());
    if (relocs != relocations->end()) {
        return relocs->second;
    } else {
        return nullptr;
    }
}

Symbol *ObjectFile::find_referencing_symbol(RelocationEntry *const relocationEntry) {

    Symbol *referencing_symbol = nullptr;
    Elf64_Off nearest_symbol_offset = UINT64_MAX;
    std::unordered_set<std::shared_ptr<Symbol>>::iterator iterator;
    for (iterator = symbols.begin(); iterator != symbols.end(); iterator++) {
        if (iterator->get()->is_undefined() ||
            !iterator->get()->is_function()) { // We need nearest offset from defined symbols only
            continue;
        }
        Elf64_Off symbol_offset = relocationEntry->getOffset() - iterator->get()->getValue();
        if (symbol_offset > 0 && symbol_offset < nearest_symbol_offset) {
            nearest_symbol_offset = symbol_offset;
            referencing_symbol = iterator->get();
        }
    }

    return referencing_symbol;
}
