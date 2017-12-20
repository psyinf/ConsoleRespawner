#include <memory>
#include <iostream>
#include <boost/process.hpp>
#include <boost/program_options.hpp>

namespace bp = boost::process;
namespace po = boost::program_options;

int main(int argc, char** argv)
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message");
		
	po::positional_options_description p;
	p.add("application", -1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	po::notify(vm);
	
	bp::child c(bp::search_path("f:/SimpleDISLogger.exe"), "");
	
	while (c.running())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	} 

	c.wait(); //wait for the process to exit   
	int result = c.exit_code();
	std::cout << "Exited with error code " << result << std::endl;
	return 0;
}