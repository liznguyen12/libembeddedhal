#include <boost/ut.hpp>
#include <libhal/error.hpp>

namespace hal {
boost::ut::suite error_test = []() {
  using namespace boost::ut;

  static_assert(std::is_same_v<double, config::float_type>,
                "For testing purposes this should be double. This is to show "
                "that the configuration option can be overriden from the "
                "default of 'float' to 'double'.");

  static_assert(sizeof(config::float_type) == 8,
                "For testing purposes this should be double (size 8 bytes). "
                "This is to show that the configuration option can be "
                "overriden from the default of 'float' to 'double'.");

  "[success] hal::on_error calls callback"_test = []() {
    // Setup
    auto current_call_count = config::callback_call_count;
    expect(that % current_call_count == config::callback_call_count);

    // Exercise
    // Should call the `on_error_callback` defined in the tweaks file
    (void)new_error();

    // Verify
    expect(that % current_call_count < config::callback_call_count);
  };

  "[success] hal::attempt calls handler"_test = []() {
    // Setup
    constexpr int expected = 123456789;
    int value_to_be_change = 0;

    // Exercise
    // Should call the `on_error_callback` defined in the tweaks file
    auto result =
      attempt([expected]() -> status { return new_error(expected); },
              [&value_to_be_change](int p_handler_value) -> status {
                value_to_be_change = p_handler_value;
                return {};
              });

    // Verify
    expect(that % value_to_be_change == expected);
    expect(that % true == bool{ result });
  };

  "[success] hal::attempt calls handler"_test = []() {
    // Setup
    constexpr int expected = 123456789;
    int value_to_be_change = 0;

    // Exercise
    // Should call the `on_error_callback` defined in the tweaks file

    attempt_all(
      [expected]() -> status { return new_error(); },
      [&value_to_be_change](int) {},
      [&value_to_be_change, expected]() { value_to_be_change = expected; });

    // Verify
    expect(that % value_to_be_change == expected);
  };
};
}  // namespace hal
