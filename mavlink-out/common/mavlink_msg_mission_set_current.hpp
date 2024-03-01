// MESSAGE MISSION_SET_CURRENT support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief MISSION_SET_CURRENT message
 *
 * 
        Set the mission item with sequence number seq as the current item and emit MISSION_CURRENT (whether or not the mission number changed).
        If a mission is currently being executed, the system will continue to this new mission item on the shortest path, skipping any intermediate mission items.
        Note that mission jump repeat counters are not reset (see MAV_CMD_DO_JUMP param2).

        This message may trigger a mission state-machine change on some systems: for example from MISSION_STATE_NOT_STARTED or MISSION_STATE_PAUSED to MISSION_STATE_ACTIVE.
        If the system is in mission mode, on those systems this command might therefore start, restart or resume the mission.
        If the system is not in mission mode this message must not trigger a switch to mission mode.
      
 */
struct MISSION_SET_CURRENT : mavlink::Message {
    static constexpr msgid_t MSG_ID = 41;
    static constexpr size_t LENGTH = 4;
    static constexpr size_t MIN_LENGTH = 4;
    static constexpr uint8_t CRC_EXTRA = 28;
    static constexpr auto NAME = "MISSION_SET_CURRENT";


    uint8_t target_system; /*<  System ID */
    uint8_t target_component; /*<  Component ID */
    uint16_t seq; /*<  Sequence */


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
        ss << "  seq: " << seq << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << seq;                           // offset: 0
        map << target_system;                 // offset: 2
        map << target_component;              // offset: 3
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> seq;                           // offset: 0
        map >> target_system;                 // offset: 2
        map >> target_component;              // offset: 3
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
