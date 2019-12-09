#include "Experiment.h"
#include <iostream>
#include <string>

namespace Cli
{

    template<typename T>
    T input(const std::string &output = "")
    {
        if (!output.empty()) {
            std::cout << output << std::flush;
        }

        T value;
        std::cin >> value;
        return value;
    }

    std::string directionToString(Investigation::Direction direction);
    std::string experimentDataToYamlString(const ExperimentData &data);
    std::string experimentDataToHtmlString(const ExperimentData &data);

}