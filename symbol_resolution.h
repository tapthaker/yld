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
        resolved_symbols = std::unordered_set<std::shared_ptr<Symbol>>();
        unresolved_symbols = std::unordered_set<std::shared_ptr<Symbol>>();
    }

    bool has_undefined_symbols() {
       return unresolved_symbols.size() > 0;
    }

    void print_undefined_symbols_error(const std::unordered_map<std::string, std::shared_ptr<ObjectFile>>& object_files);

    friend std::shared_ptr<SymbolResolutionResult> resolveSymbols(
            const std::unordered_map<std::string, std::shared_ptr<ObjectFile>>& object_files);

private:
    std::unordered_set<std::shared_ptr<Symbol>> resolved_symbols;
    std::unordered_set<std::shared_ptr<Symbol>> unresolved_symbols;

    void add_defined_symbols(std::unordered_set<std::shared_ptr<Symbol>> defined_symbols);
    void add_undefined_symbols(std::unordered_set<std::shared_ptr<Symbol>> undefined_symbols);
};

std::shared_ptr<SymbolResolutionResult> resolveSymbols(
        const std::unordered_map<std::string, std::shared_ptr<ObjectFile>>& object_files);
