/********************************************************************************
 *  File Name:
 *    container.hpp
 *
 *  Description:
 *    Describes various containers used in Chimera system drivers
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CONTAINER_HPP
#define CHIMERA_CONTAINER_HPP

/* C++ Includes */
#include <algorithm>
#include <cstring>
#include <vector>

namespace Chimera::Container
{
  /**
   *  A container type whose sole purpose is to provide hardware drivers
   *  a fast, low memory overhead way to store mappings between various
   *  resources. This is not intended to replace more generalized mapping
   *  functions in the std lib, but more of a special use case data structure.
   *
   *  In developing STM32 drivers, it was found that there frequently existed
   *  a need to map hardware channel numbers into peripheral addresses, other
   *  addresses into a resource access specifier, etc. These are light-weight,
   *  constant, infrequently looked up data that did not warrant the overhead
   *  caused by a full blown hash mapping data structure. Thus, this class was
   *  born.
   *
   *  If I'm not mistaken, worst case performance of a lookup is O(n) but seeing
   *  as this is intended for low item count (<20), I believe a linear lookup
   *  will likely be faster than hashing. This depends on the comparison function
   *  however.
   */
  template<typename T1, typename T2>
  class LightFlatMap
  {
  public:
    LightFlatMap()
    {
    }

    LightFlatMap( std::initializer_list<std::pair<T1, T2>> list )
    {
      std::copy( list.begin(), list.end(), std::back_inserter(map) );
    }

    /**
     *  Looks up the value associated with a key. If the key does not
     *  exist in the map, an empty value (zero initialized) will be
     *  returned instead.
     *
     *  @note No insertion operation is permitted by design
     *
     *  @param[in]  key   The key to search for
     *  @return T2        The key's value if it exists
     */
    T2 operator[]( const T1 key )
    {
      T2 tempVal;
      memset( &tempVal, 0, sizeof( T2 ) );

      if ( auto x = find( key ); x )
      {
        tempVal = x->second;
      }

      return tempVal;
    }

    /**
     *  Looks up the key-value pair associated with a given key
     *
     *  @param[in]  key   The key to search for
     *  @return std::pair<T1,T2>
     */
    const std::pair<T1, T2> * find( const T1 key ) const 
    {
      /*------------------------------------------------
      For now use a naive O(n) implementation as this class
      isn't meant to hold a large number of elements.
      ------------------------------------------------*/
      for ( size_t x = 0; x < map.size(); x++ )
      {
        if ( map[ x ].first == key )
        {
          return &map[ x ];
        }
      }

      return nullptr;
    }

    void append( const T1 key, const T2 value )
    {
      map.push_back( { key, value } );
    }

    /**
     *  The size of the underlying container
     *
     *  @return size_t
     */
    constexpr size_t size() const
    {
      return map.size();
    }

  private:
    std::vector<std::pair<T1, T2>> map;
  };
}  // namespace Chimera::Container

#endif /* !CHIMERA_CONTAINER_HPP */
