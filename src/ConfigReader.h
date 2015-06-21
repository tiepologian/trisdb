/* 
 * File:    ConfigReader.h
 * Author:  erwi (Ero Willman)
 * Link:    github.com/erwi/reader
 *
 * Created on 15 giugno 2014, 16.03
 */

#ifndef CONFIGREADER_H
#define	CONFIGREADER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <fstream>
#include <algorithm>
#include<regex>

struct ReaderError {
    std::string errorMessage;
    std::string fileName;
    size_t lineNumber;
    std::string lineText;

    ReaderError(const std::string &errorMessage,
            const std::string &fileName = "",
            const size_t &lineNumber = 0,
            const std::string &lineText = ""
            ) :
    errorMessage(errorMessage),
    fileName(fileName),
    lineNumber(lineNumber),
    lineText(lineText) {
    }

    void printError() {
        /*!
Prints error info to stderr.
         */
        if ((fileName.size()) && (lineNumber == 0)) { // only filename
            std::cerr << "Error in file :" << fileName << std::endl;
        } else if ((fileName.size()) && (this->lineNumber > 0)) { // filename AND line number
            std::cerr << "Error in file " << fileName << " on line " << this->lineNumber << std::endl;
            std::cerr << this->lineText << std::endl;
        }
        std::cerr << this->errorMessage << std::endl;
    }
};

class Reader {

    struct lineData { // a record for storing single key/value definition line data
        size_t _lineNumber; // line number in source file
        std::string _rawText; // raw unformatted text of the line
        std::string _val; // formatted value stored as a string
    };
    bool _isCaseSensitive; // if not case sensitive, all text will be converted to lowercase
    // ERROR MESSAGES
    const std::string _R_FILE_OPEN_ERROR_MESSAGE = "Could not open file :";
    const std::string _R_VECTOR_FORMAT_ERROR_MSG = "Bad vector format";
    const std::string _R_KEY_NOT_FOUND_ERROR_MSG = "Key not found :";
    const std::string _R_NUMBER_FORMAT_ERROR_MSG = "Bad number format :";
    const std::string _R_BAD_VALUE_ERROR_MSG = "Bad value format :";
    const std::string _R_BAD_KEY_VALUE_ERROR_MSG = "Bad key/value assignment";
    // CHARACTERS USED FOR VALIDATING KEYS AND VALUES
    const std::string _R_WHITE_SPACE = " \t"; // white space characters
    const std::string _R_COMMENT_CHARS = "#";
    const std::string _R_DIGIT_CHARS = "1234567890-.";

    // VECTOR DELIMITERS
    const char _R_OBRACE = '[';
    const char _R_CBRACE = ']';
    const char _R_VDELIM = ',';

    std::string _fileName; // holds current file name
    std::map<std::string, lineData> _keyValues; // "database" of all read key/value pairs
    std::vector<std::string> _validKeys; // optional list of valid keys. read in from a separte file
    std::string _recentKey; // previously accessed/searched key that is valid and found
    // STRING FORMATTING FUNCTIONS
    inline void removeComments(std::string &line) const; // removes everything in a line after comment character
    inline void cleanLineEnds(std::string &line) const; // removes white space from both ends of a string
    inline bool splitByChar(const std::string &line, std::string &key, std::string &value, const char &split = '=') const;
    inline bool isValidNumber(const std::string &strVal) const; // returns false if not a valid number
    inline size_t getLineNumberByKey(const std::string &key) const; // find line number in file where a given key is defined
    inline void toLower(std::string &s) const; // converts string to all lower case

    // SPECIALISATIONS FOR HANDLING DIFFERENT VALUE TYPES
    inline void parseValue(const std::string &strVal, std::string &val) const;
    inline void parseValue(const std::string &strVal, std::vector<std::string> &val)const; // make array of strings
    inline void parseValue(const std::string &strVal, size_t &val) const; // size_t conversion
    #ifdef __x86_64__
    inline void parseValue(const std::string &strVal, unsigned int &val) const; // 32 bit unsigned int conversion
    #endif

    template <class T > inline void parseValue(const std::string &strVal, std::vector<T> &val) const {
        /*!
         * Converts to a vector of numbers
         */
        // first convert to a vector od strings
        std::vector <std::string> svals;
        parseValue(strVal, svals);
        // then convert each item to a number
        for (auto &s : svals) {
            if (!isValidNumber(s)) {
                std::string errMSG = _R_NUMBER_FORMAT_ERROR_MSG + s;
                throw errMSG;
            }
            std::stringstream ss(s);
            double tval(0);
            ss >> tval;
            val.push_back(static_cast<T> (tval));
        }
    }

    template <class T > inline void parseValue(const std::string &strVal, T &val) const {
        /*!
Converts to numebers
         */
        if (!isValidNumber(strVal)) {
            std::string errMSG = _R_NUMBER_FORMAT_ERROR_MSG + strVal;
            throw errMSG;
        }
        std::string sval;
        parseValue(strVal, sval);
        // first convert to double, then to other if needed
        std::stringstream ss(sval);
        double tval(0);
        ss >> tval;
        val = static_cast<T> (tval);
    }
public:

    inline Reader() : _isCaseSensitive(true) {
    }
    inline void readSettingsFile(const std::string &fileName);
    inline void readValidKeysFile(const std::string &fileName);
    inline bool containsKey(const std::string &key);
    template<class T> inline T getValueByKey(const std::string &key) const;
    template<class T> inline T get() const; // access by last used key
    template<class T> inline T get(const std::string &key, const T& defaultValue);
    inline void printAll()const;

    inline bool isCaseSensitive()const {
        return _isCaseSensitive;
    }
    inline bool isValidKey(std::string key);

    inline void setCaseSensitivity(const bool &isCS) {
        _isCaseSensitive = isCS;
    }
    inline std::vector<std::string> getAllKeys() const; // returns vector containing all keys in file
};

//****************************************************************************************
//
// METHOD DEFINITIONS
//
//****************************************************************************************

inline void Reader::toLower(std::string &s) const {
    std::locale loc;
    for (size_t i = 0; i < s.size(); i++) {
        s[i] = std::tolower(s[i], loc);
    }
}

inline void Reader::cleanLineEnds(std::string &line) const {
    /*! Removes white space from both ends of input string*/
    if (line.size() == 0) { // empty line
        return;
    }
    // REMOVE WHITE SPACE CHARACTERS AT START OF LINE
    size_t ind = line.find_first_not_of(_R_WHITE_SPACE);
    if (ind == std::string::npos) { // if white space only line
        line = "";
        return;
    } else if (ind > 0) { // white space at start of line
        line = line.substr(ind);
    }
    // REMOVE WHITE SPACE CHARACTERS AT END OF LINE
    ind = line.find_last_not_of(_R_WHITE_SPACE);
    if (ind < line.size() - 1) {
        line = line.substr(0, ind + 1);
    }
}

inline void Reader::removeComments(std::string &line) const {
    /*! Removes everything after comment character*/
    size_t ind = line.find_first_of(_R_COMMENT_CHARS.c_str());
    if (ind != std::string::npos) {
        line = line.substr(0, ind);
    }
}

inline bool Reader::splitByChar(const std::string &line,
        std::string &key,
        std::string &value,
        const char &split
        ) const {
    /*!
     * Breaks line into two parts by specified split character. Returns true/false if success/fail.
     * If split chracter is not defined, default '=' is used
     */
    // MAKE SURE EXACTLY ONE '=' CHARACTER ON LINE
    size_t ind = line.find_first_of(split);
    size_t ind2 = line.find_last_of(split);
    if ((ind == std::string::npos) || // if no equals sign
            (ind != ind2)) { // or more than 1
        return false;
    }
    // GET NON-EMPTY KEY/VALUE STRINGS
    key = line.substr(0, ind);
    value = line.substr(ind + 1);
    cleanLineEnds(key);
    cleanLineEnds(value);
    if ((key.empty()) || (value.empty())) {
        return false;
    }
    return true;
}

void Reader::readSettingsFile(const std::string &fileName) {
    /*!
     * Reads text file and stores found key/value pair definitions in a
     * map "database". Does initial error checking for validity of format.
     */
    _keyValues.clear();
    _fileName = fileName;
    std::ifstream fin(_fileName.c_str());
    if (!fin.is_open()) {
        throw ReaderError(_R_FILE_OPEN_ERROR_MESSAGE + _fileName);
    }
    std::string line;
    size_t lineNumber = 1;
    // READ IN RAW TEXT LINE BY LINE
    do {
        std::string rawLine;
        std::getline(fin, rawLine);
        std::string line(rawLine);
        removeComments(line);
        cleanLineEnds(line);
        if (line.empty()) { // skip rest of test for empty lines
            lineNumber++;
            continue;
        }
        // EXTRACT KEY AND VALUES AS STRINGS
        std::string key;
        std::string value;
        if (!splitByChar(line, key, value)) {
            fin.close();
            throw ReaderError(_R_BAD_VALUE_ERROR_MSG, _fileName, lineNumber, rawLine);
        }
        // HANDLE CASE SENSITIVITY
        std::string rawKey = key;
        std::string rawValue = value;
        if (!isCaseSensitive()) {
            toLower(key);
            toLower(value);
        }
        // IF VALID KEYS HAVE BEEN DEFINED, CHECK THIS IS ONE
        if ((_validKeys.size() > 0) && !isValidKey(key)) {
            throw ReaderError(rawKey + " is not a valid key.", _fileName, lineNumber, rawLine);
        }
        // DISALLOW REDEFINITION OF EXISTING KEYS
        if (containsKey(key)) {
            size_t prevDefLine = this->getLineNumberByKey(key);
            std::stringstream line1;
            line1 << prevDefLine;
            std::string msg = "Multiple definitions of \"" + key + "\", which is already defined on line " +
                    line1.str();
            throw ReaderError(msg, _fileName, lineNumber, rawLine);
        }
        // IF OK SO FAR - SAVE LINE INFO TO "DATABASE"
        lineData valueData;
        valueData._lineNumber = lineNumber;
        valueData._rawText = rawLine;
        valueData._val = value;
        _keyValues[key] = valueData;
        lineNumber++;
    } while (!fin.eof());
    fin.close();
}

void Reader::readValidKeysFile(const std::string &fileName) {
    /*! Reads a text file containing a list of all allowed keys.
This should be called before reading in the actual settings file*/
    // Make sure this is called before reading the actual settings file
    if (_keyValues.size() > 0) {
        throw ReaderError("Valid keys file should be read first.", fileName);
    }
    std::ifstream fin(fileName.c_str());
    if (!fin.is_open()) {
        throw ReaderError(_R_FILE_OPEN_ERROR_MESSAGE + fileName);
    }
    std::string line;
    size_t lineNumber = 1;
    // READ IN RAW TEXT LINE BY LINE
    do {
        std::string rawLine;
        std::getline(fin, rawLine);
        std::string line(rawLine);
        removeComments(line);
        cleanLineEnds(line);
        if (line.empty()) { // skip rest of test for empty lines
            lineNumber++;
            continue;
        }
        // EXTRACT KEY AND VALUES AS STRINGS
        std::string key;
        std::string value;
        if (!splitByChar(line, key, value)) {
            fin.close();
            throw ReaderError(_R_BAD_VALUE_ERROR_MSG, fileName, lineNumber, rawLine);
        }
        // HANDLE CASE SENSITIVITY
        if (!isCaseSensitive()) {
            toLower(key);
            toLower(value);
        }
        // need to replace wildcard "*" with regex wildcard ".*"
        size_t ind = 0;
        while (ind < std::string::npos) { // loop to cover multiple wildcards in same key
            ind = key.find("*", ind);
            if (ind < std::string::npos) {
                key.replace(ind, 1, ".*");
                ind += 2; // increment by 2 to avoid infinite loop over same *->.*
            }
        }
        // IF OK SO FAR - SAVE LINE INFO TO "DATABASE"
        _validKeys.push_back(key);
        lineNumber++;
    } while (!fin.eof());
    fin.close();
}

bool Reader::isValidKey(std::string key) {
    /*! Checks whether key is in list of valid keys.
Keys may contain wildcards, marked with '*'
     */
    // compare each valid key to current key
    for (auto &k : _validKeys) {
        std::regex rx(k);
        if (std::regex_match(key.begin(), key.end(), rx)) {
            _recentKey = key; // remember key. value can then be accessed with get()
            return true;
        }
    }
    _recentKey.clear();
    return false; // key not found
}

size_t Reader::getLineNumberByKey(const std::string &key) const {
    /*!
     * Returns line number in file where a given key/value definition occurs.
     */
    auto itr = _keyValues.find(key);
    if (itr == _keyValues.end()) { // if not found
        throw std::string("could not find key: ") + key;
    }
    return itr->second._lineNumber;
}

bool Reader::containsKey(const std::string &key) {
    /*!
     * Returns true if key has been defined.
     */
    _recentKey.clear();
    // validate key format
    std::string tkey(key);
    this->cleanLineEnds(tkey);
    if (!isCaseSensitive()) toLower(tkey); // if case insensitive
    std::map<std::string, lineData>::const_iterator itr = _keyValues.find(tkey);
    //
    if (itr != _keyValues.end()) { // if contains given key
        _recentKey = key; // save as most recently used
        return true;
    } else {
        return false;
    }
}

void Reader::printAll() const {
    /*!
     * Prints all key/value pairs to stdout.
     */
    for (auto &k : _keyValues) {
        std::cout << k.first << " = " << k.second._val << std::endl;
    }
}

bool Reader::isValidNumber(const std::string &strVal) const {
    /*!
Tries to validate that the input string can be converted to a valid number.
Does not currently catch many special cases. (repeated '-', '.' and 'e' characters)
     */
    // IF NUMBER USES SCIENTIFIC NOTATION, e.g.: 1e-2 , SPLIT IT BY e AND CHECK EACH SEPARATELY
    std::string num1, num2;
    if (splitByChar(strVal, num1, num2, 'e')) { // IF 'scientific' notation fractional: 2e-5
        return isValidNumber(num1) && isValidNumber(num2); // both parts must be valid
    } else {
        // if contains non-numeric chars
        if (strVal.find_first_not_of(_R_DIGIT_CHARS) < std::string::npos) {
            return false;
        } else
            // if contains multiple dots or minuses
            if ((std::count(strVal.begin(), strVal.end(), '.') > 1) ||
                (std::count(strVal.begin(), strVal.end(), '-') > 1)) {
            return false;
        }
        // if minus sign found, but not in first position
        size_t minPos = strVal.find_first_of('-');
        if ((minPos > 0) && (minPos < std::string::npos)) {
            return false;
        }
        // found no errors
        return true;
    }
}

void Reader::parseValue(const std::string &strVal, std::string &val) const {
    /*!
     * Parses a string to a string-value, checking for whitesapces. This method is called
     * first in numeric conversions too.
     */
    val = strVal;
    // Remove white space at start and end ov current value
    cleanLineEnds(val);
    // check that no whitespace is present
    size_t ind = val.find_first_of(_R_WHITE_SPACE);
    if (ind < std::string::npos) {
        std::string errMsg = _R_BAD_VALUE_ERROR_MSG + strVal + ", found unexpected whitespace.";
        throw errMsg;
    }
}

void Reader::parseValue(const std::string &strVal, std::vector<std::string> &val) const {
    /*!
     * Parses string to a vector of strings. Checks for validity of vector formatting.
     * Is called first with numeric vectors too.
     */
    std::string str(strVal); // make working copy
    size_t i1, i2;
    // CHECK FOR OPEN/CLOSE BRAES
    if ((i1 = str.find_first_of(_R_OBRACE)) != str.find_last_of(_R_OBRACE)) {
        throw _R_VECTOR_FORMAT_ERROR_MSG; // multiple '['
    }
    if ((i2 = str.find_first_of(_R_CBRACE)) != str.find_last_of(_R_CBRACE)) {
        throw _R_VECTOR_FORMAT_ERROR_MSG; // multiple ']'
    }
    if (i1 > 0) {
        throw _R_VECTOR_FORMAT_ERROR_MSG; // '[' not first
    }
    if (i2 < str.length() - 1) {
        throw _R_VECTOR_FORMAT_ERROR_MSG; //']' not last
    }
    if (i2 == std::string::npos) {
        throw _R_VECTOR_FORMAT_ERROR_MSG; // no closing ']'
    }
    if (i1 == i2 - 1) {
        throw _R_VECTOR_FORMAT_ERROR_MSG; // empty vector
    }
    str = str.substr(i1 + 1, i2 - 1); // remove braces from both ends
    // REPLACE ALL DELIMETERS BY SINGLE BLANKS
    while ((i1 = str.find_first_of(_R_VDELIM)) < std::string::npos) {
        if (i1 == 0) { // first character is vector delimiter
            throw _R_VECTOR_FORMAT_ERROR_MSG;
        }
        std::string item = str.substr(0, i1);
        parseValue(item, item); // check that item is a valid string-value
        val.push_back(item);
        str = str.substr(i1 + 1, str.length() - i1); // remove delimeter cahracter
    }
    // reached end of delimited vector (no more commas left), read last item
    if ((i1 == std::string::npos) && (str.length() > 0)) {
        parseValue(str, str); // check that item is a valid string-value
        val.push_back(str);
    } else {
        throw _R_VECTOR_FORMAT_ERROR_MSG;
    }
}

template<class T>
T Reader::getValueByKey(const std::string &key) const {
    /*!
     * Returns value corresponding to specified key.
     */
    // create temporary working key-string
    std::string tkey(key);
    cleanLineEnds(tkey);
    if (!isCaseSensitive()) toLower(tkey);
    // First make sure key exists
    auto const &itr = _keyValues.find(tkey);
    if (itr == _keyValues.end()) {
        throw ReaderError(_R_KEY_NOT_FOUND_ERROR_MSG + key, _fileName);
    }
    // Get value stored as a string and comvert to
    // specified type
    const std::string &strVal = itr->second._val;
    try {
        T retVal;
        parseValue(strVal, retVal);
        return retVal;
    } catch (std::string msg) { // error converting to required type
        lineData badLine = (_keyValues.find(tkey)->second);
        throw ReaderError(msg, _fileName, badLine._lineNumber, badLine._rawText);
    }
}

template<class T>
T Reader::get() const {
    /*! returns value corresponding to most recently used key*/
    if (!_recentKey.empty())
        return getValueByKey<T>(_recentKey);
    else
        throw ReaderError("A valid key has not been found before calling Reader::get<class T>()");
}

template<class T>
T Reader::get(const std::string &key, const T& defaultValue) {
    /*! Returns value by key. If key does not exit, returns given default value*/
    if (containsKey(key))
        return get<T>();
    else
        return defaultValue;
}

std::vector<std::string> Reader::getAllKeys() const {
    /*! Returns vector containing all keys in input file */
    std::vector<std::string> returnKeys;
    for (auto& kv : _keyValues)
        returnKeys.push_back(kv.first);
    return returnKeys;
}

inline void Reader::parseValue(const std::string &strVal, size_t &val) const {
    /*!
     * Parses to size_t type (may be 64 bit). Checks that value is positive.
     */
    if (!isValidNumber(strVal)) {
        throw _R_NUMBER_FORMAT_ERROR_MSG;
    }
    double dval(0);
    parseValue(strVal, dval);
    if (dval >= 0) {
        val = static_cast<size_t> (dval);
    } else {
        std::string errMsg = _R_BAD_VALUE_ERROR_MSG + strVal + " , expected positive number.";
        throw errMsg;
    }
}

#ifdef __x86_64__
inline void Reader::parseValue(const std::string &strVal, unsigned int &val) const {
    /*!
     * Parses to 32 bit unsigned int. Checks that value is positive.
     */
    // First convert to size_t type, then to unisgned.
    size_t tval(0);
    parseValue(strVal, tval);
    val = static_cast<unsigned int> (tval);
}
#endif

#endif	/* CONFIGREADER_H */

