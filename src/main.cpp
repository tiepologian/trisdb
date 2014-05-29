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
#include "TcpServer.h"
#include "Shell.h"

void init(param_t params);
void run();
void test();
void shell();

Config* conf = nullptr;
TrisDb *db = nullptr;

int main(int argc, char *argv[]) {
    param_t params;
    params.setPreamble("TrisDB.\nThis is a help message.\n");
    params.addFlag("--version", false, "version", "Show version number");
    params.addFlag("-v", false, "version-compact", "Show version number");
    params.addFlag("-h", false, "help-compact", "Show help");
    params.addFlag("--shell", false, "shell", "Start command-line shell");
    params.addFlag("-s", false, "shell-compact", "Start command-line shell");
    params.addFlag("--config", "noconfig", "config", "Specify configuration file");
    params.addFlag("-c", "noconfig", "config-compact", "Specify configuration file");

    try {
        params.parseCommandLine(argc, argv);
        init(params);
        // init saves settings in config, now call run() passing settings)
        // If server starts, start TCP server
        run();
    } catch (std::exception& e) {
        //LogManager::getSingleton()->log(LogManager::ERROR, e.what());
        return 1;
    }

    // TODO: Remember to delete!    
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
        //db = new TrisDb(conf);
        shell();
    }
    // If no version, help or shell, read settings from file or use defaults and save in config var
    if (strcmp(params.getStringFlag("--config").c_str(), "noconfig") || strcmp(params.getStringFlag("-c").c_str(), "noconfig")) {
        // use specified config file
        LogManager::getSingleton()->log(LogManager::INFO, "Using config file " + params.getStringFlag("--config"));
        conf = Config::loadFromFile(params.getStringFlag("--config"));
    } else {
        // no config file, use defaults
        conf = new Config();
        conf->loadDefaults();
    }
}

void run() {
    LogManager::getSingleton()->log(LogManager::INFO, TRISDB_VERSION_STR);
    LogManager::getSingleton()->log(LogManager::INFO, "Using config " + conf->getName());
    LogManager::getSingleton()->log(LogManager::INFO, "Logging to " + conf->getSetting("logfile"));
    db = new TrisDb(conf);
    //test();

    TcpServer* tcp = new TcpServer(db);
    tcp->run();
    // UnixSocket *ux = new UnixSocket(db);
}

void test() {
    LogManager::getSingleton()->log(LogManager::INFO, "Starting performance tests");
    double now_1 = TimeUtils::getCurrentTimestamp();
    for (int i = 0; i < 1000000; i++) {
        db->create("Key" + std::to_string(i), "Chiave" + std::to_string(i), Utils::toString(i));
        if (i % 100000 == 0) {
            std::cout << std::to_string(i) << std::endl;
        }
    }
    double now_2 = TimeUtils::getCurrentTimestamp();
    double tempo = now_2 - now_1;
    double speed = 1000000.0 / (tempo / 1000.0);
    LogManager::getSingleton()->log(LogManager::INFO, "Writing 1,000,000 triples took " + std::to_string(tempo) + "ms");
    LogManager::getSingleton()->log(LogManager::INFO, "Write Speed: " + std::to_string(speed) + "/s");

    double now_3 = TimeUtils::getCurrentTimestamp();
    for (int i = 0; i < 1000000; i++) {
        Utils::ResultVector res = db->getFromA("Key" + std::to_string(i));
        if (i % 100000 == 0) {
            std::cout << std::to_string(i) << std::endl;
        }
    }
    double now_4 = TimeUtils::getCurrentTimestamp();
    double tempo_2 = now_4 - now_3;
    double speed_2 = 1000000.0 / (tempo_2 / 1000.0);
    LogManager::getSingleton()->log(LogManager::INFO, "Reading 1,000,000 triples took " + std::to_string(tempo_2) + "ms");
    LogManager::getSingleton()->log(LogManager::INFO, "Read Speed: " + std::to_string(speed_2) + "/s");
}

void shell() {
    LogManager::getSingleton()->log(LogManager::INFO, TRISDB_VERSION_STR);    
    Shell* shell = new Shell();
    shell->run();
    delete shell;
    exit(0);
}
