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
    ObjectFile(const std::string &filename, const std::shared_ptr<ELFIO::elfio> &elfFile,
               const std::unordered_set<std::shared_ptr<Symbol>> &symbols,
               const std::unordered_set<std::shared_ptr<RelocationEntry>> &relocations) : filename(filename),
                                                                                          elf_file(elfFile),
                                                                                          symbols(symbols),
                                                                                          relocations(relocations) {}

    ObjectFile(std::string filename, std::shared_ptr<ELFIO::elfio> elf_file);

    std::unordered_set<std::shared_ptr<Symbol>> declared_symbols();
    std::unordered_set<std::shared_ptr<Symbol>> required_symbols();

    std::shared_ptr<RelocationEntry> find_relocation(std::shared_ptr<Symbol> symbol);

private:
    std::string filename;
    std::shared_ptr<ELFIO::elfio> elf_file;
    std::unordered_set<std::shared_ptr<Symbol>> symbols;
    std::unordered_set<std::shared_ptr<RelocationEntry>> relocations;

};


#endif //YLD_OBJECTFILE_H
