#pragma once

#include <string>

namespace IOD
{
    namespace Tokenisation
    {
        typedef struct Token{
            size_t pos;
            std::string value;
        } Token;
        
    } // namespace Tokenisation
    
} // namespace IOD
