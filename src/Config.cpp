/* 
 * File:   Config.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 * 
 * Created on 18 maggio 2014, 12.25
 */

#include "Config.h"

Config::Config() {
}

Config::~Config() {
}

Config* Config::loadFromFile(std::string file) {
    // read from file, parse, save settings in Config object and return it
    Config* settings = new Config();
    settings->name = file;
    // first load defaults
    settings->loadDefaults();
    // then overwrite with settings from file
    return settings;
}

void Config::loadDefaults() {
    // set default settings
    settings["daemonize"] = "yes";
    settings["pidfile"] = "/var/run/trisdb/trisdb.pid";
    settings["listen"] = "localhost";
    settings["port"] = "1205";
    settings["logfile"] = "/var/log/trisdb/trisdb.log";
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