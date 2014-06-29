/* 
 * File:   StorageEngine.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 29 giugno 2014, 13.15
 */

#ifndef STORAGEENGINE_H
#define	STORAGEENGINE_H

#include <boost/filesystem/operations.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/archives/binary.hpp>
#include <snappy.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include "TripleMap.h"
#include "LogManager.h"

class StorageEngine {
public:    
    static StorageEngine* getSingleton();
    static void syncSave(TripleMap<std::string, std::string, std::string> *data, std::string dbpath);
private:
    static StorageEngine* inst_;    
    StorageEngine() {};
    StorageEngine(const StorageEngine&) {};
    StorageEngine& operator=(const StorageEngine&);  
    void saveDataToFile(TripleMap<std::string, std::string, std::string> *data, std::string filename, int index);
};

#endif	/* STORAGEENGINE_H */