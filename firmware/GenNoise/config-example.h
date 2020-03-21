/**
 * Config parameters for Gen. Noise.
 *
 * Edit this file and rename it as config.h before building the firmware
 */

// WiFi SSID of the network to connect to
const char* ssid = "my_awesome_ssid";
// WiFi password
const char* password = "very_smart_password";

/**
 * This stuff is better left as it is, unless there's a special reason to change
 * any value.
 */
// number of samples to consider for the average
const int N_AVERAGE = 1024;
// initial value for the average
const double INITIAL_AVERAGE = 100.0;
// number of random bits to output
const int NUM_BITS = 8;

/**
 * Pin assignments
 */
const int led = LED_BUILTIN;
