#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class WordTree
{
  public:
    WordTree();
    void add(std::string word);
    bool find(std::string word);
    std::vector<std::string> predict(std::string partial, std::uint8_t howMany);
    std::size_t size() { return m_wordCount; };

  private:
    struct TreeNode
    {
        bool endOfWord = false;
        std::array<std::shared_ptr<TreeNode>, 26> children;
    };

    std::shared_ptr<TreeNode> m_root;
    int m_wordCount;
};
