#include <memory>
#include <iostream>
#include <boost/process.hpp>
#include <boost/program_options.hpp>

namespace bp = boost::process;
namespace po = boost::program_options;

int main(int argc, char** argv)
{
	try
	{
		std::string program_name;
		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("application", po::value(&program_name)->required(), "Program to start and monitor");

		po::positional_options_description p;
		p.add("application", -1);

		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
		po::notify(vm);
		bp::ipstream pipe_stream;
		//bp::child c(program_name, bp::std_out > pipe_stream);

		bp::child c("cmd" , bp::std_out > pipe_stream);
		while (c.running())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::string line;
			while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
			{
				std::cerr << line << std::endl;
			}
				
		}

		c.wait(); //wait for the process to exit   
		int result = c.exit_code();
		std::cout << "Exited with error code " << result << std::endl;
	}
	catch (const std::exception& e)
	{
		
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}