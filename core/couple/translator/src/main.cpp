#include <dci/couple/meta/libraryBuilder.hpp>
#include <dci/couple/meta/library.hpp>

#include <dci/couple/parser/config.hpp>
#include <dci/couple/parser/errorInfo.hpp>
#include <dci/couple/parser/exec.hpp>

#include <dci/couple/generator/executor.hpp>

#include <dci/logger/logger.hpp>

#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

#include <boost/filesystem.hpp>

namespace po = boost::program_options;

int main(int argc, const char **argv)
{
    ////////////////////////////////////////////////////////////////////////////////
    po::options_description desc("general options");
    desc.add_options()
            ("help", "produce help message")
            ("version", "print version info")
            ;

    ////////////////////////////////////////////////////////////////////////////////
    po::options_description descInput("input options");
    descInput.add_options()
            (
                "in,i",
                po::value<std::vector<std::string>>(),
                "input file name"
            )
            (
                "include,I",
                po::value<std::vector<std::string>>(),
                "include directories"
            );


    desc.add(descInput);

    ////////////////////////////////////////////////////////////////////////////////
    po::options_description descOutput("output options");
    descOutput.add_options()
            (
                "list-generators",
                "show available generators"
            )
            (
                "generate,g",
                po::value<std::vector<std::string>>(),
                "specify generator for use"
            )
            (
                "outdir,o",
                po::value<std::string>(),
                "output directory"
            );

    desc.add(descOutput);

    ////////////////////////////////////////////////////////////////////////////////
    po::positional_options_description p;
    p.add("in", -1);

    ////////////////////////////////////////////////////////////////////////////////
    po::variables_map vars;
    try
    {
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vars);
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
    if(vars.empty() || vars.count("version"))
    {
        std::cout << "this is a version info" << std::endl;
    }

    if(vars.count("help"))
    {
        std::cout << desc << std::endl;
    }

    dci::couple::meta::Library lib;

    if(vars.count("in"))
    {

        dci::couple::parser::Config cfg;
        if(vars.count("include"))
        {
            cfg._includeDirectories = vars["include"].as<std::vector<std::string>>();
        }

        std::vector<std::string> idlFiles;

        dci::couple::meta::LibraryBuilder lb;

        for(const std::string &in: vars["in"].as<std::vector<std::string>>())
        {
            dci::couple::meta::Library lib;
            switch(lib.load(in))
            {
            case dci::couple::meta::LoadResult::ok:
                lb.merge(lib);
                std::cout << "file loaded: " << in << std::endl;
                break;
            case dci::couple::meta::LoadResult::corruptedFile:
                std::cerr << "corrupted file: " << in << std::endl;
                return EXIT_FAILURE;
            case dci::couple::meta::LoadResult::malformedFile:
                idlFiles.push_back(in);
            }
        }

        {
            std::vector<dci::couple::parser::ErrorInfo> errs;
            if(!dci::couple::parser::exec(idlFiles, cfg, errs, lb))
            {
                std::cerr << "parse failed" << std::endl;
                for(const dci::couple::parser::ErrorInfo &err : errs)
                {
                    std::cerr << err.toString() << std::endl;
                }
                return EXIT_FAILURE;
            }

            for(const std::string &idlFile : idlFiles)
            {
                std::cout << "parsed: " << idlFile << std::endl;
            }
        }

        std::vector<dci::couple::meta::CommitError> errs;
        if(!lb.commitChanges(lib, errs))
        {
            for(const dci::couple::meta::CommitError &err : errs)
            {
                std::cerr << err.toString() << std::endl;
            }
            return EXIT_FAILURE;
        }
    }

    if(vars.count("list-generators"))
    {
        std::cout << "generators available:" << std::endl;

        for(const auto &gen : dci::couple::generator::Executor::getAll())
        {
            std::cout << gen.first;
            if(!gen.second->description().empty())
            {
                std::cout << ": " << gen.second->description();
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    std::string outdir;
    if(vars.count("outdir"))
    {
        outdir = vars["generate"].as<std::string>();
    }
    else
    {
        outdir = boost::filesystem::current_path().string();
    }

    if(vars.count("generate"))
    {
        for(const std::string &gen: vars["generate"].as<std::vector<std::string>>())
        {
            dci::couple::generator::Executor *executor = nullptr;
            try
            {
                executor = dci::couple::generator::Executor::getAll().at(gen);
            }
            catch(const std::out_of_range &e)
            {
                std::cout << "generator " << gen << " is not found" << std::endl;
                continue;
            }

            std::error_code ec = executor->exec(lib, outdir);
            if(ec)
            {
                std::cout << "genarate " << gen << ": " << ec.message() << std::endl;
                break;
            }
            else
            {
                std::cout << gen << " generation done" << std::endl;
            }
        }
    }

    return EXIT_SUCCESS;
}
