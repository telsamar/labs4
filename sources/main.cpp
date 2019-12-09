// Copyright 2019 <telsamar>
#include <iostream>

#include "HardwareData.h"
#include "Cli.h"

int main()
{
    auto levels = Cli::input<size_t>("Enter amount of cache levels: ");

    std::vector<size_t> cacheMemory;
    for (size_t i = 0; i < levels; i++) {
        cacheMemory.push_back(
                Cli::input<size_t>("Enter L" + std::to_string(i + 1)
                              + " size (KiB): "));
    }

    ExperimentInitData initData = ExperimentInitData::getExperimentData({
         cacheMemory,
                                                                        });

    ExperimentData result{
            {
                    Investigation::doInvestigation(
                            Investigation::Forward,
                            initData.bufferSizes),
                    Investigation::doInvestigation(
                            Investigation::Backward,
                            initData.bufferSizes),
                    Investigation::doInvestigation(
                            Investigation::Random,
                            initData.bufferSizes),
            },
    };

    std::cout << "\n\n\nYAML Report:\n" <<
         Cli::experimentDataToYamlString(result) << std::endl;

    std::cout << "\n\n\nHTML Report:\n" <<
         Cli::experimentDataToHtmlString(result) << std::endl;

    return 0;
}
