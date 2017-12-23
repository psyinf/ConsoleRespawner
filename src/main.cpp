#include <memory>
#include <iostream>
#include <boost/process.hpp>
#include <boost/program_options.hpp>

namespace bp = boost::process;
namespace po = boost::program_options;

po::variables_map getCommandLine(int argc, char** argv)
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
	return vm;
}

void test()
{
	try
	{
		boost::asio::io_service ios;
		std::vector<char> buf(64);
		bp::async_pipe ap(ios);

		bp::child c(bp::search_path("ping"), "127.0.0.1", bp::std_out > ap);

		auto reader = [&buf](const boost::system::error_code &ec, std::size_t size) { std::copy_n(std::begin(buf), size, std::ostream_iterator<char>(std::cerr, ""));	};

		boost::asio::async_read(ap, boost::asio::buffer(buf), reader);
		ios.run();
		c.wait();
		ios.stop();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in execution: " << e.what();
	}
	
}
void test_sync()
{
	try
	{
		bp::ipstream is;

		bp::child c(bp::search_path("ping"), "127.0.0.1", bp::std_out > is);

		std::string line;
		while (c.running() && std::getline(is, line) && !line.empty())
		{
			std::cerr << line << std::endl;
		}
		c.wait();

	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in execution: " << e.what();
	}
}



int main(int argc, char** argv)
{

	test_sync();

	try
	{
		auto vm = getCommandLine(argc, argv);
		//get parameters
		std::string application = vm["application"].as<std::string>();
		std::cout << "Starting " << application << std::endl;


		//ASYNC
		boost::asio::io_service ios;
		std::vector<char> buf(1024);
		bp::async_pipe ap(ios);
		
		bp::child c(application
			, "-epool eth-eu1.nanopool.org:9999 -ewal 2222.psyinf1 -epsw x -mode 1 -ftime 10 -wd 0 -ethi 7 -r 60 -li 1"
			, bp::std_in.close(), bp::std_err.close(), bp::std_out > ap);

		/*bp::child c(bp::search_path("ping"), "127.0.0.1", bp::std_in.close(),
			bp::std_err.close(), bp::std_out > ap);*/
		boost::asio::async_read(ap, boost::asio::buffer(buf),[&buf](const boost::system::error_code &ec, std::size_t size) 
		{
			std::copy_n(std::begin(buf), size, std::ostream_iterator<char>(std::cerr, ""));
		});

		//start service
		ios.run();
		//wait for the process to exit   
		c.wait(); 
		//report
		int result = c.exit_code();
		std::cout << "Exited with error code " << result << std::endl;
	}
	catch (const std::exception& e)
	{
		
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}