// File Header Here

#include "openjaus/transport/ojTransport/jshm/Interface.h"
#include <cxxtest/TestSuite.h>
#include <stdio.h>
// Start of user code header files:
// End of user code

class TestInterface : public CxxTest::TestSuite
{
public:
	// Start of user code TestInterface:
	openjaus::transport::ojTransport::jshm::Interface *interface;
	// End of user code
	
	void setUp()
	{
		// Start of user code test setup:
		interface = new openjaus::transport::ojTransport::jshm::Interface();	
	
		// End of user code
	}

	void tearDown()
	{
		// Start of user code test tear down:
		
		delete interface;
		// End of user code
	}

	void testConstruction()
	{
		// Start of user code testConstruction:
		TS_ASSERT(interface);
		// End of user code
	}
	
	// Start of user code custom tests:
	// End of user code
};
