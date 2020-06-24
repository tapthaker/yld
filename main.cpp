#include <iostream>
#include <elfio/elfio.hpp>
#include <unordered_map>
#include <memory>
#include "symbol_resolution.h"
#include "ObjectFile.h"

using namespace ELFIO;

std::unordered_map<std::string, std::shared_ptr<ObjectFile>>
generate_object_files(const std::unordered_map<std::string, std::shared_ptr<elfio>>& elf_files) {

    std::unordered_map<std::string, std::shared_ptr<ObjectFile>> object_files;
    for (auto entry : elf_files) {
        auto object_file = std::make_shared<ObjectFile>(entry.first, entry.second);
        object_files.insert({entry.first, object_file});
    }
    return object_files;
}

std::unordered_map<std::string, std::shared_ptr<elfio>> read_elf_files(int argc, char** argv ) {

    std::unordered_map<std::string, std::shared_ptr<elfio>> elf_files;

    for (int i = 1; i < argc; i++) {
        std::string file_path = argv[i];
        std::shared_ptr<elfio> reader = std::make_shared<elfio>();
        bool success = reader->load(file_path);
        if (!success) {
            std::cout << "Could not load file: " << argv[argc] << std::endl;
            exit(1);
        }
        elf_files.insert({file_path, reader});
    }

    return elf_files;
}

bool validate_elf_files(const std::unordered_map<std::string, std::shared_ptr<elfio>>& elf_files) {
    for (auto elf_file_pair : elf_files) {
        if (elf_file_pair.second->get_class() != ELFCLASS64) {
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <elf_files>" << std::endl;
        return 1;
    }

    auto elf_files = read_elf_files(argc, argv);
    bool isValid = validate_elf_files(elf_files);
    if (!isValid) {
        return 1;
    }

    const std::unordered_map<std::string, std::shared_ptr<ObjectFile>> object_files = generate_object_files(elf_files);
    auto resolve_symbols_result = resolveSymbols(object_files);
    if(resolve_symbols_result->has_undefined_symbols()) {
        resolve_symbols_result->print_undefined_symbols_error(object_files);
        return 1;
    }

    return 0;
}

