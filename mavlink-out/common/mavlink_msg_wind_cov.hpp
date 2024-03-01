// MESSAGE WIND_COV support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief WIND_COV message
 *
 * Wind estimate from vehicle. Note that despite the name, this message does not actually contain any covariances but instead variability and accuracy fields in terms of standard deviation (1-STD).
 */
struct WIND_COV : mavlink::Message {
    static constexpr msgid_t MSG_ID = 231;
    static constexpr size_t LENGTH = 40;
    static constexpr size_t MIN_LENGTH = 40;
    static constexpr uint8_t CRC_EXTRA = 105;
    static constexpr auto NAME = "WIND_COV";


    uint64_t time_usec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number. */
    float wind_x; /*< [m/s] Wind in North (NED) direction (NAN if unknown) */
    float wind_y; /*< [m/s] Wind in East (NED) direction (NAN if unknown) */
    float wind_z; /*< [m/s] Wind in down (NED) direction (NAN if unknown) */
    float var_horiz; /*< [m/s] Variability of wind in XY, 1-STD estimated from a 1 Hz lowpassed wind estimate (NAN if unknown) */
    float var_vert; /*< [m/s] Variability of wind in Z, 1-STD estimated from a 1 Hz lowpassed wind estimate (NAN if unknown) */
    float wind_alt; /*< [m] Altitude (MSL) that this measurement was taken at (NAN if unknown) */
    float horiz_accuracy; /*< [m/s] Horizontal speed 1-STD accuracy (0 if unknown) */
    float vert_accuracy; /*< [m/s] Vertical speed 1-STD accuracy (0 if unknown) */


    inline std::string get_name(void) const override
    {
            return NAME;
    }

    inline Info get_message_info(void) const override
    {
            return { MSG_ID, LENGTH, MIN_LENGTH, CRC_EXTRA };
    }

    inline std::string to_yaml(void) const override
    {
        std::stringstream ss;

        ss << NAME << ":" << std::endl;
        ss << "  time_usec: " << time_usec << std::endl;
        ss << "  wind_x: " << wind_x << std::endl;
        ss << "  wind_y: " << wind_y << std::endl;
        ss << "  wind_z: " << wind_z << std::endl;
        ss << "  var_horiz: " << var_horiz << std::endl;
        ss << "  var_vert: " << var_vert << std::endl;
        ss << "  wind_alt: " << wind_alt << std::endl;
        ss << "  horiz_accuracy: " << horiz_accuracy << std::endl;
        ss << "  vert_accuracy: " << vert_accuracy << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << time_usec;                     // offset: 0
        map << wind_x;                        // offset: 8
        map << wind_y;                        // offset: 12
        map << wind_z;                        // offset: 16
        map << var_horiz;                     // offset: 20
        map << var_vert;                      // offset: 24
        map << wind_alt;                      // offset: 28
        map << horiz_accuracy;                // offset: 32
        map << vert_accuracy;                 // offset: 36
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> time_usec;                     // offset: 0
        map >> wind_x;                        // offset: 8
        map >> wind_y;                        // offset: 12
        map >> wind_z;                        // offset: 16
        map >> var_horiz;                     // offset: 20
        map >> var_vert;                      // offset: 24
        map >> wind_alt;                      // offset: 28
        map >> horiz_accuracy;                // offset: 32
        map >> vert_accuracy;                 // offset: 36
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
