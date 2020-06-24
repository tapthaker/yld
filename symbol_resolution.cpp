#include "symbol_resolution.h"
#include <elfio/elfio_symbols.hpp>
#include <elfio/elfio_dump.hpp>
#include <assert.h>
#include "Relocation.h"

bool find_relocation(std::shared_ptr<Symbol> symbol,
                     std::unordered_map<std::string, std::shared_ptr<ObjectFile>> object_files,
                     std::shared_ptr<RelocationEntry> &relocation_result,
                     std::shared_ptr<ObjectFile> &object_file_result) {
    for (auto entry : object_files) {
        std::shared_ptr<RelocationEntry> relocation = entry.second->find_relocation(symbol);
        if (relocation != nullptr) {
            relocation_result = relocation;
            object_file_result = entry.second;
            return true;
        }
    }
    return false;
}

std::string find_symbol_name_referencing() {
    return "";
}

void SymbolResolutionResult::add_defined_symbols(std::unordered_set<std::shared_ptr<Symbol>> defined_symbols) {
    for (auto defined_symbol : defined_symbols) {
        if (unresolved_symbols.count(defined_symbol) == 1) {
            unresolved_symbols.erase(defined_symbol);
        }
        resolved_symbols.insert(defined_symbol);
    }
}

void SymbolResolutionResult::add_undefined_symbols(std::unordered_set<std::shared_ptr<Symbol>> undefined_symbols) {
    for (auto undefined_symbol : undefined_symbols) {
        if (resolved_symbols.count(undefined_symbol) == 1) {
            return;
        }
        unresolved_symbols.insert(undefined_symbol);
    }
}


void SymbolResolutionResult::print_undefined_symbols_error(
        const std::unordered_map<std::string, std::shared_ptr<ObjectFile>>& object_files) {
    for (auto symbol : unresolved_symbols) {
        std::cout << "Undefined Symbol " << symbol->symbol_name() << std::endl;
//        std::shared_ptr<RelocationEntry> *relocation;
//        std::shared_ptr<ObjectFile> *object_file;
//        auto relocation_found = find_relocation(symbol, object_files, relocation, object_file);
//        assert(relocation_found);
    }
}

std::shared_ptr<SymbolResolutionResult> resolveSymbols(
        const std::unordered_map<std::string, std::shared_ptr<ObjectFile>>& object_files) {
    std::shared_ptr<SymbolResolutionResult> symbol_resolution_result = std::make_shared<SymbolResolutionResult>();
    for (auto entry : object_files) {
        symbol_resolution_result->add_defined_symbols(entry.second->declared_symbols());
        symbol_resolution_result->add_undefined_symbols(entry.second->required_symbols());
    }
    return symbol_resolution_result;
}
