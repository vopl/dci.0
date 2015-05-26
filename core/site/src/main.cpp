#include <cstdlib>

#include <dci/site/manager.hpp>
#include <dci/site/testHub.hpp>

#include <dci/async.hpp>
#include <dci/logger/logger.hpp>

#include <csignal>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

dci::site::Manager *manager = nullptr;

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
            (
                "test-stage",
                po::value<std::string>(),
                "stage for testing, one of noenv, min, mload, mstart"
            )
            ;

    ////////////////////////////////////////////////////////////////////////////////
    po::variables_map vars;
    try
    {
        po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vars);
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
    dci::site::testHub::Stage testStage = dci::site::testHub::Stage::null;
    if(vars.count("test-stage"))
    {
        {
            auto s  = vars["test-stage"].as<std::string>();

                 if("noenv" == s)   testStage = dci::site::testHub::Stage::noenv;
            else if("min" == s)     testStage = dci::site::testHub::Stage::min;
            else if("mload" == s)   testStage = dci::site::testHub::Stage::mload;
            else if("mstart" == s)  testStage = dci::site::testHub::Stage::mstart;
            else
            {
                LOGF("unrecognized test stage: "<<s);
                return EXIT_FAILURE;
            }
        }

        if(dci::site::testHub::Stage::noenv == testStage)
        {
            return dci::site::Manager::executeTest(argc, argv, testStage);
        }
    }

    if(vars.count("version"))
    {
        std::cout << "this is a version info" << std::endl;
        return EXIT_SUCCESS;
    }

    if(vars.count("help"))
    {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }

    if(vars.count("genmanifest"))
    {
        std::string content = dci::site::Manager::generateManifest(vars["genmanifest"].as<std::string>());

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
        manager = new dci::site::Manager;

        signal(SIGINT,  signalHandler);
        signal(SIGTERM, signalHandler);

        std::error_code ec = manager->run(argc, argv, testStage);
        if(ec && ec != dci::site::err_general::test_failed)
        {
            LOGE("run: "<<ec);
        }

        signal(SIGINT,  SIG_DFL);
        signal(SIGTERM, SIG_DFL);

        delete manager;
        manager = nullptr;

        if(ec)
        {
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
