#pragma once
//
// Created by matt on 3/12/17.
//

#include <chrono>
#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>

namespace mathernogl{

static long getTimeMS() {
  using namespace std::chrono;
  milliseconds ms = duration_cast<milliseconds>(
    system_clock::now().time_since_epoch()
  );
  return ms.count();
}

static long getTimeUS(){
  using namespace std::chrono;
  microseconds us = duration_cast< microseconds >(
    system_clock::now().time_since_epoch()
  );
  return us.count();
}

template<typename ... Args>
static std::string stringFormat(const std::string& format, Args ... args){
  int bufferSize = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
  char* buffer = new char[bufferSize];
  snprintf(buffer, bufferSize, format.c_str(), args ...);
  return std::string(buffer, buffer + bufferSize - 1);
}

static const long msInSec = 1000;
static const long msInMin = msInSec * 60;
static const long msInHour = msInMin * 60;

static std::string formatTime(uint timeMS)
  {
  const long ms = timeMS % 1000;
  const long s = (long) floor((double) timeMS / msInSec) % 60;
  const long m = (long) floor((double) timeMS / msInMin) % 60;
  const long h = (long) floor((double) timeMS / msInHour);
  return std::to_string(h) + ":" + std::to_string(m) + ":" + std::to_string(s) + ":" + std::to_string(ms);
  }

static std::vector<std::string> stringSplit(std::string stringToSplit, char delimiter)
  {
  std::stringstream stream(stringToSplit);
  std::string item;
  std::vector<std::string> splitStrings;
  while (std::getline(stream, item, delimiter))
    splitStrings.push_back(item);
  return splitStrings;
  }

}

