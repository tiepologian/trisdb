/* 
 * File:   Config.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 18 maggio 2014, 12.25
 */

#include "Config.h"
#include "LogManager.h"

Config::Config() {
}

Config::~Config() {
}

Config* Config::loadFromFile(std::string file) {
    // read from file, parse, save settings in Config object and return it
    Config* settings = new Config();    
    settings->loadDefaults();
    settings->name = file;
    // parse settings from file and save them in the config object
    Reader reader;
    try {
        reader.setCaseSensitivity(false);
        reader.readSettingsFile(file);
        // if the file doesn't contain a key, default value is used
        if(reader.containsKey("listen")) settings->setSetting("listen", reader.getValueByKey<std::string>("listen"));
        if(reader.containsKey("port")) settings->setSetting("port", reader.getValueByKey<std::string>("port"));
        if(reader.containsKey("unixsocket")) settings->setSetting("unixsocket", reader.getValueByKey<std::string>("unixsocket"));
        if(reader.containsKey("loglevel")) settings->setSetting("loglevel", reader.getValueByKey<std::string>("loglevel"));
        if(reader.containsKey("logfile")) settings->setSetting("logfile", reader.getValueByKey<std::string>("logfile"));
        if(reader.containsKey("dbfolder")) settings->setSetting("dbfolder", reader.getValueByKey<std::string>("dbfolder"));
    } catch(ReaderError e) {
        // error in config file, use defaults
        LogManager::getSingleton()->log(LogManager::LERROR, "Error parsing config file, using default settings");
        settings->loadDefaults();
        return settings;
    }
    return settings;
}

void Config::loadDefaults() {
    // set default settings
    settings["listen"] = "localhost";
    settings["port"] = "1205";
    settings["unixsocket"] = "no";
    settings["loglevel"] = "info";
    settings["logfile"] = "/var/log/trisdb/trisdb.log";
    settings["dbfolder"] = "/var/lib/trisdb";
    name = "default";
}

std::string Config::getSetting(std::string name) {
    return settings[name];
}

void Config::setSetting(std::string name, std::string value) {
    settings[name] = value;
}

std::string Config::getName() {
    return name;
}