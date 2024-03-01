// MESSAGE TIMESYNC support class

#pragma once

namespace mavlink {
namespace common {
namespace msg {

/**
 * @brief TIMESYNC message
 *
 * 
        Time synchronization message.
        The message is used for both timesync requests and responses.
        The request is sent with `ts1=syncing component timestamp` and `tc1=0`, and may be broadcast or targeted to a specific system/component.
        The response is sent with `ts1=syncing component timestamp` (mirror back unchanged), and `tc1=responding component timestamp`, with the `target_system` and `target_component` set to ids of the original request.
        Systems can determine if they are receiving a request or response based on the value of `tc`.
        If the response has `target_system==target_component==0` the remote system has not been updated to use the component IDs and cannot reliably timesync; the requestor may report an error.
        Timestamps are UNIX Epoch time or time since system boot in nanoseconds (the timestamp format can be inferred by checking for the magnitude of the number; generally it doesn't matter as only the offset is used).
        The message sequence is repeated numerous times with results being filtered/averaged to estimate the offset.
      
 */
struct TIMESYNC : mavlink::Message {
    static constexpr msgid_t MSG_ID = 111;
    static constexpr size_t LENGTH = 18;
    static constexpr size_t MIN_LENGTH = 16;
    static constexpr uint8_t CRC_EXTRA = 34;
    static constexpr auto NAME = "TIMESYNC";


    int64_t tc1; /*< [ns] Time sync timestamp 1. Syncing: 0. Responding: Timestamp of responding component. */
    int64_t ts1; /*< [ns] Time sync timestamp 2. Timestamp of syncing component (mirrored in response). */
    uint8_t target_system; /*<  Target system id. Request: 0 (broadcast) or id of specific system. Response must contain system id of the requesting component. */
    uint8_t target_component; /*<  Target component id. Request: 0 (broadcast) or id of specific component. Response must contain component id of the requesting component. */


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
        ss << "  tc1: " << tc1 << std::endl;
        ss << "  ts1: " << ts1 << std::endl;
        ss << "  target_system: " << +target_system << std::endl;
        ss << "  target_component: " << +target_component << std::endl;

        return ss.str();
    }

    inline void serialize(mavlink::MsgMap &map) const override
    {
        map.reset(MSG_ID, LENGTH);

        map << tc1;                           // offset: 0
        map << ts1;                           // offset: 8
        map << target_system;                 // offset: 16
        map << target_component;              // offset: 17
    }

    inline void deserialize(mavlink::MsgMap &map) override
    {
        map >> tc1;                           // offset: 0
        map >> ts1;                           // offset: 8
        map >> target_system;                 // offset: 16
        map >> target_component;              // offset: 17
    }
};

} // namespace msg
} // namespace common
} // namespace mavlink
