#include <fstream>

#include <q3/database.hpp>
#include <q3/query.hpp>
#include <q3/record.hpp>

auto main() -> int {
	using namespace q3;

	// The following code should compile succesfully when uncommented if you have a good
	// implementation

	auto studentsdb = database();
	std::ifstream studentsfile("source/q3/students.txt");
	studentsfile >> studentsdb;

	// write out the Smiths
	std::ofstream smithfile("smith.db");
	smithfile << studentsdb.select(query_equals("last-name", "Smith"));

	// write everyone but the Smiths
	std::ofstream notsmithfile("notsmith.db");
	notsmithfile << studentsdb.select(query_not(query_equals("last-name", "Smith")));

	// write out the Smiths and Jonesess
	std::ofstream smithandjonesfile("smith_and_jones.db");
	smithandjonesfile << studentsdb.select(
	   query_or(query_equals("last-name", "Smith"), query_equals("last-name", "Jones")));

	return 0;
}
