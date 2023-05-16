#include <chrono>
#include <memory>
#include <string>

#include "Trie.hpp"

Trie_c::Trie_c( size_t num ) : _numWorkers( num )
{
  _root = std::make_unique< TrieNode_t >();
  if ( _numWorkers == 0 )
    _numWorkers = 1;

  _workers.resize( _numWorkers );

  // _idleWorkers is a deque so as to avoid vector<bool>
  for ( size_t i = 0; i < _numWorkers; ++i )
  {
    //_idleWorkers.push_back( true );
    _idleWorkers.push_back( i );
    _workers[ i ] = std::thread();
  }
}

Trie_c::~Trie_c() = default;

void Trie_c::insertWord( const std::string & word )
{
  TrieNode_t* nodePtr = _root.get();

  for ( const char letter : word )
  {
    if ( !nodePtr->_children.count( letter ) )
      nodePtr->_children[ letter ] = new TrieNode_t();

    // point to new child node
    nodePtr = nodePtr->_children[ letter ];
  }
  nodePtr->_isLeaf = true;
}

/*!
  Private helper function to perform depth-first traversal, a.k.a pre-order traversal
  Given the root of the sub-trie under prefix, traverse all nodes in depth-first order
  to reach the leaves
  */
void Trie_c::traverse( const TrieNode_t * rootSubT, const std::string & word,
    size_t workerIndex )
{
  if ( rootSubT->_isLeaf )
    pushBackResult( word );

  if ( !rootSubT->_children.empty() )
  {
    if ( rootSubT->_children.size() == 1 )  // No need for other threads
    {
      const std::string temp = word + rootSubT->_children.begin()->first;
      const auto nodePtr = rootSubT->_children.begin()->second;
      traverse( nodePtr, temp, workerIndex );
    }
    else // Other threads could help, and our paths diverge
    {
      for ( const auto& [ letter, tnPtr ] : rootSubT->_children )
      {
        if ( _stopAllWorkers )
          break;

        const std::string temp = word + letter;
        size_t newCandidateWorkerId;
        if ( reserveFreeWorker( newCandidateWorkerId ) )
        {
          // The candidate finished executing code yet still active.
          if ( _workers[ newCandidateWorkerId ].joinable() )
            _dump.emplace_back( std::move( _workers[ newCandidateWorkerId ] ) );

          _workers[ newCandidateWorkerId ] = std::thread(
             &Trie_c::startThread, this, tnPtr, temp, newCandidateWorkerId );
          _workers[ newCandidateWorkerId ].detach();
        }
        else
        {
          // ok, everyone busy. I will do it myself...
          traverse( tnPtr, temp, workerIndex );
        }
      }
    }
  }
}

void Trie_c::startThread( const TrieNode_t * rootSubT, const std::string & word,
  size_t workerId )
{
  traverse( rootSubT, word, workerId );

  std::lock_guard< std::mutex > guard( _accessWorkers );
  _idleWorkers.push_back( workerId );

  if ( _idleWorkers.size() == _numWorkers )
    // the last worker calls the callback function
    onFinnishedSearch( _results );
}

void Trie_c::setCallback( const callback & cb ) { onFinnishedSearch = cb; }

std::vector<std::string> Trie_c::requestResult() const {
    std::lock_guard< std::mutex > guard( _accessResults );
    return _results;
}

void Trie_c::findPrefixMatches( const std::string & prefix ) {
    _input = prefix;
    _reachedNode = _root.get();

    // todo: we could check whether restart is really necessary.
    stopAllWorkers();
    clearResults();

    for ( const char letter : prefix )
    {
      if ( !_reachedNode->_children.count( letter ) )
      {
        // Callback: wait for search to finish and print results
        onFinnishedSearch( _results );
        return;
      }
      _reachedNode = _reachedNode->_children.at( letter );
    }

    size_t index{ 0 };
    if ( !reserveFreeWorker( index ) )
      return;

    _workers[ index ] = std::thread( &Trie_c::startThread, this, _reachedNode,
        prefix, index );
    _workers[ index ].detach();
}

void Trie_c::pushBackResult( const std::string & word ) {
    std::lock_guard< std::mutex > guard( _accessResults );
    _results.push_back( word );
}

void Trie_c::clearResults() {
    std::lock_guard< std::mutex > guard( _accessResults );
    _results.clear();
    _dump.clear();
}

bool Trie_c::reserveFreeWorker( size_t & index )
{
  using namespace std::chrono_literals;
  std::lock_guard< std::mutex > guard( _accessWorkers );

  if ( _idleWorkers.empty() )
  {
    return false;
  }
  else
  {
    index = _idleWorkers.front();
    _idleWorkers.pop_front();
    return true;
  }
  return false;
}

void Trie_c::stopAllWorkers() {
    _stopAllWorkers = true;
    while ( _idleWorkers.size() < _numWorkers ) {
        []() {}();
    }

    _stopAllWorkers = false;
}