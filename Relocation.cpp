//
// Created by tapanthaker on 6/21/20.
//

#include "Relocation.h"
#include <elfio/elfio_relocation.hpp>
#include "utils.h"

struct relocation_type_table_t {
    ELFIO::Elf_Word type;
    const char *str;
}
        relocation_type_table[] = {
        {R_X86_64_NONE,            "R_X86_64_NONE"},
        {R_X86_64_64,              "R_X86_64_64"},
        {R_X86_64_PC32,            "R_X86_64_PC32"},
        {R_X86_64_GOT32,           "R_X86_64_GOT32"},
        {R_X86_64_PLT32,           "R_X86_64_PLT32"},
        {R_X86_64_COPY,            "R_X86_64_COPY"},
        {R_X86_64_GLOB_DAT,        "R_X86_64_GLOB_DAT"},
        {R_X86_64_JUMP_SLOT,       "R_X86_64_JUMP_SLOT"},
        {R_X86_64_RELATIVE,        "R_X86_64_RELATIVE"},
        {R_X86_64_GOTPCREL,        "R_X86_64_GOTPCREL"},
        {R_X86_64_32,              "R_X86_64_32"},
        {R_X86_64_32S,             "R_X86_64_32S"},
        {R_X86_64_16,              "R_X86_64_16"},
        {R_X86_64_PC16,            "R_X86_64_PC16"},
        {R_X86_64_8,               "R_X86_64_8"},
        {R_X86_64_PC8,             "R_X86_64_PC8"},
        {R_X86_64_DTPMOD64,        "R_X86_64_DTPMOD64"},
        {R_X86_64_DTPOFF64,        "R_X86_64_DTPOFF64"},
        {R_X86_64_TPOFF64,         "R_X86_64_TPOFF64"},
        {R_X86_64_TLSGD,           "R_X86_64_TLSGD"},
        {R_X86_64_TLSLD,           "R_X86_64_TLSLD"},
        {R_X86_64_DTPOFF32,        "R_X86_64_DTPOFF32"},
        {R_X86_64_GOTTPOFF,        "R_X86_64_GOTTPOFF"},
        {R_X86_64_TPOFF32,         "R_X86_64_TPOFF32"},
        {R_X86_64_PC64,            "R_X86_64_PC64"},
        {R_X86_64_GOTOFF64,        "R_X86_64_GOTOFF64"},
        {R_X86_64_GOTPC32,         "R_X86_64_GOTPC32"},
        {R_X86_64_GOT64,           "R_X86_64_GOT64"},
        {R_X86_64_GOTPCREL64,      "R_X86_64_GOTPCREL64"},
        {R_X86_64_GOTPC64,         "R_X86_64_GOTPC64"},
        {R_X86_64_GOTPLT64,        "R_X86_64_GOTPLT64"},
        {R_X86_64_PLTOFF64,        "R_X86_64_PLTOFF64"},
        {R_X86_64_GOTPC32_TLSDESC, "R_X86_64_GOTPC32_TLSDESC"},
        {R_X86_64_TLSDESC_CALL,    "R_X86_64_TLSDESC_CALL"},
        {R_X86_64_TLSDESC,         "R_X86_64_TLSDESC"},
        {R_X86_64_IRELATIVE,       "R_X86_64_IRELATIVE"},
        {R_X86_64_GNU_VTINHERIT,   "R_X86_64_GNU_VTINHERIT"},
        {R_X86_64_GNU_VTENTRY,     "R_X86_64_GNU_VTENTRY"}
};


void RelocationEntry::describe() {
    std::printf("[%4lx] %s %s %s %s %s\n",
                index, int_to_hex(offset).c_str(), int_to_hex(symbol_value).c_str(), symbol_name.c_str(),
                relocation_type_table[type].str, int_to_hex(addend).c_str());
}

std::unordered_map<std::string, std::unordered_set<std::shared_ptr<RelocationEntry>> *> *
generateRelocationEntries(const std::shared_ptr<ELFIO::elfio> &elf_file, ObjectFile *const object_file) {
    auto relocation_entries =
            new std::unordered_map<std::string, std::unordered_set<std::shared_ptr<RelocationEntry>> *>();
    for (auto section : elf_file->sections) {
        if (section->get_type() == SHT_RELA || section->get_type() == SHT_REL) {
            ELFIO::relocation_section_accessor relocation_section_accessor(*elf_file, section);
            for (ELFIO::Elf_Xword index = 0; index < relocation_section_accessor.get_entries_num(); ++index) {
                ELFIO::Elf64_Addr offset;
                ELFIO::Elf64_Addr symbolValue;
                std::string symbolName;
                ELFIO::Elf_Word type;
                ELFIO::Elf_Sxword addend;
                ELFIO::Elf_Sxword calcValue; // CalcValue is dropped because it is not computed for x86_64 by ELFIO
                relocation_section_accessor.get_entry(index, offset, symbolValue, symbolName, type, addend,
                                                      calcValue);
                std::shared_ptr<RelocationEntry> relocation_entry = std::make_shared<RelocationEntry>(index, offset,
                                                                                                      symbolValue,
                                                                                                      symbolName,
                                                                                                      type, addend,
                                                                                                      object_file);
                relocation_entry->describe();
                auto relocation_entry_set = relocation_entries->find(*relocation_entry->getSymbolName());
                if (  relocation_entry_set != relocation_entries->end()) {
                    (*relocation_entry_set).second->insert(relocation_entry);
                } else {
                    auto *relocations = new std::unordered_set<std::shared_ptr<RelocationEntry>>();
                    relocations->insert(relocation_entry);
                    relocation_entries->insert(std::make_pair(*relocation_entry->getSymbolName(), relocations));
                }
            }
        }
    }
    return relocation_entries;
}
