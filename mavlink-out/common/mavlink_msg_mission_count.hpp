// MESSAGE MISSION_COUNT support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief MISSION_COUNT message
 *
 * This message is emitted as response to MISSION_REQUEST_LIST by the MAV and to initiate a write transaction. The GCS can then request the individual mission item based on the knowledge of the total number of waypoints.
 */
struct MISSION_COUNT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 44;
    static constexpr size_t LENGTH = 9;
    static constexpr size_t MIN_LENGTH = 4;
    static constexpr uint8_t CRC_EXTRA = 221;
    static constexpr auto NAME = "MISSION_COUNT";


    uint8_t target_system; /*<  System ID */
    uint8_t target_component; /*<  Component ID */
    uint16_t count; /*<  Number of mission items in the sequence */
    uint8_t mission_type; /*<  Mission type. */
    uint32_t opaque_id; /*<  Id of current on-vehicle mission, fence, or rally point plan (on download from vehicle).
        This field is used when downloading a plan from a vehicle to a GCS.
        0 on upload to the vehicle from GCS.
        0 if plan ids are not supported.
        The current on-vehicle plan ids are streamed in `MISSION_CURRENT`, allowing a GCS to determine if any part of the plan has changed and needs to be re-uploaded.
        The ids are recalculated by the vehicle when any part of the on-vehicle plan changes (when a new plan is uploaded, the vehicle returns the new id to the GCS in MISSION_ACK).
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
        ss << "  count: " << count << std::endl;
        ss << "  mission_type: " << +mission_type << std::endl;
        ss << "  opaque_id: " << opaque_id << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << count;                         // offset: 0
        map << target_system;                 // offset: 2
        map << target_component;              // offset: 3
        map << mission_type;                  // offset: 4
        map << opaque_id;                     // offset: 5
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> count;                         // offset: 0
        map >> target_system;                 // offset: 2
        map >> target_component;              // offset: 3
        map >> mission_type;                  // offset: 4
        map >> opaque_id;                     // offset: 5
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
