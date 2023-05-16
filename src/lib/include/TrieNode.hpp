#pragma once

#include <unordered_map>

struct TrieNode_t
{
  TrieNode_t() : _isLeaf( false ){}
  TrieNode_t( const TrieNode_t& ) = delete;

  ~TrieNode_t()
  {
    for ( auto &item : _children )
    {
      if ( item.second )
        delete item.second;
    }
  }

  bool _isLeaf;
  std::unordered_map< char, TrieNode_t* > _children;
};