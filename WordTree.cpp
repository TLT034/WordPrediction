#include "WordTree.hpp"

#include <iostream>
#include <queue>
#include <tuple>

const int ASCII_OFFSET = -97;

WordTree::WordTree()
{
    m_wordCount = 0;
    m_root = std::make_shared<TreeNode>();
}

void WordTree::add(std::string word)
{
    // queue to hold each level of treenode
    std::queue<std::shared_ptr<TreeNode>> q;
    q.push(m_root);
    std::string::size_type i = 0;

    while (!q.empty() && i < word.length())
    {
        int letterIdx = word[i] + ASCII_OFFSET;

        // get the next level of treenode from the queue, then remove it from the queue
        std::shared_ptr<TreeNode> tempNode = q.front();
        q.pop();

        // if the treenode's child at the specified letter, is not null, then the child has already been created and we add it to the queue.
        // if it is null, then set it to a new treenode
        if (tempNode->children[letterIdx] != nullptr)
        {
            //std::cout << "Found: " << char(letterIdx - ASCII_OFFSET) << " while adding: " << word << std::endl;
            q.push(tempNode->children[letterIdx]);
        }
        else
        {
            //std::cout << "Adding: " << char(letterIdx - ASCII_OFFSET) << " from: " << word << std::endl;
            tempNode->children[letterIdx] = std::make_shared<TreeNode>();

            q.push(tempNode->children[letterIdx]);
            // if we are at the end of the word, then mark the end of word and increment the word count
            if (i == word.length() - 1)
            {
                tempNode->children[letterIdx]->endOfWord = true;
                m_wordCount++;
            }
        }
        i++;
    }
}

bool WordTree::find(std::string word)
{
    std::queue<std::shared_ptr<TreeNode>> q;
    q.push(m_root);
    std::string::size_type i = 0;

    while (!q.empty() && i < word.length())
    {
        int letterIdx = word[i] + ASCII_OFFSET;

        std::shared_ptr<TreeNode> tempNode = q.front();
        q.pop();

        if (tempNode->children[letterIdx] != nullptr)
        {
            //std::cout << "Found: " << char(letterIdx - ASCII_OFFSET) << std::endl;
            if (tempNode->children[letterIdx]->endOfWord && i == word.length() - 1)
            {
                return true;
            }
            q.push(tempNode->children[letterIdx]);
        }
        i++;
    }

    return false;
}

std::vector<std::string> WordTree::predict(std::string partial, std::uint8_t howMany)
{
    std::vector<std::string> predictions;
    if (partial.empty())
    {
        return predictions;
    }

    // create a root at the end of the partial string
    auto partialRoot = m_root;
    for (std::string::size_type i = 0; i < partial.length(); i++)
    {
        int letterIdx = partial[i] + ASCII_OFFSET;
        if (partialRoot->children[letterIdx] != nullptr)
        {
            partialRoot = partialRoot->children[letterIdx];
        }
        else
        {
            return predictions;
        }
    }

    std::queue<std::pair<std::shared_ptr<TreeNode>, std::string>> q;
    q.push(std::pair(partialRoot, partial));

    while (!q.empty() && predictions.size() < howMany)
    {
        std::shared_ptr<TreeNode> tempNode = q.front().first;
        std::string currWord = q.front().second;
        q.pop();

        if (tempNode->endOfWord && currWord != partial)
        {
            predictions.push_back(currWord);
        }

        for (std::size_t i = 0; i < tempNode->children.size(); i++)
        {
            if (tempNode->children[i] != nullptr)
            {
                q.push(std::pair(tempNode->children[i], currWord + char(i - ASCII_OFFSET)));
            }
        }
    }

    return predictions;
}
