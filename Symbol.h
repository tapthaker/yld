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
#include <utility>

using namespace ELFIO;

#define STN_COM 65522 // This is not defined by ELFIO so we are defining it

class Symbol {
public:
    Symbol(std::string name, Elf64_Addr value, Elf_Xword size,
           unsigned char bind, unsigned char type, Elf_Half section_index,
           unsigned char other) : name(std::move(name)), value(value), size(size), bind(bind), type(type),
                                  section_index(section_index), other(other) {}

    std::string str_type() const {
        return symbol_type_table[type].str;
    }

    bool is_function() const {
        return type == STT_FUNC;
    }

    bool is_no_type() const {
        return type == STT_NOTYPE;
    }

    bool is_object() const {
        return type == STT_OBJECT;
    }

    std::string str_bind() const {
        return symbol_bind_table[bind].str;
    }

    std::string str_section_index() const {
        if(is_undefined()) {
            return "UND";
        } else {
            return std::to_string(section_index);
        }
    }

    bool is_local() const {
        return bind == STB_LOCAL;
    }

    bool is_weak() const {
        return bind == STB_WEAK;
    }

    bool  is_global() const {
        return bind == STB_GLOBAL;
    }

    bool is_undefined() const {
        return  ( is_no_type() && section_index == STN_UNDEF ) ||
                ( is_object() && section_index == STN_COM );
    }

    const std::string* symbol_name() const {
        return &name;
    }
    void describe() const {
        std::cout << " Value: " << int_to_hex(value) << " Size: " << int_to_hex(size) << " Type: "
                  << str_type() << " Bind: " << str_bind()  << " SectionIndex: " <<  str_section_index()
                  << " Name: " << name << std::endl;
    }

    Elf64_Addr getValue() const {
        return value;
    }

    bool operator==(const Symbol &rhs) const {
        return name == rhs.name &&
               value == rhs.value &&
               size == rhs.size &&
               bind == rhs.bind &&
               type == rhs.type &&
               section_index == rhs.section_index &&
               other == rhs.other;
    }

    bool operator!=(const Symbol &rhs) const {
        return !(rhs == *this);
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

std::unordered_set<std::shared_ptr<Symbol>> findSymbols(const std::shared_ptr<elfio>& elf_file);


#endif //YLD_SYMBOL_H
