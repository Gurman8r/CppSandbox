#include <MemeCore/TestRequest.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	TestRequest::TestRequest()
	{
	}

	TestRequest::~TestRequest()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void TestRequest::process()
	{
		Debug::log("Processing");
	}

	void TestRequest::finish()
	{
		Debug::log("Finishing");
	}

	/* * * * * * * * * * * * * * * * * * * * */
}