#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <deque>
#include "include/TrieNode.hpp"

class Trie_c
{
  using callback = std::function< void( const std::vector< std::string >& ) >;

public:
  Trie_c( size_t num = 0 );
  Trie_c( const Trie_c& ) = delete;
  ~Trie_c();

  void insertWord( const std::string& );
  void findPrefixMatches( const std::string& );

  std::vector<std::string> requestResult() const;

  void setCallback( const callback& cb );

private:
  void traverse( const TrieNode_t*, const std::string&, size_t );
  void startThread( const TrieNode_t*, const std::string&, size_t );

  void pushBackResult( const std::string& );
  void clearResults();

  bool reserveFreeWorker( size_t& index );

  void stopAllWorkers();

  std::unique_ptr< TrieNode_t > _root;

  bool _stopAllWorkers = false;
  // size_t _numRunningWorkers = 0;
  mutable std::mutex _accessWorkers;
  std::vector< std::thread > _workers;
  std::vector< std::thread > _dump;
  //std::deque< bool > _idleWorkers;
  std::deque< size_t > _idleWorkers;
  mutable std::mutex _accessResults;
  std::vector< std::string > _results;

  TrieNode_t* _reachedNode;
  size_t _numWorkers;
  std::string _input = "";
  callback onFinnishedSearch = []( const std::vector< std::string >& ) {};
};
