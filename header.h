/* Cube Defense External / Custom Libraries Core
 * https://github.com/Samuel1rada/Cube_defense
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so.
 *
 * Extra information / reasonings behind code (for those new to c++)
 *
 * I use specific int sizes for better memory management.
 * These int sizes scale as following: int8_t, int16_t, int32_t and int64_t.
 * Don't pay attention to the '_t', this is just part of the C++ type definition.
 * The number after the 'int' refers to the amount of bits it can store these
 * translate to 255 for int8_t, 65535 for int16_t, 4294967295 for int32_t and
 * 18446744073709600000 at last for int64_t. You may notice a certain pattern
 * in these numbers... they are all powers of 2. Precicely, they are all 2
 * raised to the power after the int, minus 1 (2^8 is 256, 2^16 is 65536 ect..)
 * The reason why we subtract 1, is because that number goes to the 0. Ints
 * can be of two types, signed and unsigned. Signed ints having no special
 * affix and unsigned ints being suffixed with 'u_'. So an unsigned int of 8
 * bits looks like u_int8_t. The only difference between signed and unsigned
 * ints is that unsigned ints can be negative, whereas signed ints can not be
 * negative. The only drawback to unsigned ints is that their maximum upper
 * / lower bound is effectively half of that of their signed counterparts.
 * This is because no matter if int is signed or unsigned, they still only
 * have 8, 16, 32 or 64 bits to work with. So to allocate the space for negative
 * numbers, unsiged ints 'cut' their capacity in half, and put it below the 0.
 * An example of this int8_t vs u_int8_t. The min/max of an int8_t is 0 to 255,
 * whereas the mix/max of an u_int8_t is -128 to 128 (128 is half of 256).
 * It works the same way for 16, 32 and 64 bit sized ints. Like I said above
 * I use these specific int sizes for memory management, this works because
 * if I know a variable will never be above 255 or below 0, it would be
 * useless to allocate the normal 32 bits reserved for the int type, as
 * I will never use the remaining 0.0000059605% of this variable. Which
 * makes for a lot of wasted memory space, which is already very limited
 * on a microcontroller. Now sometimes I also will use booleans for variables
 * that don't regularly meet the expectations of a boolean. This is because
 * a boolean does not actually store a special 'value' for true or false,
 * a boolean is 0 when it is false, and 1 when it is true... an integer.
 * So if you have a int that can only ever be 0 or 1 (like digital pin values)
 * don't allocate an entire 8 bits for it with an int8_t, use a boolean.
 * At last I also use quite a few #defines. These are even better for
 * memory management as they take up 0 space in memory. They are practically
 * just placeholders for a value that will never change. The compiler of
 * C++ will simply insert the value of your #defined statement wherever
 * you used it in your code. It is just a way to make your code easier
 * to read and maintain without costing any additional memory.
 *
 * Hope you had a fun read, class is dismissed
 */

#ifndef HEADER_H
#define HEADER_H
#include <Arduino.h>
#include <usb_joystick.h>

/*
 *  JOYSTICK CONFIGURATION:
 *  BUTTON 1: ROTARY ENCODER CLOCKWISE
 *  BUTTON 2: ROTARY ENCODER COUNTER-CLOCKWISE
 *  BUTTON 3: PIN 0 HIGH
 *  BUTTON 4: PIN 1 HIGH
 *  BUTTON 5: PIN 2 HIGH
 *  BUTTON 6: PIN 3 HIGH
 */

// defines for joystick button pins

#define PIN_JOYSTICK_3 0 // Connected to pin 00, toggles the joystick button 3
#define PIN_JOYSTICK_4 1 // Connected to pin 01, toggles the joystick button 4
#define PIN_JOYSTICK_5 2 // Connected to pin 02, toggles the joystick button 5
#define PIN_JOYSTICK_6 3 // Connected to pin 03, toggles the joystick button 6

// defines for rotary encoder pins & constants
#define ENCODER_PIN_A 11     // Connected to CLK on KY-040
#define ENCODER_PIN_B 12     // Connected to DT on KY-040
#define DEGREES_PER_PULSE 12 // Total amount of degrees per pulse (KY-040 has 30 pulses per full rotation, 360 / 30 = 12)
#define RESET_DEGREES 180    // Total amount of degrees the rotary encoder can rotate before it resets back to 0

// defines for the serial communication
#define GIVE_INPUT 'a'    // the byte signalling that the computer wants input
#define ISSUE_COMMAND 'b' // the byte signalling that the computer issues a command

/// @brief Sets up all the required pins for the buttons to be used as `INPUT_PULLDOWN`
void button_setup();
/// @brief Sets up all the pins required by the rotary encoder as `INPUT`
void encoder_setup();
/// @brief Check if a rotary encoder in rotating by comparing the current state of a pin to the previous one.
/// If the current pin state is a different value than the previous pin state it must mean the encoder is rotating
/// @param pin_current Current value of the A pin on the encoder
/// @param pin_previous Last known value of the A pin on the encoder
/// @return `true` if rotating, `false` if not
bool is_rotating(bool pin_current, bool pin_previous);
/// @brief Checks if a rotary encoder is rotating clockwise
/// @param pin_current current position of the A pin on the encoder
/// @return `true` if clockwise, `false` if counter-clockwise
bool clockwise(bool pin_current);
/// @brief Handles a series of actions when the encoder has rotated a set amount of degrees (amount defined in RESET_DEGREES)
/// @param degrees Local variable that counts the current rotated degrees
/// @return If the encoder rotated the require amount we reset `degrees` and return it. If the encoder does not sucessfully rotate
/// the required degrees we return `degrees` without altering it
int16_t handle_full_rotation(int16_t degrees);
/// @brief Collect data from a series of (analog) pins and print them to the serial port to be detected by other programs
void passValuesToComputer();
/// @brief Read the serial port and if the data matches certain criteria do something (defined in function)
void parseCommands();
/// @brief Read the serial port to check if a certain 'key' of data is sent and handle them accordingly
void handle_command();
/// @brief Pulse a `Joystick.button` for a certain amount of time in milliseconds
/// @param button The `Joystick.button` to pulse
/// @param millis Amount of miliseconds before turning the `button` off again
void pulse_button(int8_t button, int32_t millis);
void button_conditional(u_int8_t pin, u_int8_t button);

#endif