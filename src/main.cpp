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
#include "cxxopts.hpp"
#include "Shell.h"

void init(cxxopts::Options options);
void run();
void shell(std::string port, std::string socket);

Config* conf = nullptr;
TrisDb *db = nullptr;

int main(int argc, char *argv[]) {
	try {
		cxxopts::Options options(argv[0], " - command line options");
		options.add_options()("c,config", "configuration file",
				cxxopts::value<std::string>(), "FILE")("s,shell",
				"start command-line shell")("b,benchmark",
				"start benchmark utility")("v,version", "print version number")(
				"h,help", "print help");

		options.add_options("Shell")("port", "Tcp port", cxxopts::value<int>(),
				"PORT")("socket", "Unix socket", cxxopts::value<std::string>(),
				"FILE");

		options.parse(argc, argv);
		init(options);
		run();
	} catch (std::exception& e) {
		LogManager::getSingleton()->log(LogManager::LERROR, e.what());
		return 1;
	}

	delete conf;
	LogManager::getSingleton()->log(LogManager::LINFO, "Exiting, bye bye");
	return 0;
}

void init(cxxopts::Options options) {
	if (options.count("help")) {
		std::cout << options.help( { "", "Shell" }) << std::endl;
		exit(0);
	} else if (options.count("version")) {
		std::cout << TRISDB_VERSION_STR << std::endl;
		exit(0);
	} else if (options.count("shell")) {
		if (options.count("port")) {
			shell(std::to_string(options["port"].as<int>()), "nosocket");
		} else if (options.count("socket")) {
			shell("", options["socket"].as<std::string>());
		} else {
			shell("1205", "nosocket");
		}
	} else if (options.count("benchmark")) {
		conf = new Config();
		conf->loadDefaults();
		db = new TrisDb(conf);
		db->benchmark();
		delete db;
		delete conf;
		exit(0);
	} else {
		// If no version, help or shell, read settings from file or use defaults and save in config var
		if (options.count("config")) {
			conf = Config::loadFromFile(options["config"].as<std::string>());
		} else {
			// no config file, use defaults
			conf = new Config();
			conf->loadDefaults();
		}
	}
}

void run() {
	Utils::printAsciiLogo();
	LogManager::getSingleton()->log(LogManager::LINFO, TRISDB_VERSION_STR);
	LogManager::getSingleton()->log(LogManager::LINFO,
			"Using config " + conf->getName());
	LogManager::getSingleton()->log(LogManager::LINFO,
			"Logging to " + conf->getSetting("logfile"));
	db = new TrisDb(conf);
	db->run();
	LogManager::getSingleton()->log(LogManager::LINFO,
			"Shutting down database");
	delete db;
}

void shell(std::string port, std::string socket) {
	LogManager::getSingleton()->log(LogManager::LINFO, TRISDB_VERSION_STR);
	Shell* shell = new Shell(port, socket);
	shell->run();
	delete shell;
	exit(0);
}
