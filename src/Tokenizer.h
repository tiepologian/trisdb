/* 
 * File:   Tokenizer.h
 * Author: tiepologian <tiepolo.gian@gmail.com>
 *
 * Created on 23 maggio 2014, 0.16
 */

#ifndef TOKENIZER_H
#define	TOKENIZER_H

#include <iostream>
#include <string>

class Tokenizer {
public:
    static const std::string DELIMITERS;
    Tokenizer(const std::string& str);
    Tokenizer(const std::string& str, const std::string& delimiters);
    bool NextToken();
    bool NextToken(const std::string& delimiters);
    const std::string GetToken() const;
    void Reset();
protected:
    size_t m_offset;
    const std::string m_string;
    std::string m_token;
    std::string m_delimiters;
};

#endif	/* TOKENIZER_H */