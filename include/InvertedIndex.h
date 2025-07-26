#pragma once

#include <unordered_map>
#include <vector>
#include <filesystem>
#include <string>
#include "Document.h"

namespace fs = std::filesystem;

namespace IOD
{
    namespace SerDe
    {
        // A posting for a particular token refers to the map of all docIds where the token
        // is present to the positions in the respective doc
        typedef std::unordered_map<long, std::vector<size_t>> Posting;

        typedef struct DocInfo {
            long docId;
            fs::path docPath;
            size_t tokCount;
        } DocInfo;

        class InvertedIndex
        {
        public:
            InvertedIndex() { }

            void addDocToIndex(Document doc);
            void serialise(std::string outFileName);
            static InvertedIndex deserialise(std::string inFileName);

            // Number of docs in index
            size_t totalDocs()         const { return documentSet.size(); }

            // Number of tokens in `docId`
            size_t docLength(long id)  const { return documentSet.at(id).tokCount; }

            // Number of docs with `term`
            size_t docFreq(const std::string& term) const {
                auto it = invIndex.find(term);
                return it==invIndex.end()? 0 : it->second.size();
            }

            // Number of occurences of `term` in `docId`
            size_t termFreq(long docId, const std::string& term) const {
                auto pit = invIndex.find(term);
                if(pit==invIndex.end()) return 0;
                auto dit = pit->second.find(docId);
                return dit==pit->second.end() ? 0 : dit->second.size();
            }

            // Average number of tokens per doc
            double avgDocLength() const {
                double sum=0;
                for(auto& [id,info]: documentSet) sum += info.tokCount;
                return sum / documentSet.size();
            }

            std::unordered_map<std::string, Posting> getIndex() { return invIndex; }
            std::unordered_map<long, DocInfo> getDocumentSet() { return documentSet; }
            DocInfo getDocById(long id) { return documentSet.at(id); }

        private:
            std::unordered_map<long, DocInfo> documentSet;
            std::unordered_map<std::string, Posting> invIndex;
        };
    } // namespace SerDe
} // namespace IOD

