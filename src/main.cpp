/*
 * File:   main.cpp
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 17 maggio 2014, 13.02
 */

#include <string>
#include "TripleMap.h"
#include "TimeUtils.h"
#include "Utils.h"
#include "param_t.h"

int main(int argc, char *argv[]) {    
    param_t params;
    params.setPreamble("TrisDB.\nThis is a help message.\n");
    params.addFlag("--version", false, "version", "Show version number");
    try {
        params.parseCommandLine(argc, argv);
    }
    catch (...) {
        return 1;
    }
    
    if(params.getBoolFlag("--version")) {
        std::cout << TRISDB_VERSION_STR << std::endl;
        return 0;
    }

    TripleMap<std::string, std::string, int> mappa;

    double now_1 = TimeUtils::getCurrentTimestamp();
    for (int i = 0; i < 1000000; i++) {
        mappa.add("Key" + std::to_string(i), "Chiave" + std::to_string(i), i);
        if (i % 100000 == 0) {
            std::cout << std::to_string(i) << std::endl;
        }
    }
    double now_2 = TimeUtils::getCurrentTimestamp();
    double tempo = now_2 - now_1;
    double speed = 1000000.0 / (tempo/1000.0);
    std::cout << "\nWriting 1,000,000 triples took " << tempo << "ms" << std::endl;
    std::cout << "Write Speed: " << speed << "/s" << std::endl;

    double now_3 = TimeUtils::getCurrentTimestamp();
    for (int i = 0; i < 1000000; i++) {
        auto res = mappa.getA("Key" + std::to_string(i));
        std::string result = std::get<0>(res);
        if (i % 100000 == 0) {
            std::cout << std::to_string(i) << std::endl;
        }
    }
    double now_4 = TimeUtils::getCurrentTimestamp();
    double tempo_2 = now_4 - now_3;
    double speed_2 = 1000000.0 / (tempo_2/1000.0);
    std::cout << "\nReading 1,000,000 triples took " << tempo_2 << "ms" << std::endl;
    std::cout << "Read Speed: " << speed_2 << "/s" << std::endl;

    std::cout << "COMMAND:" << std::endl;
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        unsigned pos = input.find(" ");
        if (input.substr(0, pos) == "GETA") {
            // get by name
            auto res = mappa.getA(input.substr(pos + 1));
            if (std::get<0>(res) == "") std::cout << "NOT FOUND" << std::endl;
            else std::cout << std::get<0>(res) << " " << std::get<1>(res) << " - " << std::get<2>(res) << std::endl;
        } else if (input.substr(0, pos) == "GETB") {
            // get by surname
            auto res2 = mappa.getB(input.substr(pos + 1));
            if (std::get<0>(res2) == "") std::cout << "NOT FOUND" << std::endl;
            else std::cout << std::get<0>(res2) << " " << std::get<1>(res2) << " - " << std::get<2>(res2) << std::endl;
        }
    }

    return 0;
}
