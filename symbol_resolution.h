#pragma once

#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <elfio/elfio.hpp>
#include <elfio/elfio_dump.hpp>
#include "utils.h"
#include "Symbol.h"
#include "ObjectFile.h"

using namespace ELFIO;

class SymbolResolutionResult {

public:
    SymbolResolutionResult() {
        resolved_symbols = std::unordered_map<std::string, Symbol *>();
        unresolved_symbols = std::unordered_map<std::string, Symbol *>();
    }

    bool has_undefined_symbols() {
        return !unresolved_symbols.empty();
    }

    void
    print_undefined_symbols_error(const std::unordered_map<std::string, std::shared_ptr<ObjectFile>> &object_files);

    friend SymbolResolutionResult *resolveSymbols(
            const std::unordered_map<std::string, std::shared_ptr<ObjectFile>> &object_files);

private:
    std::unordered_map<std::string, Symbol *> resolved_symbols;
    std::unordered_map<std::string, Symbol *> unresolved_symbols;

    void add_defined_symbols(const std::unordered_set<Symbol *> *defined_symbols);
    void add_undefined_symbols(const std::unordered_set<Symbol *> *undefined_symbols);
};

SymbolResolutionResult *
resolveSymbols(const std::unordered_map<std::string, std::shared_ptr<ObjectFile>> &object_files);
