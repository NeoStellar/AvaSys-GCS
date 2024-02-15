// MESSAGE MISSION_CURRENT support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief MISSION_CURRENT message
 *
 * 
        Message that announces the sequence number of the current target mission item (that the system will fly towards/execute when the mission is running).
        This message should be streamed all the time (nominally at 1Hz).
        This message should be emitted following a call to MAV_CMD_DO_SET_MISSION_CURRENT or SET_MISSION_CURRENT.
      
 */
struct MISSION_CURRENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 42;
    static constexpr size_t LENGTH = 18;
    static constexpr size_t MIN_LENGTH = 2;
    static constexpr uint8_t CRC_EXTRA = 28;
    static constexpr auto NAME = "MISSION_CURRENT";


    uint16_t seq; /*<  Sequence */
    uint16_t total; /*<  Total number of mission items on vehicle (on last item, sequence == total). If the autopilot stores its home location as part of the mission this will be excluded from the total. 0: Not supported, UINT16_MAX if no mission is present on the vehicle. */
    uint8_t mission_state; /*<  Mission state machine state. MISSION_STATE_UNKNOWN if state reporting not supported. */
    uint8_t mission_mode; /*<  Vehicle is in a mode that can execute mission items or suspended. 0: Unknown, 1: In mission mode, 2: Suspended (not in mission mode). */
    uint32_t mission_id; /*<  Id of current on-vehicle mission plan, or 0 if IDs are not supported or there is no mission loaded. GCS can use this to track changes to the mission plan type. The same value is returned on mission upload (in the MISSION_ACK). */
    uint32_t fence_id; /*<  Id of current on-vehicle fence plan, or 0 if IDs are not supported or there is no fence loaded. GCS can use this to track changes to the fence plan type. The same value is returned on fence upload (in the MISSION_ACK). */
    uint32_t rally_points_id; /*<  Id of current on-vehicle rally point plan, or 0 if IDs are not supported or there are no rally points loaded. GCS can use this to track changes to the rally point plan type. The same value is returned on rally point upload (in the MISSION_ACK). */


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
        ss << "  seq: " << seq << std::endl;
        ss << "  total: " << total << std::endl;
        ss << "  mission_state: " << +mission_state << std::endl;
        ss << "  mission_mode: " << +mission_mode << std::endl;
        ss << "  mission_id: " << mission_id << std::endl;
        ss << "  fence_id: " << fence_id << std::endl;
        ss << "  rally_points_id: " << rally_points_id << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << seq;                           // offset: 0
        map << total;                         // offset: 2
        map << mission_state;                 // offset: 4
        map << mission_mode;                  // offset: 5
        map << mission_id;                    // offset: 6
        map << fence_id;                      // offset: 10
        map << rally_points_id;               // offset: 14
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> seq;                           // offset: 0
        map >> total;                         // offset: 2
        map >> mission_state;                 // offset: 4
        map >> mission_mode;                  // offset: 5
        map >> mission_id;                    // offset: 6
        map >> fence_id;                      // offset: 10
        map >> rally_points_id;               // offset: 14
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
