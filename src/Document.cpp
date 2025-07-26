#include <filesystem>
#include <vector>
#include <string>
#include "Document.h"
#include "Token.h"
#include "Tokeniser.h"

namespace fs = std::filesystem;

namespace IOD
{
    size_t Document::tokenise() {
        Tokenisation::Tokeniser tokeniser(filePath);
        tokenList = tokeniser.tokenise();
        return tokenList.size();
    }

    const std::vector<Tokenisation::Token>& Document::getTokens() {
        return this->tokenList;
    }
} // namespace IOD

