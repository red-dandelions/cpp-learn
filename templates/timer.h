#ifndef __TEMPLATES_TIMER_H__
#define __TEMPLATES_TIMER_H__

#include <chrono>

namespace templates {
class Timer {
 private:
  typedef std::chrono::steady_clock clock;
  typedef std::chrono::nanoseconds  ns_;
  typedef std::chrono::microseconds us_;
  typedef std::chrono::milliseconds ms_;
  typedef std::chrono::seconds      s_;

 public:
  Timer() { start_ = clock::now(); }
  virtual ~Timer() {}

  inline void start() { start_ = clock::now(); }

  uint64_t ns() { return static_cast<uint64_t>(std::chrono::duration_cast<ns_>(clock::now() - start_).count()); }
  uint64_t us() { return static_cast<uint64_t>(std::chrono::duration_cast<us_>(clock::now() - start_).count()); }
  uint64_t ms() { return static_cast<uint64_t>(std::chrono::duration_cast<ms_>(clock::now() - start_).count()); }
  uint64_t s()  { return static_cast<uint64_t>(std::chrono::duration_cast<s_>( clock::now() - start_).count()); }

 private:
  std::chrono::time_point<clock> start_;
};


} // namespace templates

#endif