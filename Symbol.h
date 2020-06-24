//
// Created by tapanthaker on 6/22/20.
//

#ifndef YLD_SYMBOL_H
#define YLD_SYMBOL_H
#include <iostream>
#include <elfio/elfio.hpp>
#include <elfio/elfio_dump.hpp>
#include "utils.h"
#include <unordered_set>
#include <memory>

using namespace ELFIO;

#define STN_COM 65522 // This is not defined by ELFIO so we are defining it

class Symbol {
public:
    Symbol(std::string name, Elf64_Addr value, Elf_Xword size,
           unsigned char bind, unsigned char type, Elf_Half section_index,
           unsigned char other) {
        this->name = name;
        this->value = value;
        this->size = size;
        this->bind = bind;
        this->type = type;
        this->section_index = section_index;
        this->other = other;
    }

    std::string str_type() {
        return symbol_type_table[type].str;
    }

    bool is_function() {
        return type == STT_FUNC;
    }

    bool is_no_type() {
        return type == STT_NOTYPE;
    }

    bool is_object() {
        return type == STT_OBJECT;
    }

    std::string str_bind() {
        return symbol_bind_table[bind].str;
    }

    std::string str_section_index() {
        if(is_undefined()) {
            return "UND";
        } else {
            return std::to_string(section_index);
        }
    }

    bool is_local() {
        return bind == STB_LOCAL;
    }

    bool is_weak() {
        return bind == STB_WEAK;
    }

    bool  is_global() {
        return bind == STB_GLOBAL;
    }

    bool is_undefined() {
        return  ( is_no_type() && section_index == STN_UNDEF ) ||
                ( is_object() && section_index == STN_COM );
    }

    std::string symbol_name() {
        return name;
    }
    void describe() {
        std::cout << " Value: " << int_to_hex(value) << " Size: " << int_to_hex(size) << " Type: "
                  << str_type() << " Bind: " << str_bind()  << " SectionIndex: " <<  str_section_index()
                  << " Name: " << name << std::endl;
    }
private:
    std::string name;
    Elf64_Addr value;
    Elf_Xword size;
    unsigned char bind;
    unsigned char type;
    Elf_Half section_index;
    unsigned char other;
};

std::unordered_set<std::shared_ptr<Symbol>> findSymbols(std::shared_ptr<elfio> elf_file);


#endif //YLD_SYMBOL_H
