#pragma once
//
// Created by matt on 3/12/17.
//

#include <chrono>

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

}

