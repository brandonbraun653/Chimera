/********************************************************************************
 *  File Name:
 *    common_fixture.hpp
 *
 *  Description:
 *    Defines common resources uses in the Chimera integration test fixtures
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef CHIMERA_COMMON_INTEGRATION_TEST_FIXTURE_HPP
#define CHIMERA_COMMON_INTEGRATION_TEST_FIXTURE_HPP

namespace Chimera
{
  namespace IntegrationTests
  {
    /**
     *	A stub function that allows the Chimera backend to provide
     *  custom setup functionality that will be run with every test
     *  fixture.
     *
     *	@return void
     */
    extern void CommonSetUp();

    /**
     *	A stub function that allows the Chimera backend to provide
     *  custom tear down functionality that will be run with every test
     *  fixture.
     *
     *	@return void
     */
    extern void CommonTearDown();

  }  // namespace IntegrationTests
}  // namespace Chimera


#endif /* !CHIMERA_COMMON_INTEGRATION_TEST_FIXTURE_HPP */