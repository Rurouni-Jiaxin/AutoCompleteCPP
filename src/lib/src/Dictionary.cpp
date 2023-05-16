#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "Dictionary.hpp"

Dictionary_c::Dictionary_c()
{
  _trie = std::make_unique< Trie_c >( 4 );
}

Dictionary_c::~Dictionary_c() = default;

void Dictionary_c::initDictionary( const std::string &filePath )
{
  std::ifstream myFile( filePath.c_str() );
  //myFile.open( filePath, std::ios::in );

  if ( myFile.is_open() )
  {
    std::string line;
    while ( std::getline( myFile, line ) )
    {
      this->_trie->insertWord( line );
    }
    myFile.close();
  }
  else std::cout << "Unable to open file";
}
