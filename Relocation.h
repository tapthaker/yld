//
// Created by tapanthaker on 6/21/20.
//

#ifndef YLD_RELOCATION_H
#define YLD_RELOCATION_H

#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <elfio/elfio.hpp>



class RelocationEntry {
public:


    void describe();

public:
    RelocationEntry(unsigned long index, unsigned long offset, unsigned long symbolValue, const std::string &symbolName,
                    unsigned int type, long addend) : index(index), offset(offset),
                                                      symbol_value(symbolValue), symbol_name(symbolName),
                                                      type(type), addend(addend){}

    const std::string getSymbolName() const {
        return symbol_name;
    }
private:
    ELFIO::Elf_Xword index;
    ELFIO::Elf64_Addr offset;
    ELFIO::Elf64_Addr  symbol_value;
    std::string symbol_name;
    ELFIO::Elf_Word    type;
    ELFIO::Elf_Sxword  addend;

};

std::unordered_set<std::shared_ptr<RelocationEntry>>
findRelocationEntries(std::shared_ptr<ELFIO::elfio> elf_file);


#endif //YLD_RELOCATION_H
