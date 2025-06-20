#pragma once

#include <filesystem>
#include <vector>
#include <unordered_set>
#include "Token.h"

namespace fs = std::filesystem;

namespace IOD
{
    namespace Tokenisation
    {        
        static const std::unordered_set<char> trimChars({'.', ',', '?', '!', '-', ';', ' ', '\n', '\r', '\t'});

        class Tokeniser {
        public:
            Tokeniser() {}
            Tokeniser(fs::path file) : filePath(file) {}

            std::vector<Token> tokenise();
            std::vector<Token> tokenise(std::string_view content);

        private:
            fs::path filePath;
        };
    } // namespace Tokenisation
} // namespace IOD
