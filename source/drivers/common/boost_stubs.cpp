/********************************************************************************
 *  File Name:
 *    boost_stubs.cpp
 *
 *  Description:
 *    Stubs required to compile with Boost
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <exception>


namespace boost
{
  void throw_exception( std::exception const &e )
  {
    //Do nothing 
  }
}