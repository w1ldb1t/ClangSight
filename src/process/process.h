#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "base/logging.h"
#include "base/platform.h"

class Process {
public:
  virtual ~Process() = default;
  
  virtual auto Start(const std::string &command) -> bool = 0;
  virtual auto IsRunning() -> bool = 0;
  virtual auto ReadConsoleText() -> std::string = 0;
  virtual auto WriteConsoleText(const std::string &input) -> bool = 0;
  virtual auto IsInitialized() -> bool = 0;
  virtual void Stop() = 0;
};

#endif //_PROCESS_H_