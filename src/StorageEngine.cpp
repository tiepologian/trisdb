/* 
 * File:   StorageEngine.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 29 giugno 2014, 13.15
 */

#include <thread>

#include "StorageEngine.h"

StorageEngine* StorageEngine::inst_ = nullptr;

StorageEngine* StorageEngine::getSingleton() {
    if (inst_ == nullptr) {
        inst_ = new StorageEngine();
    }
    return inst_;
}

void StorageEngine::syncSave(TripleMap<std::string, std::string, std::string>* data, std::string dbpath) {
    LogManager::getSingleton()->log(LogManager::LINFO, "Saving database to disk...");
    auto saveDuration = TimeUtils::getCurrentTimestamp();
    boost::system::error_code error;
    boost::filesystem::create_directories(dbpath + "/data", error);
    std::ofstream manifestFile;
    manifestFile.open(dbpath + "/Manifest");
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; i++) {
        threads.push_back(std::thread([&, i] {
            inst_->saveDataToFile(data, dbpath + "/data/data_" + std::to_string(i) + ".tdb", i);
        }));
        manifestFile << "data/data_" + std::to_string(i) + ".tdb\n";
    }
    manifestFile.close();
    // wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }
    saveDuration = TimeUtils::getCurrentTimestamp() - saveDuration;
    LogManager::getSingleton()->log(LogManager::LINFO, "Database saved to disk in " + std::to_string((int) saveDuration) + "ms");
}

void StorageEngine::saveDataToFile(TripleMap<std::string, std::string, std::string>* data, std::string filename, int index) {
    // serialize TripleMap
    std::ostringstream os;
    cereal::BinaryOutputArchive oarchive(os);
    if (index == 0) oarchive(*data->getDataA());
    else if (index == 1) oarchive(*data->getDataB());
    else if (index == 2) oarchive(*data->getDataC());
    // compress using Snappy
    std::string serialData = os.str();
    std::string compressedData;
    snappy::Compress(serialData.data(), serialData.size(), &compressedData);
    std::ofstream out(filename, std::ios::binary);
    out << compressedData;
    out.close();
}

bool StorageEngine::checkManifest(std::string dbpath) {
    std::ifstream manifestFile(dbpath + "/Manifest");
    if (manifestFile.is_open()) {
        manifestFile.close();
        return true;
    } else {
        // manifest not found
        return false;
    }
}

void StorageEngine::syncLoad(TripleMap<std::string, std::string, std::string>* data, std::string dbpath) {
    LogManager::getSingleton()->log(LogManager::LINFO, "Loading database from disk...");
    auto loadDuration = TimeUtils::getCurrentTimestamp();
    std::string dbFiles[3];
    std::string line;
    int lineCount = 0;
    // read tdb filenames from manifest
    std::ifstream manifestFile(dbpath + "/Manifest");
    if (manifestFile.is_open()) {
        while (getline(manifestFile, line)) {
            dbFiles[lineCount] = line;
            lineCount++;
        }
        manifestFile.close();
    }
    // now de-serialize data
    //for (int i = 0; i < 3; i++) {
    //    inst_->loadDataFromFile(data, dbpath + "/" + dbFiles[i], i);
    //}
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; i++) {
        threads.push_back(std::thread([&, i] {
            inst_->loadDataFromFile(data, dbpath + "/" + dbFiles[i], i);
        }));
    }
    // wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }
    loadDuration = TimeUtils::getCurrentTimestamp() - loadDuration;
    LogManager::getSingleton()->log(LogManager::LINFO, "Database loaded in " + std::to_string((int) loadDuration) + "ms");
}

void StorageEngine::loadDataFromFile(TripleMap<std::string, std::string, std::string>* data, std::string filename, int index) {
    // de-serialize TripleMap
    std::ifstream is(filename, std::ios::binary);
    std::stringstream buffer;
    buffer << is.rdbuf();
    is.close();
    std::string compressedData = buffer.str();
    std::string uncompressedData;
    snappy::Uncompress(compressedData.data(), compressedData.size(), &uncompressedData);
    std::stringstream iss;
    iss.str(uncompressedData);
    cereal::BinaryInputArchive iarchive(iss);
    if (index == 0) iarchive(*data->getDataA());
    else if (index == 1) iarchive(*data->getDataB());
    else if (index == 2) iarchive(*data->getDataC());
}