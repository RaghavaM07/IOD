#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include "Token.h"

namespace fs = std::filesystem;

namespace IOD
{
    class Document {
    public:
        Document(fs::path filePath): filePath(filePath) {
            static long nextId = 0;
            id = ++nextId;
        }

        // tokenises the file, fills tokens in tokenList and returns number of tokens created
        size_t tokenise();
        long getId() { return this->id; }
        const std::vector<Tokenisation::Token>& getTokens();

    private:
        long id;
        fs::path filePath;
        std::vector<Tokenisation::Token> tokenList;
    };
} // namespace IOD

