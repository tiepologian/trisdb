/*
 * File:   main.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 17 maggio 2014, 13.02
 */

#include <string>
#include <string.h>
#include <stdio.h>
#include "TrisDb.h"
#include "LogManager.h"
#include "param_t.h"
#include "Shell.h"

void init(param_t params);
void run();
void shell(std::string port, std::string socket);

Config* conf = nullptr;
TrisDb *db = nullptr;

int main(int argc, char *argv[]) {
    param_t params;
    params.setPreamble("TrisDB.\n");
    params.addFlag("--version", false, "version", "Show version number");
    params.addFlag("-v", false, "version-compact", "Show version number");
    params.addFlag("-h", false, "help-compact", "Show help");
    params.addFlag("--shell", false, "shell", "Start command-line shell");
    params.addFlag("-s", false, "shell-compact", "Start command-line shell");
    params.addFlag("--port", "1205", "port", "Shell port");
    params.addFlag("--socket", "nosocket", "socket", "Unix socket");
    params.addFlag("--benchmark", false, "benchmark", "Start benchmark utility");
    params.addFlag("-b", false, "benchmark-compact", "Start benchmark utility");
    params.addFlag("--config", "noconfig", "config", "Specify configuration file");
    params.addFlag("-c", "noconfig", "config-compact", "Specify configuration file");

    try {
        params.parseCommandLine(argc, argv);
        init(params);
        run();
    } catch (std::exception& e) {
        LogManager::getSingleton()->log(LogManager::LERROR, e.what());
        return 1;
    }

    delete conf;
    LogManager::getSingleton()->log(LogManager::LINFO, "Exiting, bye bye");
    return 0;
}

void init(param_t params) {
    if (params.getBoolFlag("--version") || params.getBoolFlag("-v")) {
        std::cout << TRISDB_VERSION_STR << std::endl;
        exit(0);
    } else if (params.getBoolFlag("-h")) {
        params.printHelp();
        exit(0);
    } else if (params.getBoolFlag("--shell") || params.getBoolFlag("-s")) {
        shell(params.getStringFlag("--port"), params.getStringFlag("--socket"));
    } else if (params.getBoolFlag("--benchmark") || params.getBoolFlag("-b")) {
        conf = new Config();
        conf->loadDefaults();
        db = new TrisDb(conf);
        db->benchmark();
        delete db;
        delete conf;
        exit(0);
    }
    // If no version, help or shell, read settings from file or use defaults and save in config var
    if (strcmp(params.getStringFlag("--config").c_str(), "noconfig") != 0) {
        // use specified config file
        conf = Config::loadFromFile(params.getStringFlag("--config"));
    } else if (strcmp(params.getStringFlag("-c").c_str(), "noconfig") != 0) {
        conf = Config::loadFromFile(params.getStringFlag("-c"));
    } else {
        // no config file, use defaults
        conf = new Config();
        conf->loadDefaults();
    }
}

void run() {
    Utils::printAsciiLogo();
    LogManager::getSingleton()->log(LogManager::LINFO, TRISDB_VERSION_STR);
    LogManager::getSingleton()->log(LogManager::LINFO, "Using config " + conf->getName());
    LogManager::getSingleton()->log(LogManager::LINFO, "Logging to " + conf->getSetting("logfile"));
    db = new TrisDb(conf);
    db->run();
    LogManager::getSingleton()->log(LogManager::LINFO, "Shutting down database");
    delete db;
}

void shell(std::string port, std::string socket) {
    LogManager::getSingleton()->log(LogManager::LINFO, TRISDB_VERSION_STR);
    Shell* shell = new Shell(port, socket);
    shell->run();
    delete shell;
    exit(0);
}