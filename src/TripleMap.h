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

template <typename T1, typename T2, typename T3>
class TripleMap {
public:

    void add(T1 t1, T2 t2, T3 t3) {
        m1[t1] = std::make_tuple(t1, t2, t3);
        m2[t2] = std::make_tuple(t1, t2, t3);
    }

    std::tuple<T1, T2, T3> getA(T1 t1) {
        return m1[t1];
    }

    std::tuple<T1, T2, T3> getB(T2 t2) {
        return m2[t2];
    }
private:
    std::unordered_map<T1, std::tuple<T1, T2, T3>> m1;
    std::unordered_map<T2, std::tuple<T1, T2, T3>> m2;
};

#endif	/* TRIPLEMAP_H */