#pragma once

#include <cstdint>

#include "../config.hpp"
#include "../error.hpp"
#include "../percentage.hpp"

namespace hal {
/**
 * @addtogroup dac
 * Available Digital to Analog Converter (DAC) APIs
 * @{
 */
/**
 * @brief Digital to Analog Converter (DAC) hardware abstraction interface.
 *
 * Use this interface for devices and peripherals that can create arbitrary
 * analog voltages between a defined Vss (negative reference) and Vcc (positive
 * reference) voltage.
 *
 */
class dac
{
public:
  /**
   * @brief Set the output voltage of the DAC.
   *
   * @param p_value - percentage scale from Vss to Vcc voltage
   * @return status - success or failure
   */
  [[nodiscard]] status write(percentage p_value) noexcept
  {
    return driver_write(p_value);
  }

private:
  virtual status driver_write(percentage p_value) noexcept = 0;
};
/** @} */
}  // namespace hal
