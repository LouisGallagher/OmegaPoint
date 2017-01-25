#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>

#include <boost/program_options.hpp>

#ifndef OPTIONS_H_
#define OPTIONS_H_

namespace po = boost::program_options;

class Options
{
	public:
		static const Options& get(int argc = 0, char ** argv = 0)
		{
			static const Options instance(argc, argv);
			return instance;
		}

		int fps;
		int width;
		int height;
		int ttl;
		bool stream;
		bool log;
		std::string channel;
		std::string name;

	private:
		Options(int argc, char ** argv)
		: width(640),
		  height(480),
		  fps(30),
		  ttl(0),
		  stream(false),
		  log(false),
		  channel("Logger2")
		{
			po::options_description desc("Logger2, a tool for logging data captured on an ASUS Xtion Pro Live and Intel RealSense R200.\n Options:");
			
			desc.add_options()
			("help", "Print this help message")
			("width,w", po::value<int>(), "Frame width. [Default = 640]")
			("height,h", po::value<int>(), "Frame height [Default = 480]")
			("fps,f", po::value<int>(), "Frames per second. [Default = 30]")
			("ttl,t", po::value<int>(), "Set the UDP ttl. Set to 1 to stream packets on local network. A value of 0 means packets wont be transmitted on the wire. [Default = 0]")
			("stream,s", " Stream the frames using LCM framework (Lightweight Communications and Marshalling). [Default = false]")
			("log,l", "Log the frames to a local file. [Default = false]")
			("channel", po::value<std::string>(), "The channel to which frames should be published when streaming is enabled. [Default = Logger2]");
	
			po::variables_map vm;
			po::store(po::parse_command_line(argc, argv, desc), vm);
			po::notify(vm);

			if(vm.count("help"))
			{
				std::cout << desc << std::endl;
				std::exit(EXIT_SUCCESS);
			}

			if(vm.count("width"))
				width = vm["width"].as<int>();
			
			if(vm.count("height"))
				height = vm["height"].as<int>();

			if(vm.count("fps"))
				fps = vm["fps"].as<int>();

			if(vm.count("channel"))
				channel = vm["channel"].as<std::string>();

			if(vm.count("stream"))
				stream = true;

			if(vm.count("log"))
				log = true;

			if(vm.count("ttl"))
				ttl = vm["ttl"].as<int>();

			char hostname[256];
			char username[256];
			
			gethostname(hostname, 256);
			getlogin_r(username, 256);
	
			name = std::string(hostname) + "/" + std::string(username) + "/" + std::to_string(getpid());	

		}

	public:
		Options(Options const & ) = delete;
		void operator=(Options const & ) = delete;
};

#endif //OPTIONS_H_