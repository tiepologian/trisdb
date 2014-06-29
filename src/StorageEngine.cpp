/* 
 * File:   StorageEngine.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 29 giugno 2014, 13.15
 */

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
    // create directory and write to file
    boost::system::error_code error;
    boost::filesystem::create_directories(dbpath + "/data", error);    
    inst_->saveDataToFile(data, dbpath+"/data/data_0.tdb", 0);
    inst_->saveDataToFile(data, dbpath+"/data/data_1.tdb", 1);
    inst_->saveDataToFile(data, dbpath+"/data/data_2.tdb", 2);
    saveDuration = TimeUtils::getCurrentTimestamp() - saveDuration;
    LogManager::getSingleton()->log(LogManager::LINFO, "Database saved to disk in " + std::to_string(saveDuration) + "ms");
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