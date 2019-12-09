#include <vector>
#include <memory>
#include <random>
#include <functional>
#include <chrono>
#include <set>

struct Experiment;

struct ExperimentInitData
{
    using BufferType = std::set<size_t>;

    static ExperimentInitData getExperimentData(const struct HardwareData &hardware);

    BufferType bufferSizes;
};


struct Investigation
{
    enum Direction
    {
        Forward,
        Backward,
        Random,
    };

    Direction direction;

    std::vector<Experiment> experiments;

    static Investigation doInvestigation(Direction direction, const ExperimentInitData::BufferType& bufferSizes);
};

struct Experiment
{
    using AtomicType = uint64_t;
    using BufferPtr = std::shared_ptr<AtomicType[]>;
    using WalkFunctionType = std::function<AtomicType(BufferPtr, size_t)>;

    size_t bufferSize;
    size_t duration;

    static Experiment doExperiment(size_t bufferSize, Investigation::Direction direction);

    static void runForward(const BufferPtr &buffer, std::vector<size_t> &indexes);
    static void runBackward(const BufferPtr &buffer, std::vector<size_t> &indexes);
    static void runRandom(const BufferPtr &buffer, std::vector<size_t> &indexes);

    static void run(const BufferPtr &buffer, const std::vector<size_t> &indexes);

    static size_t kibToSize(size_t kib);
    static size_t sizeToKib(size_t size);

    static const size_t iterationAmount;
private:
    static BufferPtr createFilledBuffer(size_t size);
};

struct ExperimentData
{
    std::vector<Investigation> investigations;
};