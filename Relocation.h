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
#include <utility>

class ObjectFile;


class RelocationEntry {
public:


    void describe();

public:
    RelocationEntry(unsigned long index, unsigned long offset, unsigned long symbolValue, std::string symbolName,
                    unsigned int type, long addend, ObjectFile *const objectFile) : index(index), offset(offset),
                                                                                    symbol_value(symbolValue),
                                                                                    symbol_name(std::move(symbolName)),
                                                                                    type(type), addend(addend),
                                                                                    object_file(objectFile) {}

    const std::string *getSymbolName() const {
        return &symbol_name;
    }

    ObjectFile *getObjectFile() {
        return object_file;
    }

    ELFIO::Elf64_Addr getOffset() const {
        return offset;
    }

private:
    ELFIO::Elf_Xword index;
    ELFIO::Elf64_Addr offset;
    ELFIO::Elf64_Addr symbol_value;
    std::string symbol_name;
    ELFIO::Elf_Word type;
    ELFIO::Elf_Sxword addend;
    ObjectFile *const object_file;

};

std::unordered_map<std::string, std::unordered_set<std::shared_ptr<RelocationEntry>> *> *
generateRelocationEntries(const std::shared_ptr<ELFIO::elfio> &elf_file, ObjectFile *object_file);


#endif //YLD_RELOCATION_H
