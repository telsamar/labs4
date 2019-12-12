#pragma once

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <forward_list>

struct BrokerFile //opisanie faila
{
    using DirsType = std::list<std::string>;

    std::string filename;
    DirsType directories;

    [[nodiscard]] std::string directoriesToString() const;

    bool operator==(const BrokerFile &pair) const
    {
        return pair.filename == filename && pair.directories == directories;
    }
};

struct BrokerData
{
    struct Pair // dannie brokera i nomer akaunta
    {
        std::string broker;
        size_t account;

        bool operator==(const Pair &pair) const
        {
            return pair.account == account && pair.broker == broker;
        }

        struct Hash
        {
            size_t operator()(const Pair &pair) const
            {
                return std::hash<size_t>{}(pair.account) ^ std::hash<std::string>{}(pair.broker);
            }
        };
    };

    struct Data //kol-vo failov i datu poslednei modifikacii
    {
        size_t files = 0;
        std::string lastDate;
    };
};

class BrokerResolver //kollekciya dannih, kollekciyu failov, shabloni reg-h virazhenii dlya imeni failov
{
public:
    using Collection = std::forward_list<BrokerFile>;
    using DataCollection = std::unordered_map<BrokerData::Pair, BrokerData::Data, BrokerData::Pair::Hash>;
    static boost::regex filenameChecker;

    template<typename T>
    static void setIfMore(T &oldValue, const T &newValue)
    {
        if (oldValue < newValue) {
            oldValue = newValue;
        }
    }

    // Запомнить информацию о файле, если он удовлетворяет условию
    void saveFileIfCorrect(const boost::filesystem::path &path);

    // Запомнить все файлы, удовлетворяющие условиям
    void resolve(const boost::filesystem::path &path);

    // Получить результат
    [[nodiscard]] const Collection &getFileCollection() const
    {
        return fileCollection;
    }

    // Получить результат по данным
    [[nodiscard]] const DataCollection &getDataCollection() const
    {
        return dataCollection;
    }
private:
    BrokerFile::DirsType currentDirectories;            // Список директорий, относительно текущей
    Collection fileCollection;                          // Результат
    DataCollection dataCollection;
};