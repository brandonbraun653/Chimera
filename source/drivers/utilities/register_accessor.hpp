/********************************************************************************
 *  File Name:
 *    register_accessor.hpp
 *
 *  Description:
 *    Template metaprogramming for generating utility classes to define and
 *    interact with hardware registers.
 *
 *  2021 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef CHIMERA_REGISTER_ACCESSOR_HPP
#define CHIMERA_REGISTER_ACCESSOR_HPP

/* STL Includes */
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

#if defined( EMBEDDED )
#define __IO volatile
#else
#define __IO
#endif

namespace Chimera::Gen
{
  /*-------------------------------------------------------------------------------
  Helpers for defining a peripheral memory mapped region
  -------------------------------------------------------------------------------*/
  template<typename T, const T StartAddr, const T EndAddr>
  class MemRegion
  {
  public:
    static constexpr T StartAddress = StartAddr;
    static constexpr T EndAddress = EndAddr;
    static constexpr T MemoryRange = EndAddr - StartAddr;

    static_assert( StartAddr % sizeof( T) == 0 );
    static_assert( EndAddr % sizeof( T ) == 0 );
  };

  /*-------------------------------------------------------------------------------
  Helpers for generating register/field access classes
  -------------------------------------------------------------------------------*/
  /**
   *  Register descriptor. This can apply to either an entire register or
   *  a field inside a register.
   *
   *  @see RegAccess
   *  @see FieldAccess
   *
   *  @param[in]  T         Base type that the register is sized for
   *  @param[in]  Addr      Memory mapped address for the register
   *  @param[in]  Mask      Mask that identifies which bits are accessible (default all)
   *  @param[in]  Reset     Reset value for the register (default 0)
   *  @param[in]  Offset    Bit position that the register starts at (default 0)
   */
  template<typename T, const T Addr, const T Mask = std::numeric_limits<T>::max(), const T Reset = 0, const T Offset = 0>
  class RegDecl
  {
  public:
    /*-------------------------------------------------
    Assumptions & Checks
    -------------------------------------------------*/
    /* clang-format off */
    static_assert( std::is_pod<T>::value,                   "Register type must be P.O.D" );
    static_assert( Addr != static_cast<T>( 0 ),             "Register assigned to NULL. This is probably not true." );
    static_assert( Addr % sizeof( T ) == 0,                 "Register address is not aligned to its underlying type." );
    static_assert( Offset < std::numeric_limits<T>::digits, "Bit offset larger than what type can contain." );
    static_assert( Mask != static_cast<T>( 0 ),             "Mask is zero. This disables all functionality!" );
    /* clang-format on */

    /*-------------------------------------------------
    Public Fields
    -------------------------------------------------*/
    static constexpr __IO T *_accessor = static_cast<__IO T *>( Addr ); /**< Memory mapped address */
    static constexpr T _address        = Addr;                          /**< Raw address value */
    static constexpr T _bitOffset      = Offset;                        /**< Starting bit position for this field */
    static constexpr T _bitMask        = Mask;                          /**< Bit mask for this field */
    static constexpr T _resetValue     = Reset;                         /**< Value of the field upon reset */
  };

  /**
   *  Register access helper functions
   *
   *  @param[in]  T         Base type that the register is sized for
   *  @param[in]  Addr      Memory mapped address for the register
   *  @param[in]  Mask      Mask that identifies which bits are accessible
   *  @param[in]  Reset     Reset value for the register
   *  @param[in]  Offset    Bit position that the register starts at (always zero)
   */
  template<typename T, const T Addr, const T Mask = std::numeric_limits<T>::max(), const T Reset = 0,
           const T Offset = 0>
  class RegAccess : public RegDecl<T, Addr, Mask, Reset, Offset>
  {
  public:
    /*-------------------------------------------------------------------------------
    Public Functions
    -------------------------------------------------------------------------------*/
    /**
     *  Reads the current state of the register, masked appropriately
     *  @return T
     */
    inline T read() const
    {
      return ( *_accessor ) & _bitMask;
    }

    /**
     *  Assigns the entire register to the given value
     *
     *  @param[in]  val     Data to be assigned
     *  @return void
     */
    inline void write( const T val )
    {
      *_accessor = val & _bitMask;
    }

    /**
     *  Sets the entire register to zero
     *  @return void
     */
    inline void clear()
    {
      write( 0 );
    }

    /**
     *  Writes the reset value to the entire register
     *  @return void
     */
    inline void reset()
    {
      write( _resetValue );
    }

    /**
     *  Sets the bits in the given value, assuming it makes it through
     *  the register mask.
     *
     *  @param[in]  val     Bit values to set
     *  @return void
     */
    inline void bitwiseSet( const T val )
    {
      *_accessor |= val & _bitMask;
    }

    /**
     *  Clears the bits that are set in the given value, if supported
     *  by the register mask
     *
     *  @param[in]  val     Bit values to clear
     *  @return void
     */
    inline void bitwiseClear( const T val )
    {
      *_accessor &= ~( val & _bitMask );
    }
  };


  /**
   *  Helper class to declare a register access type that describes a sub
   *  field of an entire register.
   *
   *  @param[in]  Base      Base register declaration, of type RegAccess
   *  @param[in]  Mask      Mask that identifies which bits belong to the field
   *  @param[in]  Reset     Reset value for the field
   *  @param[in]  Offset    Bit position that the field starts at
   */
  template<typename Base, const size_t Mask = std::numeric_limits<decltype( Base::_address )>::max(),
           const decltype( Base::_address ) Reset = 0, decltype( Base::_address ) Offset = 0>
  class FieldAccess : public RegAccess<decltype( Base::_address ), Base::_address, Mask, Reset, Offset>
  {
  };

}  // namespace Chimera::Gen

#endif /* !CHIMERA_REGISTER_ACCESSOR_HPP */
