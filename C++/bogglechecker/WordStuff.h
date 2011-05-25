#pragma  once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

void LoadFile( const std::string &filename, std::string &destination );

void SplitWords( const std::string &words, std::vector<std::string> &subwords );

void Output( const std::string &filename, const std::vector<std::string> &subwords );
