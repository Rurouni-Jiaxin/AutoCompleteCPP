#pragma once

#include<memory>
#include<string>

#include "Trie.hpp"

class Dictionary_c
{
public:
  Dictionary_c();
  ~Dictionary_c();

  void initDictionary( const std::string& );

  std::unique_ptr< Trie_c > _trie;
};