#include "symbol_resolution.h"
#include <elfio/elfio_dump.hpp>
#include <cassert>
#include "Relocation.h"

std::unordered_set<std::shared_ptr<RelocationEntry>> *find_relocations(Symbol *const symbol,
                                                                       const std::unordered_map<std::string, std::shared_ptr<ObjectFile>> &object_files) {
    for (const auto &entry : object_files) {
        auto relocation = entry.second->find_relocations(symbol);
        if (relocation != nullptr) {
            return relocation;
        }
    }
    return nullptr;
}

void SymbolResolutionResult::add_defined_symbols(const std::unordered_set<Symbol*> *defined_symbols) {
    for (const auto &defined_symbol : *defined_symbols) {
        if (unresolved_symbols.find(*defined_symbol->symbol_name()) != unresolved_symbols.end()) {
            unresolved_symbols.erase(*defined_symbol->symbol_name());
        }

        resolved_symbols.insert({*defined_symbol->symbol_name(), defined_symbol});
    }
}

void
SymbolResolutionResult::add_undefined_symbols(const std::unordered_set<Symbol*> *undefined_symbols) {
    for (const auto &undefined_symbol : *undefined_symbols) {
        if (resolved_symbols.find(*undefined_symbol->symbol_name()) != resolved_symbols.end()) {
            continue;
        }
        if (*undefined_symbol->symbol_name() == "_GLOBAL_OFFSET_TABLE_") {
            continue;
        }
        unresolved_symbols.insert({*undefined_symbol->symbol_name(), undefined_symbol});
    }
}


void SymbolResolutionResult::print_undefined_symbols_error(
        const std::unordered_map<std::string, std::shared_ptr<ObjectFile>> &object_files) {
    for (const auto &unresolved_symbol_entry : unresolved_symbols) {
        std::unordered_set<std::shared_ptr<RelocationEntry>> *relocation_entries = find_relocations(
                unresolved_symbol_entry.second, object_files);
        assert(relocation_entries);
        std::cout << "Undefined Symbol: "
                  << *unresolved_symbol_entry.second->symbol_name()
                  << std::endl;
        for (const auto &relocation_entry : *relocation_entries) {
            Symbol *referencing_symbol = relocation_entry->getObjectFile()->find_referencing_symbol(
                    relocation_entry.get());
            std::cout << "      Referenced by: "
                      << *referencing_symbol->symbol_name()
                      << std::endl;
        }
    }
}

SymbolResolutionResult *
resolveSymbols(const std::unordered_map<std::string, std::shared_ptr<ObjectFile>> &object_files) {
    auto* symbol_resolution_result = new SymbolResolutionResult();
    for (const auto &entry : object_files) {
        symbol_resolution_result->add_defined_symbols(entry.second->declared_symbols());
        symbol_resolution_result->add_undefined_symbols(entry.second->required_symbols());
    }
    return symbol_resolution_result;
}
