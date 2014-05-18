/* 
 * File:   TripleMap.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 17 maggio 2014, 17.01
 */

#ifndef TRIPLEMAP_H
#define	TRIPLEMAP_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

template <typename T1, typename T2, typename T3>
class TripleMap {
    typedef std::vector<std::tuple<T1, T2, T3>> ResultVector;
public:

    void add(T1 t1, T2 t2, T3 t3) {
        m1.insert(std::pair<T1, std::tuple<T1, T2, T3 >> (t1, std::make_tuple(t1, t2, t3)));
        m2.insert(std::pair<T2, std::tuple<T1, T2, T3 >> (t2, std::make_tuple(t1, t2, t3)));
        m3.insert(std::pair<T3, std::tuple<T1, T2, T3 >> (t3, std::make_tuple(t1, t2, t3)));
    }

    ResultVector getA(T1 t1) {
        ResultVector result;
        auto range = m1.equal_range(t1);
        for (auto it = range.first; it != range.second; ++it) {
            result.push_back((*it).second);
        }
        return result;
    }

    ResultVector getB(T2 t2) {
        ResultVector result;
        auto range = m2.equal_range(t2);
        for (auto it = range.first; it != range.second; ++it) {
            result.push_back((*it).second);
        }
        return result;
    }

    ResultVector getC(T3 t3) {
        ResultVector result;
        auto range = m3.equal_range(t3);
        for (auto it = range.first; it != range.second; ++it) {
            result.push_back((*it).second);
        }
        return result;
    }
private:
    std::unordered_multimap<T1, std::tuple<T1, T2, T3>> m1;
    std::unordered_multimap<T2, std::tuple<T1, T2, T3>> m2;
    std::unordered_multimap<T3, std::tuple<T1, T2, T3>> m3;
};

#endif	/* TRIPLEMAP_H */