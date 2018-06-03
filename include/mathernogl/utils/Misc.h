#pragma once
//
// Created by matt on 3/12/17.
//

#include <chrono>
#include <iostream>

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

}

