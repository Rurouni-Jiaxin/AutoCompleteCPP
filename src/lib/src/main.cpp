/*!
  For string matching, the trie is employed
  A trie is a tree-based data structure for storing strings in order to
  support fast prefix matching.

  Given a set of words(strings), a trie represents these with paths from the root
  to its leaf nodes. A word in the set is allowed to be a prefix of another word.
 */
#include <iostream>
#include <memory>
#include <string>
#include "include/timer.hpp"

#include "Dictionary.hpp"

// oje, global
bool askAgain = false;
tool_n::Timer timer;

const std::string trieTraverseTimer = "trie traverse time";

bool promptUser( const std::string& question )
{
  std::cout << question << " [Y/N]\n";
  while ( true )
  {
    std::string input;
    std::getline(std::cin, input);
    if (input.size() > 0)
    {
      if (input[0] == 'Y' || input[0] == 'y' )
        return true;

      if (input[0] == 'n' || input[0] == 'N')
        return false;
    }
    std::cout << "Please type Y or N then hit Enter\n";
  }
}

void outputResult( const std::vector< std::string >& result )
{
  timer.stop( trieTraverseTimer );
  std::cout << "found " << result.size() << " words with this prefix."
        << std::endl;
  if ( !promptUser("Shall I print them?") )
  {
    askAgain = true;
    return;
  }
  std::cout << "------------------------------------------\n";
  for ( const auto& matchWord : result )
  {
    std::cout << matchWord << "\n";
  }

  std::cout << "------------------------------------------" << std::endl;
  askAgain = true;
}

int main() {
  using namespace std::chrono_literals;

  const std::string filePath =
    "charlesDickens.txt";

  Dictionary_c dictionary;
  dictionary.initDictionary( filePath );

  std::unique_ptr< Trie_c > triePtr = std::move( dictionary._trie );
  const auto cb = std::bind( &outputResult, std::placeholders::_1 );
  triePtr->setCallback( cb );
  std::string prefix;

  do
  {
    std::cout << "Enter a prefix: " << std::endl;
    std::getline( std::cin, prefix );
    timer.start( trieTraverseTimer );
    triePtr->findPrefixMatches( prefix );

    while ( !askAgain )
    {
      std::this_thread::sleep_for( 5ms );
    }
    askAgain = false;
  } while ( promptUser( "Would you like to continue?" ) );
  std::cout << timer << "\n";
}