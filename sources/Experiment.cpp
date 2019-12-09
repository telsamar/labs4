// Copyright 2019 <telsamar>
#include "HardwareData.h"
#include "Cli.h"
#include <cmath>
#include <chrono>
#include <iostream>
#include <random>

using BufferType = ExperimentInitData::BufferType;

Experiment Experiment::doExperiment(size_t bufferSize,
                   Investigation::Direction direction)
{
    std::cout << "Experiment. Size (KiB): " << bufferSize
             << ". Size (array size): " << kibToSize(bufferSize) << std::endl;
    bufferSize = kibToSize(bufferSize);

    // Инициализация
    std::chrono::system_clock::time_point startTime, stopTime;
    auto buffer = createFilledBuffer(bufferSize);

    std::vector<size_t> indexes(bufferSize);
    // заполняем массив индексов
    std::iota(indexes.begin(), indexes.end(), 0);

    static const std::unordered_map<
            Investigation::Direction,
            void (*)(const BufferPtr &, std::vector<size_t> &)
    > runners = {
            {Investigation::Forward, &Experiment::runForward},
            {Investigation::Backward, &Experiment::runBackward},
            {Investigation::Random, &Experiment::runRandom},
    };

    // Выполнение
    startTime = std::chrono::high_resolution_clock::now();
    for (size_t k = 0; k < iterationAmount; k++) {
        runners.at(direction)(buffer, indexes);
    }
    stopTime = std::chrono::high_resolution_clock::now();

    // Возврат результата
    return {
            sizeToKib(bufferSize),
            static_cast<size_t>(
            std::chrono::duration_cast<std::chrono::
            milliseconds>(stopTime - startTime).count()),
    };
}

void Experiment::runForward(const Experiment::BufferPtr &buffer,
                           std::vector<size_t> &indexes)
{
    return run(buffer, indexes);
}

void Experiment::runBackward(const Experiment::BufferPtr &buffer,
                            std::vector<size_t> &indexes)
{
    std::reverse(indexes.begin(), indexes.end());

    return run(buffer, indexes);
}

void Experiment::runRandom(const Experiment::BufferPtr &buffer,
                          std::vector<size_t> &indexes)
{
    std::shuffle(indexes.begin(), indexes.end(),
    std::mt19937{std::random_device{}()});

    return run(buffer, indexes);
}

void Experiment::run(const BufferPtr &buffer,
          const std::vector<size_t> &indexes)
{
    size_t value = 0;

    for (size_t index : indexes) {
        value = buffer[index];
    }
    buffer[0] = value;          // Чтобы компилятор не ругался
}

const size_t Experiment::iterationAmount = 1000;

Experiment::BufferPtr Experiment::createFilledBuffer(size_t size)
{
    auto buffer = BufferPtr(new size_t[size]);
    auto random = std::mt19937{std::random_device{}()};

    for (size_t i = 0; i < size; i++) {
        buffer[i] = random();
    }

    return buffer;
}

size_t Experiment::kibToSize(size_t kib)
{
    return kib * 1024 / sizeof(size_t);
}

size_t Experiment::sizeToKib(size_t size)
{
    return size * sizeof(size_t) / 1024;
}

Investigation Investigation::doInvestigation(Direction direction,
                                   const BufferType &bufferSizes)
{
    std::cout << "Investigation. Direction: " <<
    Cli::directionToString(direction) << std::endl;

    std::vector<Experiment> experiments;

    for (size_t size : bufferSizes) {
        experiments.emplace_back(
                Experiment::doExperiment(size, direction));
    }

    return {
            direction,
            std::move(experiments),
    };
}
















ExperimentInitData ExperimentInitData::getExperimentData
                          (const HardwareData &hardware)
{
    size_t startValue = hardware.cacheMemoryKB.front() / 2;
    size_t stopValue = hardware.cacheMemoryKB.back() * 3 / 2;

    size_t startPower = ceil(log2(startValue));
    size_t stopPower = floor(log2(stopValue));

    BufferType resultBufferSizes;

    resultBufferSizes.insert(startValue);
    for (size_t power = startPower; power <= stopPower; power++) {
        resultBufferSizes.insert(1u << power);
    }
    resultBufferSizes.insert(stopValue);

    return {
            std::move(resultBufferSizes),
    };
}
