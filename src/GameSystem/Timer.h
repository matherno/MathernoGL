#pragma once
//
// Created by matt on 7/02/18.
//


class Timer
  {
private:
  long timeOut = 5000;
  long timer = 0;
  bool paused = false;

public:
  void reset()
    {
    timer = timeOut;
    }

  void setTimeOut(long timeOut)
    {
    this->timeOut = timeOut;
    }

  bool incrementTimer(long time)
    {
    if (!paused)
      timer -= time;
    return timedOut();
    }

  bool timedOut() const
    {
    return timer <= 0;
    }

  void setPaused(bool paused)
    {
    this->paused = paused;
    }
  };