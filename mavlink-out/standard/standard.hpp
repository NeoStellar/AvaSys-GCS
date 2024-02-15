/** @file
 *	@brief MAVLink comm protocol generated from standard.xml
 *	@see http://mavlink.org
 */

#pragma once

#include <array>
#include <cstdint>
#include <sstream>

#ifndef MAVLINK_STX
#define MAVLINK_STX 253
#endif

#include "../message.hpp"

namespace mavlink {
namespace standard {

/**
 * Array of msg_entry needed for @p mavlink_parse_char() (through @p mavlink_get_msg_entry())
 */
constexpr std::array<mavlink_msg_entry_t, 2> MESSAGE_ENTRIES {{ {0, 50, 9, 9, 0, 0, 0}, {300, 217, 22, 22, 0, 0, 0} }};

//! MAVLINK VERSION
constexpr auto MAVLINK_VERSION = 2;


// ENUM DEFINITIONS




} // namespace standard
} // namespace mavlink

// MESSAGE DEFINITIONS


// base include
#include "../minimal/minimal.hpp"
