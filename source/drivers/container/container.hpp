/********************************************************************************
 *  File Name:
 *    container.hpp
 *
 *  Description:
 *    Describes various containers used in Chimera system drivers
 *
 *  2019-2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_CONTAINER_HPP
#define CHIMERA_CONTAINER_HPP

/* STL Includes */
#include <algorithm>
#include <cstring>
#include <iterator>
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
    using value_type = std::pair<T1, T2>;

    LightFlatMap() : emptyObj( {} )
    {
    }

    LightFlatMap( std::initializer_list<value_type> list ) : emptyObj( {} )
    {
      std::copy( list.begin(), list.end(), std::back_inserter( map ) );
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
    T2 operator[]( const T1 &key )
    {
      T2 tempVal = {};

      if ( auto x = at( key ); &x != &emptyObj )
      {
        tempVal = x.second;
      }

      return tempVal;
    }

    /**
     *  Looks up the key-value pair associated with a given key
     *
     *  @param[in]  key   The key to search for
     *  @return std::pair<T1,T2>
     */
    value_type &at( const T1 &key ) 
    {
      /*------------------------------------------------
      For now use a naive O(n) implementation as this class
      isn't meant to hold a large number of elements.
      ------------------------------------------------*/
      for ( size_t x = 0; x < map.size(); x++ )
      {
        if ( map[ x ].first == key )
        {
          return map[ x ];
        }
      }

      return emptyObj;
    }

    const value_type &at( const T1 &key ) const
    {
      /*------------------------------------------------
      For now use a naive O(n) implementation as this class
      isn't meant to hold a large number of elements.
      ------------------------------------------------*/
      for ( size_t x = 0; x < map.size(); x++ )
      {
        if ( map[ x ].first == key )
        {
          return map[ x ];
        }
      }

      return emptyObj;
    }

    /**
     *  Looks up the key-value pair associated with a given value
     *
     *  @param[in]  value     The value to search for
     *  @return std::pair<T1,T2>
     */
    value_type *findWithValue( const T2 &value )
    {
      for ( size_t x = 0; x < map.size(); x++ )
      {
        if ( map[ x ].second == value )
        {
          return &map[ x ];
        }
      }

      return nullptr;
    }

    /**
     *  Checks to see if the given key exists in the map. If it does, will 
     *  return true. Optionally can request that the key/value pair be returned
     *  to the caller via an argument.
     *
     *  @param[in]  key       The key to lookup
     *  @param[in]  element   The key-value pair element associated with the key
     *  @return bool
     */
    bool exists( const T1 &key, value_type **element = nullptr )
    {
      auto tmp = at( key );
      auto result = static_cast<bool>( &tmp != &emptyObj );

      if ( element )
      {
        *element = tmp;
      }

      return result;
    }

    /**
     *  Constructs a new key-value pair and adds it to the map
     *
     *  @param[in]  key     The key to lookup
     *  @param[in]  value   The new value to assign
     *  @return void
     */
    void append( const T1 &key, const T2 &value )
    {
      map.push_back( { key, value } );
    }

    /**
     *  Assigns an existing entry in the map and updates its value
     *
     *  @param[in]  key     The key to lookup
     *  @param[in]  value   The new value to assign
     *  @return void
     */
    void assign( const T1 &key, const T2 &value )
    {
      auto tmp = at( key );

      if ( &tmp != &emptyObj )
      {
        tmp.second = value;
      }
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
    /**
     *  Underlying type that holds the mapping. Using a vector instead of an array
     *  due to issues with being able to pass derived data types into functions.
     *
     *  For example, 3 chips might have register X and there is a function that operates
     *  on that register, taking in a map of configuration options. Chip 1 might have 
     *  eight options, chip 2 four, and chip 3 twelve. There should only be one function
     *  that takes the generic map type, not a function for each option length.
     *
     *  Most of the data this LightFlat map is used for is statically lookup tables, so
     *  no issues of runtime allocation with vectors in embedded systems should occur. This
     *  way we can handle multi-length common table types without issue.
     */
    std::vector<value_type> map;
    value_type emptyObj;
  };
}  // namespace Chimera::Container

#endif /* !CHIMERA_CONTAINER_HPP */

