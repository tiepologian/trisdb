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
#include <set>
#include "Utils.h"

template <typename T1, typename T2, typename T3>
class TripleMap {
    //typedef std::set<std::tuple<T1, T2, T3>> ResultVector;
public:

    void add(T1 t1, T2 t2, T3 t3) {
        m1.insert(std::pair<T1, std::tuple<T1, T2, T3 >> (t1, std::make_tuple(t1, t2, t3)));
        m2.insert(std::pair<T2, std::tuple<T1, T2, T3 >> (t2, std::make_tuple(t1, t2, t3)));
        m3.insert(std::pair<T3, std::tuple<T1, T2, T3 >> (t3, std::make_tuple(t1, t2, t3)));
    }

    bool remove(T1 t1, T2 t2, T3 t3) {
        // search for t1 in first index, check if other params are equal and delete
        if (t1 != Utils::kQueryWildcard) {
            auto range = m1.equal_range(t1);
            for (auto it = range.first; it != range.second;) {
                if (isEqual(std::get<1>((*it).second), t2) && isEqual(std::get<2>((*it).second), t3)) {
                    removeFromB(std::get<0>((*it).second), std::get<1>((*it).second), std::get<2>((*it).second));
                    removeFromC(std::get<0>((*it).second), std::get<1>((*it).second), std::get<2>((*it).second));
                    m1.erase(it++);
                }
                else ++it;
            }
        } else if (t2 != Utils::kQueryWildcard) {
            // t1 is a wildcard, let's use t2
            auto range = m2.equal_range(t2);
            for (auto it = range.first; it != range.second;) {
                if (isEqual(std::get<0>((*it).second), t1) && isEqual(std::get<2>((*it).second), t3)) {
                    removeFromA(std::get<0>((*it).second), std::get<1>((*it).second), std::get<2>((*it).second));
                    removeFromC(std::get<0>((*it).second), std::get<1>((*it).second), std::get<2>((*it).second));
                    m2.erase(it++);
                }
                else ++it;
            }
        } else if (t3 != Utils::kQueryWildcard) {
            // t1 and t2 are wildcard, let's use t3
            auto range = m3.equal_range(t3);
            for (auto it = range.first; it != range.second;) {
                if (isEqual(std::get<1>((*it).second), t2) && isEqual(std::get<0>((*it).second), t1)) {
                    removeFromB(std::get<0>((*it).second), std::get<1>((*it).second), std::get<2>((*it).second));
                    removeFromA(std::get<0>((*it).second), std::get<1>((*it).second), std::get<2>((*it).second));
                    m3.erase(it++);
                }
                else ++it;
            }
        } else {
            // all wildcards, delete everything!
            clearAll();
        }
    }

    Utils::ResultVector getA(T1 t1) {
        Utils::ResultVector result;
        auto range = m1.equal_range(t1);
        for (auto it = range.first; it != range.second; ++it) {
            result.push_back((*it).second);
        }
        return result;
    }

    Utils::ResultVector getB(T2 t2) {
        Utils::ResultVector result;
        auto range = m2.equal_range(t2);
        for (auto it = range.first; it != range.second; ++it) {
            result.push_back((*it).second);
        }
        return result;
    }

    Utils::ResultVector getC(T3 t3) {
        Utils::ResultVector result;
        auto range = m3.equal_range(t3);
        for (auto it = range.first; it != range.second; ++it) {
            result.push_back((*it).second);
        }
        return result;
    }

    Utils::ResultVector getAll() {
        Utils::ResultVector result;
        for (std::unordered_multimap<std::string, std::tuple < std::string, std::string, std::string>>::iterator it = m1.begin(); it != m1.end(); ++it)
            result.push_back((*it).second);

        return result;
    }

    void clearAll() {
        m1.clear();
        m2.clear();
        m3.clear();
    }
private:
    std::unordered_multimap<T1, std::tuple<T1, T2, T3>> m1;
    std::unordered_multimap<T2, std::tuple<T1, T2, T3>> m2;
    std::unordered_multimap<T3, std::tuple<T1, T2, T3>> m3;

    bool isEqual(std::string s1, std::string s2) {
        if ((s2 == Utils::kQueryWildcard) || (s1 == s2)) return true;
        else return false;
    }

    void removeFromA(std::string s1, std::string s2, std::string s3) {
        // search in second index using s2 and then delete
        auto range = m1.equal_range(s1);
        for (auto it = range.first; it != range.second;) {
            if (isEqual(std::get<1>((*it).second), s2) && isEqual(std::get<2>((*it).second), s3)) {
                m1.erase(it++);
            }
            else ++it;
        }
    }

    void removeFromB(std::string s1, std::string s2, std::string s3) {
        // search in second index using s2 and then delete
        auto range = m2.equal_range(s2);
        for (auto it = range.first; it != range.second;) {
            if (isEqual(std::get<0>((*it).second), s1) && isEqual(std::get<2>((*it).second), s3)) {
                m2.erase(it++);
            }
            else ++it;
        }
    }

    void removeFromC(std::string s1, std::string s2, std::string s3) {
        // search in third  index using s3 and then delete
        auto range = m3.equal_range(s3);
        for (auto it = range.first; it != range.second;) {
            if (isEqual(std::get<0>((*it).second), s1) && isEqual(std::get<1>((*it).second), s2)) {
                m3.erase(it++);
            }
            else ++it;
        }
    }
};

#endif	/* TRIPLEMAP_H */