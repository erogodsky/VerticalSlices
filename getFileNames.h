#pragma once

#include <iostream>
#include <filesystem>

using namespace std;
namespace stdfs = std::filesystem;

vector<string> getFileNames(stdfs::path path);