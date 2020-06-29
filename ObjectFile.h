//
// Created by tapanthaker on 6/23/20.
//

#ifndef YLD_OBJECTFILE_H
#define YLD_OBJECTFILE_H

#include <elfio/elfio.hpp>
#include "Symbol.h"
#include "Relocation.h"


class ObjectFile {

public:

    ObjectFile(std::string filename, std::shared_ptr<ELFIO::elfio> elf_file);

    const std::unordered_set<Symbol *> *declared_symbols() const;

    const std::unordered_set<Symbol *> *required_symbols() const;

    std::unordered_set<std::shared_ptr<RelocationEntry>> *find_relocations(Symbol *symbol);

    Symbol *find_referencing_symbol(RelocationEntry *relocationEntry);

private:
    std::string filename;
    std::shared_ptr<ELFIO::elfio> elf_file;
    std::unordered_set<std::shared_ptr<Symbol>> symbols;
    std::unordered_map<std::string, std::unordered_set<std::shared_ptr<RelocationEntry>> *> *relocations;

};


#endif //YLD_OBJECTFILE_H
