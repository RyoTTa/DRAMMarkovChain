#include "./../ext/headers/args.hxx"

#include "common.h"
#include "configuration.h"
#include "dram.h"

using namespace std;
using namespace naivemarkov;

int main(int argc, const char **argv){
    args::ArgumentParser parser(
            "DRAM Fault Simulator.",
            "Examples: \n."
            "./build/naivemarkovmain -r 1000\n"
            "./build/naivemarkovmain configs/test.ini -r 1000");
    args::HelpFlag help(parser, "help", "Display the help menu", {'h', "help"});
    args::ValueFlag<uint64_t> num_repeat_arg(
            parser, "num_repeat",
            "Number of repeat round",
            {'r', "num-repeat"}, 1000);
    args::ValueFlag<std::string> output_dir_arg(
            parser, "output_dir", "Output directory for stats files",
            {'o', "output-dir"}, ".");
    args::ValueFlag<std::string> output_prefix_arg(
            parser, "output_prefix", "Output prefix for stats and fault trace file",
            {'p', "output-prefix"}, "current");
    args::ValueFlag<std::string> config_arg(
            parser, "config", "The config file name (mandatory)",
            {'c', "config"});

    //Check arguments
    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    //Read simple argument
    int repeat_round = args::get(num_repeat_arg);
    std::string output_dir = args::get(output_dir_arg);
    std::string output_prefix = args::get(output_prefix_arg);

    //Read configuration file
    std::string config_file = args::get(config_arg);
    if (config_file.empty()) {
        std::cerr << parser;
        return 1;
    }
    Config *config = new Config(config_file, output_dir, output_prefix);

	DRAM *mem = new DRAM(*config);
    std::cout << "repeat round : " << repeat_round << std::endl;

	for(int i=0; i < repeat_round; i++){
        if ((mem->SIZE_B + mem->SIZE_G + mem->SIZE_N + mem->SIZE_W) != mem->SIZE) {
            std::cerr << "Size unFit" << std::endl;
            AbruptExit(__FILE__, __LINE__);
        }
        mem->Fault_injection();
        mem->State_change();
		if((i % (repeat_round/100)) == 0)
            std::cout << "#";

	}
    std::cout << std::endl;
    mem->PrintFaultmap();
    
}