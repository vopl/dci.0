#include <cstdlib>

#include "impl/manager.hpp"
#include "impl/module.hpp"
#include <dci/async/functions.hpp>
#include <dci/logger/logger.hpp>
#include <dci/couple/runtime/sign.hpp>

#include <csignal>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

dci::site::impl::Manager *manager = nullptr;

void signalHandler(int signum)
{
    switch(signum)
    {
    case SIGINT:
    case SIGTERM:
        if(manager)
        {
            dci::async::spawn([](){
                auto f = manager->stop();
                if(f.hasError())
                {
                    LOGE("stop: "<<f.error());
                }
            });
        }
        break;

    default:
        exit(EXIT_FAILURE);
    }
}

bool printoutput(const po::variables_map &vars, const std::string &content);

int main(int argc, char *argv[])
{
    //set current path as bin/..
    {
        boost::system::error_code ec;
        fs:: path p{argv[0]};
        p = fs::system_complete(p, ec);
        if(ec)
        {
            LOGE("unable to determine current directory: "<<ec);
            return EXIT_FAILURE;
        }

        p = p.normalize().parent_path().parent_path();
        fs::current_path(p, ec);
        if(ec)
        {
            LOGE("unable to set current directory to "<<p<<": "<<ec);
            return EXIT_FAILURE;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    po::options_description desc("dci site");
    desc.add_options()
            ("help", "produce help message")
            ("version", "print version info")
            (
                "genmanifest",
                po::value<std::string>(),
                "generate manifest file for module shared library"
            )
            (
                "rndsign",
                "generate new random sign"
            )
            (
                "outfile",
                po::value<std::string>(),
                "output file name for genmanifest, rndsign"
            )
            ;

    ////////////////////////////////////////////////////////////////////////////////
    po::variables_map vars;
    try
    {
        po::store(po::command_line_parser(argc, argv).options(desc).run(), vars);
    }
    catch(std::exception &e)
    {
        LOGE("exception: "<<e.what());
        return EXIT_FAILURE;
    }
    catch(...)
    {
        LOGE("unknown exception");
        return EXIT_FAILURE;
    }
    po::notify(vars);

    ////////////////////////////////////////////////////////////////////////////////
    if(vars.count("version"))
    {
        std::cout << "this is a version info" << std::endl;
    }

    if(vars.count("help"))
    {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }

    if(vars.count("genmanifest"))
    {
        std::string content = dci::site::impl::Module::generateManifest(vars["genmanifest"].as<std::string>());

        if(content.empty())
        {
            return EXIT_FAILURE;
        }

        return printoutput(vars, content) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    if(vars.count("rndsign"))
    {
        dci::couple::runtime::Sign s;
        s.fromRnd();
        return printoutput(vars, s.toHex()) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    ////////////////////////////////////////////////////////////////////////////////
    {
        manager = new dci::site::impl::Manager{};

        signal(SIGINT,  signalHandler);
        signal(SIGTERM, signalHandler);

        std::error_code ec = manager->run();
        if(ec)
        {
            LOGE("run: "<<ec);
        }

        signal(SIGINT,  SIG_DFL);
        signal(SIGTERM, SIG_DFL);

        delete manager;
        manager = nullptr;

        if(ec)
        {
            LOGE(ec);
            return ec.value();
        }
    }

    return EXIT_SUCCESS;
}

bool printoutput(const po::variables_map &vars, const std::string &content)
{
    if(vars.count("outfile"))
    {
        std::ofstream out(vars["outfile"].as<std::string>());
        if(!out)
        {
            std::cerr<<strerror(errno)<<std::endl;
            return false;
        }

        out.write(content.data(), content.size());
        return true;
    }

    std::cout.write(content.data(), content.size());
    return true;
}
