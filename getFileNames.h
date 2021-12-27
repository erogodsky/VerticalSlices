#pragma once

#include <iostream>
#include <filesystem>
#include "getFileNames.h"

using namespace std;
namespace stdfs = std::filesystem;

vector<string> getFileNames(stdfs::path path = "ANONIM");