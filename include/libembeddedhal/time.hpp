#pragma once

#include <chrono>
#include <functional>

namespace embed::this_thread {
/**
 * @brief smallest increment of time for clocks
 *
 */
using time_increment = std::chrono::nanoseconds;
/**
 * @brief definition of a sleep function
 *
 */
using sleep_function = std::function<void(time_increment p_sleep_time)>;
/**
 * @brief definition of an uptime function
 *
 */
using uptime_function = std::function<time_increment(void)>;

/**
 * @brief structure containing internal clock variables and functions
 *
 */
struct global_clocks
{
public:
  /**
   * @brief Default uptime timer that simply counts each time it is called. This
   * should almost never be used.
   *
   * @return time_increment fake uptime
   */
  static time_increment incremental_uptime()
  {
    using namespace std::chrono_literals;
    static time_increment count{ 0 };
    count += 100ns;
    return count;
  }
  /**
   * @brief Default sleep function that simply counts until it reaches a
   * designated number.
   *
   * @param p_loop_count number of iterations to loop for to mimick sleeping
   */
  static void loop_sleep(time_increment p_loop_count)
  {
    using namespace std::chrono_literals;
    time_increment count{ 0 };
    while (count < p_loop_count) {
      count += 100us;
    }
  }
  /**
   * @brief Store the global sleep function
   *
   */
  static inline sleep_function m_global_sleep = loop_sleep;
  /**
   * @brief Store the global uptime function
   *
   */
  static inline uptime_function m_global_uptime = incremental_uptime;
};

/**
 * @brief Delay execution of the program by a predefined time
 *
 * @param p_delay the amount of time to delay execution by
 */
static auto sleep_for(time_increment p_delay)
{
  if (global_clocks::m_global_sleep) {
    return global_clocks::m_global_sleep(p_delay);
  } else {
    return global_clocks::loop_sleep(p_delay);
  }
}
/**
 * @brief Get the current system uptime
 *
 * @return auto the global uptime
 */
static auto uptime()
{
  if (global_clocks::m_global_uptime) {
    return global_clocks::m_global_uptime();
  } else {
    return global_clocks::incremental_uptime();
  }
}

/**
 * @brief Set the global sleep object
 *
 * @param p_sleep_function the function to handle sleeping
 */
static void set_global_sleep(sleep_function p_sleep_function)
{
  global_clocks::m_global_sleep = p_sleep_function;
}
/**
 * @brief Set the global uptime object
 *
 * @param p_uptime_function the function to return the current system uptime
 */
static void set_global_uptime(uptime_function p_uptime_function)
{
  global_clocks::m_global_uptime = p_uptime_function;
}
}  // namespace embed::this_thread