// MESSAGE MISSION_ACK support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief MISSION_ACK message
 *
 * Acknowledgment message during waypoint handling. The type field states if this message is a positive ack (type=0) or if an error happened (type=non-zero).
 */
struct MISSION_ACK : mavlink::Message {
    static constexpr msgid_t MSG_ID = 47;
    static constexpr size_t LENGTH = 8;
    static constexpr size_t MIN_LENGTH = 3;
    static constexpr uint8_t CRC_EXTRA = 153;
    static constexpr auto NAME = "MISSION_ACK";


    uint8_t target_system; /*<  System ID */
    uint8_t target_component; /*<  Component ID */
    uint8_t type; /*<  Mission result. */
    uint8_t mission_type; /*<  Mission type. */
    uint32_t opaque_id; /*<  Id of new on-vehicle mission, fence, or rally point plan (on upload to vehicle).
        The id is calculated and returned by a vehicle when a new plan is uploaded by a GCS.
        The only requirement on the id is that it must change when there is any change to the on-vehicle plan type (there is no requirement that the id be globally unique).
        0 on download from the vehicle to the GCS (on download the ID is set in MISSION_COUNT).
        0 if plan ids are not supported.
        The current on-vehicle plan ids are streamed in `MISSION_CURRENT`, allowing a GCS to determine if any part of the plan has changed and needs to be re-uploaded.
       */


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
        ss << "  target_system: " << +target_system << std::endl;
        ss << "  target_component: " << +target_component << std::endl;
        ss << "  type: " << +type << std::endl;
        ss << "  mission_type: " << +mission_type << std::endl;
        ss << "  opaque_id: " << opaque_id << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << target_system;                 // offset: 0
        map << target_component;              // offset: 1
        map << type;                          // offset: 2
        map << mission_type;                  // offset: 3
        map << opaque_id;                     // offset: 4
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> target_system;                 // offset: 0
        map >> target_component;              // offset: 1
        map >> type;                          // offset: 2
        map >> mission_type;                  // offset: 3
        map >> opaque_id;                     // offset: 4
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
