#pragma once

#include <functional>

#include "../error.hpp"
#include "../input_pin/pin_resistors.hpp"

namespace hal {
/**
 * @addtogroup interrupt_pin Interrupt Pin
 * Available digital interrupt pin APIs
 * @{
 */
/**
 * @brief Digital interrupt pin hardware abstraction
 *
 * Use this to automatically call a function when a pin's state has
 * transitioned.
 *
 * The transition states are:
 *
 *   - falling edge: the pin reads a transitions from HIGH to LOW
 *   - rising edge: the pin reads a transitions from LOW to HIGH
 *   - both: the pin reads any state change
 *
 */
class interrupt_pin
{
public:
  /// The condition in which an interrupt is triggered.
  enum class trigger_edge
  {
    /// Trigger and interrupt when a pin transitions from HIGH voltage to
    /// LOW voltage.
    falling = 0,
    /// Trigger and interrupt when a pin transitions from LOW voltage to
    /// HIGH voltage.
    rising = 1,
    /// Trigger and interrupt when a pin transitions it state
    both = 2,
  };

  /// Generic settings for interrupt pins
  struct settings
  {
    /// Pull resistor for an interrupt pin.
    ///
    /// In general, it is highly advised to either set the pull resistor to
    /// something other than "none" or to attach an external pull up resistor to
    /// the interrupt pin in order to prevent random interrupt from firing.
    pin_resistor resistor = pin_resistor::pull_up;

    /// The trigger condition that will signal the system to run the callback.
    trigger_edge trigger = trigger_edge::rising;

    /**
     * @brief Default operators for <, <=, >, >= and ==
     *
     * @return auto - result of the comparison
     */
    [[nodiscard]] constexpr auto operator<=>(const settings&) const noexcept =
      default;
  };

  /// Interrupt pin handler
  ///
  /// @param true - state of the pin when the interrupt was triggered was HIGH
  /// @param false - state of the pin when the interrupt was triggered was LOW
  using handler = void(bool p_state);

  /**
   * @brief Configure the interrupt pin to match the settings supplied
   *
   * @param p_settings - settings to apply to interrupt pin
   * @return status - success or failure
   * @throws std::errc::invalid_argument if the settings could not be achieved.
   */
  [[nodiscard]] status configure(const settings& p_settings) noexcept
  {
    return driver_configure(p_settings);
  }

  /**
   * @brief Set the callback for when the interrupt occurs
   *
   * Any state transitions before this function is called are lost.
   *
   * @param p_callback function to execute when the trigger condition is met.
   * Set to nullptr to disable this interrupt.
   */
  void on_trigger(std::function<handler> p_callback) noexcept
  {
    return driver_on_trigger(p_callback);
  }

private:
  virtual status driver_configure(const settings& p_settings) noexcept = 0;
  virtual void driver_on_trigger(
    std::function<handler> p_callback) noexcept = 0;
};
/** @} */
}  // namespace hal
