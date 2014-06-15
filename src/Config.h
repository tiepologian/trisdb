/* 
 * File:   Config.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 18 maggio 2014, 12.25
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include "ConfigReader.h"
#include "Utils.h"
#include <map>

class Config {
public:
    Config();
    virtual ~Config();
    static Config* loadFromFile(std::string file);
    void loadDefaults();
    std::string getSetting(std::string name);
    void setSetting(std::string name, std::string value);
    std::string getName();
private:
    std::map<std::string, std::string> settings;
    std::string name;
};

#endif	/* CONFIG_H */