QT += quick
QT += network
QT += serialport

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LIBS += -L/home/kardasland/QtProjects/AvaSys-GCS/openssl-1.1.1g/include -lcrypto -L/home/kardasland/QtProjects/AvaSys-GCS/openssl-1.1.1g -lssl
LIBS += -ludev
#LIBS += -lgstreamer-1.0
#LIBS += -lglib-2.0
#LIBS += -lgobject-2.0

CONFIG += link_pkgconfig
CONFIG += opencv4
PKGCONFIG += opencv4
PKGCONFIG += glib-2.0
PKGCONFIG += gstreamer-1.0
PKGCONFIG += gstreamer-app-1.0
PKGCONFIG += gobject-2.0
PKGCONFIG += gstreamer-pbutils-1.0
#PKGCONFIG += glib-2.0 gobject-2.0
#PKGCONFIG += gstreamer-1.0 glib-2.0 gobject-2.0

#INCLUDEPATH += /usr/include/opencv4/opencv2

#LIBS += -L/usr/include -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_shape -lopencv_videoio -lgobject-2.0 -lgstreamer-1.0
#LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_shape -lopencv_videoio -lgobject-2.0
# Add GStreamer include path
INCLUDEPATH += /usr/include/gstreamer-1.0
#INCLUDEPATH += /usr/include/glib-2.0
#INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
INCLUDEPATH += pkg-config --cflags glib-2.0

#LIBS += -L/lib/x86_64-linux-gnu -lgobject-2.0 -lglib-2.0 -lgstreamer-1.0
# Link against GStreamer libraries
#LIBS += -lgstreamer-1.0

#LIBS +=
#LIBS += -/home/kardasland/QtProjects/AvaSys-GCS/mavlink-out

SOURCES += \
        main.cpp \
        mavlink/mavlinkudp.cpp \
        mavlink/plane.cpp \
        mavlink/planecontroller.cpp \
        serial/serialmanager.cpp \
        utils/httpclient.cpp \
        utils/mavlinkproperties.cpp \
        utils/teknofestproperties.cpp \
        utils/udpmanager.cpp \
        video/videoitem.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ui/assets/11.png \
    ui/assets/car3.png \
    ui/assets/neostellar-logo.png \
    ui/assets/padlock-unlock.png \
    ui/assets/padlock.png \
    ui/assets/plane.png \
    ui/assets/plane_selected.png \
    ui/assets/ui-current.png

HEADERS += \
    mavlink-out/checksum.h \
    mavlink-out/common/common.h \
    mavlink-out/common/common.hpp \
    mavlink-out/common/gtestsuite.hpp \
    mavlink-out/common/mavlink.h \
    mavlink-out/common/mavlink_msg_actuator_control_target.h \
    mavlink-out/common/mavlink_msg_actuator_control_target.hpp \
    mavlink-out/common/mavlink_msg_actuator_output_status.h \
    mavlink-out/common/mavlink_msg_actuator_output_status.hpp \
    mavlink-out/common/mavlink_msg_adsb_vehicle.h \
    mavlink-out/common/mavlink_msg_adsb_vehicle.hpp \
    mavlink-out/common/mavlink_msg_ais_vessel.h \
    mavlink-out/common/mavlink_msg_ais_vessel.hpp \
    mavlink-out/common/mavlink_msg_altitude.h \
    mavlink-out/common/mavlink_msg_altitude.hpp \
    mavlink-out/common/mavlink_msg_att_pos_mocap.h \
    mavlink-out/common/mavlink_msg_att_pos_mocap.hpp \
    mavlink-out/common/mavlink_msg_attitude.h \
    mavlink-out/common/mavlink_msg_attitude.hpp \
    mavlink-out/common/mavlink_msg_attitude_quaternion.h \
    mavlink-out/common/mavlink_msg_attitude_quaternion.hpp \
    mavlink-out/common/mavlink_msg_attitude_quaternion_cov.h \
    mavlink-out/common/mavlink_msg_attitude_quaternion_cov.hpp \
    mavlink-out/common/mavlink_msg_attitude_target.h \
    mavlink-out/common/mavlink_msg_attitude_target.hpp \
    mavlink-out/common/mavlink_msg_auth_key.h \
    mavlink-out/common/mavlink_msg_auth_key.hpp \
    mavlink-out/common/mavlink_msg_autopilot_state_for_gimbal_device.h \
    mavlink-out/common/mavlink_msg_autopilot_state_for_gimbal_device.hpp \
    mavlink-out/common/mavlink_msg_autopilot_version.h \
    mavlink-out/common/mavlink_msg_autopilot_version.hpp \
    mavlink-out/common/mavlink_msg_battery_status.h \
    mavlink-out/common/mavlink_msg_battery_status.hpp \
    mavlink-out/common/mavlink_msg_button_change.h \
    mavlink-out/common/mavlink_msg_button_change.hpp \
    mavlink-out/common/mavlink_msg_camera_capture_status.h \
    mavlink-out/common/mavlink_msg_camera_capture_status.hpp \
    mavlink-out/common/mavlink_msg_camera_fov_status.h \
    mavlink-out/common/mavlink_msg_camera_fov_status.hpp \
    mavlink-out/common/mavlink_msg_camera_image_captured.h \
    mavlink-out/common/mavlink_msg_camera_image_captured.hpp \
    mavlink-out/common/mavlink_msg_camera_information.h \
    mavlink-out/common/mavlink_msg_camera_information.hpp \
    mavlink-out/common/mavlink_msg_camera_settings.h \
    mavlink-out/common/mavlink_msg_camera_settings.hpp \
    mavlink-out/common/mavlink_msg_camera_tracking_geo_status.h \
    mavlink-out/common/mavlink_msg_camera_tracking_geo_status.hpp \
    mavlink-out/common/mavlink_msg_camera_tracking_image_status.h \
    mavlink-out/common/mavlink_msg_camera_tracking_image_status.hpp \
    mavlink-out/common/mavlink_msg_camera_trigger.h \
    mavlink-out/common/mavlink_msg_camera_trigger.hpp \
    mavlink-out/common/mavlink_msg_can_filter_modify.h \
    mavlink-out/common/mavlink_msg_can_filter_modify.hpp \
    mavlink-out/common/mavlink_msg_can_frame.h \
    mavlink-out/common/mavlink_msg_can_frame.hpp \
    mavlink-out/common/mavlink_msg_canfd_frame.h \
    mavlink-out/common/mavlink_msg_canfd_frame.hpp \
    mavlink-out/common/mavlink_msg_cellular_config.h \
    mavlink-out/common/mavlink_msg_cellular_config.hpp \
    mavlink-out/common/mavlink_msg_cellular_status.h \
    mavlink-out/common/mavlink_msg_cellular_status.hpp \
    mavlink-out/common/mavlink_msg_change_operator_control.h \
    mavlink-out/common/mavlink_msg_change_operator_control.hpp \
    mavlink-out/common/mavlink_msg_change_operator_control_ack.h \
    mavlink-out/common/mavlink_msg_change_operator_control_ack.hpp \
    mavlink-out/common/mavlink_msg_collision.h \
    mavlink-out/common/mavlink_msg_collision.hpp \
    mavlink-out/common/mavlink_msg_command_ack.h \
    mavlink-out/common/mavlink_msg_command_ack.hpp \
    mavlink-out/common/mavlink_msg_command_cancel.h \
    mavlink-out/common/mavlink_msg_command_cancel.hpp \
    mavlink-out/common/mavlink_msg_command_int.h \
    mavlink-out/common/mavlink_msg_command_int.hpp \
    mavlink-out/common/mavlink_msg_command_long.h \
    mavlink-out/common/mavlink_msg_command_long.hpp \
    mavlink-out/common/mavlink_msg_component_information.h \
    mavlink-out/common/mavlink_msg_component_information.hpp \
    mavlink-out/common/mavlink_msg_component_metadata.h \
    mavlink-out/common/mavlink_msg_component_metadata.hpp \
    mavlink-out/common/mavlink_msg_control_system_state.h \
    mavlink-out/common/mavlink_msg_control_system_state.hpp \
    mavlink-out/common/mavlink_msg_current_event_sequence.h \
    mavlink-out/common/mavlink_msg_current_event_sequence.hpp \
    mavlink-out/common/mavlink_msg_data_stream.h \
    mavlink-out/common/mavlink_msg_data_stream.hpp \
    mavlink-out/common/mavlink_msg_data_transmission_handshake.h \
    mavlink-out/common/mavlink_msg_data_transmission_handshake.hpp \
    mavlink-out/common/mavlink_msg_debug.h \
    mavlink-out/common/mavlink_msg_debug.hpp \
    mavlink-out/common/mavlink_msg_debug_float_array.h \
    mavlink-out/common/mavlink_msg_debug_float_array.hpp \
    mavlink-out/common/mavlink_msg_debug_vect.h \
    mavlink-out/common/mavlink_msg_debug_vect.hpp \
    mavlink-out/common/mavlink_msg_distance_sensor.h \
    mavlink-out/common/mavlink_msg_distance_sensor.hpp \
    mavlink-out/common/mavlink_msg_efi_status.h \
    mavlink-out/common/mavlink_msg_efi_status.hpp \
    mavlink-out/common/mavlink_msg_encapsulated_data.h \
    mavlink-out/common/mavlink_msg_encapsulated_data.hpp \
    mavlink-out/common/mavlink_msg_esc_info.h \
    mavlink-out/common/mavlink_msg_esc_info.hpp \
    mavlink-out/common/mavlink_msg_esc_status.h \
    mavlink-out/common/mavlink_msg_esc_status.hpp \
    mavlink-out/common/mavlink_msg_estimator_status.h \
    mavlink-out/common/mavlink_msg_estimator_status.hpp \
    mavlink-out/common/mavlink_msg_event.h \
    mavlink-out/common/mavlink_msg_event.hpp \
    mavlink-out/common/mavlink_msg_extended_sys_state.h \
    mavlink-out/common/mavlink_msg_extended_sys_state.hpp \
    mavlink-out/common/mavlink_msg_fence_status.h \
    mavlink-out/common/mavlink_msg_fence_status.hpp \
    mavlink-out/common/mavlink_msg_file_transfer_protocol.h \
    mavlink-out/common/mavlink_msg_file_transfer_protocol.hpp \
    mavlink-out/common/mavlink_msg_flight_information.h \
    mavlink-out/common/mavlink_msg_flight_information.hpp \
    mavlink-out/common/mavlink_msg_follow_target.h \
    mavlink-out/common/mavlink_msg_follow_target.hpp \
    mavlink-out/common/mavlink_msg_generator_status.h \
    mavlink-out/common/mavlink_msg_generator_status.hpp \
    mavlink-out/common/mavlink_msg_gimbal_device_attitude_status.h \
    mavlink-out/common/mavlink_msg_gimbal_device_attitude_status.hpp \
    mavlink-out/common/mavlink_msg_gimbal_device_information.h \
    mavlink-out/common/mavlink_msg_gimbal_device_information.hpp \
    mavlink-out/common/mavlink_msg_gimbal_device_set_attitude.h \
    mavlink-out/common/mavlink_msg_gimbal_device_set_attitude.hpp \
    mavlink-out/common/mavlink_msg_gimbal_manager_information.h \
    mavlink-out/common/mavlink_msg_gimbal_manager_information.hpp \
    mavlink-out/common/mavlink_msg_gimbal_manager_set_attitude.h \
    mavlink-out/common/mavlink_msg_gimbal_manager_set_attitude.hpp \
    mavlink-out/common/mavlink_msg_gimbal_manager_set_manual_control.h \
    mavlink-out/common/mavlink_msg_gimbal_manager_set_manual_control.hpp \
    mavlink-out/common/mavlink_msg_gimbal_manager_set_pitchyaw.h \
    mavlink-out/common/mavlink_msg_gimbal_manager_set_pitchyaw.hpp \
    mavlink-out/common/mavlink_msg_gimbal_manager_status.h \
    mavlink-out/common/mavlink_msg_gimbal_manager_status.hpp \
    mavlink-out/common/mavlink_msg_global_position_int.h \
    mavlink-out/common/mavlink_msg_global_position_int.hpp \
    mavlink-out/common/mavlink_msg_global_position_int_cov.h \
    mavlink-out/common/mavlink_msg_global_position_int_cov.hpp \
    mavlink-out/common/mavlink_msg_global_vision_position_estimate.h \
    mavlink-out/common/mavlink_msg_global_vision_position_estimate.hpp \
    mavlink-out/common/mavlink_msg_gps2_raw.h \
    mavlink-out/common/mavlink_msg_gps2_raw.hpp \
    mavlink-out/common/mavlink_msg_gps2_rtk.h \
    mavlink-out/common/mavlink_msg_gps2_rtk.hpp \
    mavlink-out/common/mavlink_msg_gps_global_origin.h \
    mavlink-out/common/mavlink_msg_gps_global_origin.hpp \
    mavlink-out/common/mavlink_msg_gps_inject_data.h \
    mavlink-out/common/mavlink_msg_gps_inject_data.hpp \
    mavlink-out/common/mavlink_msg_gps_input.h \
    mavlink-out/common/mavlink_msg_gps_input.hpp \
    mavlink-out/common/mavlink_msg_gps_raw_int.h \
    mavlink-out/common/mavlink_msg_gps_raw_int.hpp \
    mavlink-out/common/mavlink_msg_gps_rtcm_data.h \
    mavlink-out/common/mavlink_msg_gps_rtcm_data.hpp \
    mavlink-out/common/mavlink_msg_gps_rtk.h \
    mavlink-out/common/mavlink_msg_gps_rtk.hpp \
    mavlink-out/common/mavlink_msg_gps_status.h \
    mavlink-out/common/mavlink_msg_gps_status.hpp \
    mavlink-out/common/mavlink_msg_high_latency.h \
    mavlink-out/common/mavlink_msg_high_latency.hpp \
    mavlink-out/common/mavlink_msg_high_latency2.h \
    mavlink-out/common/mavlink_msg_high_latency2.hpp \
    mavlink-out/common/mavlink_msg_highres_imu.h \
    mavlink-out/common/mavlink_msg_highres_imu.hpp \
    mavlink-out/common/mavlink_msg_hil_actuator_controls.h \
    mavlink-out/common/mavlink_msg_hil_actuator_controls.hpp \
    mavlink-out/common/mavlink_msg_hil_controls.h \
    mavlink-out/common/mavlink_msg_hil_controls.hpp \
    mavlink-out/common/mavlink_msg_hil_gps.h \
    mavlink-out/common/mavlink_msg_hil_gps.hpp \
    mavlink-out/common/mavlink_msg_hil_optical_flow.h \
    mavlink-out/common/mavlink_msg_hil_optical_flow.hpp \
    mavlink-out/common/mavlink_msg_hil_rc_inputs_raw.h \
    mavlink-out/common/mavlink_msg_hil_rc_inputs_raw.hpp \
    mavlink-out/common/mavlink_msg_hil_sensor.h \
    mavlink-out/common/mavlink_msg_hil_sensor.hpp \
    mavlink-out/common/mavlink_msg_hil_state.h \
    mavlink-out/common/mavlink_msg_hil_state.hpp \
    mavlink-out/common/mavlink_msg_hil_state_quaternion.h \
    mavlink-out/common/mavlink_msg_hil_state_quaternion.hpp \
    mavlink-out/common/mavlink_msg_home_position.h \
    mavlink-out/common/mavlink_msg_home_position.hpp \
    mavlink-out/common/mavlink_msg_hygrometer_sensor.h \
    mavlink-out/common/mavlink_msg_hygrometer_sensor.hpp \
    mavlink-out/common/mavlink_msg_isbd_link_status.h \
    mavlink-out/common/mavlink_msg_isbd_link_status.hpp \
    mavlink-out/common/mavlink_msg_landing_target.h \
    mavlink-out/common/mavlink_msg_landing_target.hpp \
    mavlink-out/common/mavlink_msg_link_node_status.h \
    mavlink-out/common/mavlink_msg_link_node_status.hpp \
    mavlink-out/common/mavlink_msg_local_position_ned.h \
    mavlink-out/common/mavlink_msg_local_position_ned.hpp \
    mavlink-out/common/mavlink_msg_local_position_ned_cov.h \
    mavlink-out/common/mavlink_msg_local_position_ned_cov.hpp \
    mavlink-out/common/mavlink_msg_local_position_ned_system_global_offset.h \
    mavlink-out/common/mavlink_msg_local_position_ned_system_global_offset.hpp \
    mavlink-out/common/mavlink_msg_log_data.h \
    mavlink-out/common/mavlink_msg_log_data.hpp \
    mavlink-out/common/mavlink_msg_log_entry.h \
    mavlink-out/common/mavlink_msg_log_entry.hpp \
    mavlink-out/common/mavlink_msg_log_erase.h \
    mavlink-out/common/mavlink_msg_log_erase.hpp \
    mavlink-out/common/mavlink_msg_log_request_data.h \
    mavlink-out/common/mavlink_msg_log_request_data.hpp \
    mavlink-out/common/mavlink_msg_log_request_end.h \
    mavlink-out/common/mavlink_msg_log_request_end.hpp \
    mavlink-out/common/mavlink_msg_log_request_list.h \
    mavlink-out/common/mavlink_msg_log_request_list.hpp \
    mavlink-out/common/mavlink_msg_logging_ack.h \
    mavlink-out/common/mavlink_msg_logging_ack.hpp \
    mavlink-out/common/mavlink_msg_logging_data.h \
    mavlink-out/common/mavlink_msg_logging_data.hpp \
    mavlink-out/common/mavlink_msg_logging_data_acked.h \
    mavlink-out/common/mavlink_msg_logging_data_acked.hpp \
    mavlink-out/common/mavlink_msg_mag_cal_report.h \
    mavlink-out/common/mavlink_msg_mag_cal_report.hpp \
    mavlink-out/common/mavlink_msg_manual_control.h \
    mavlink-out/common/mavlink_msg_manual_control.hpp \
    mavlink-out/common/mavlink_msg_manual_setpoint.h \
    mavlink-out/common/mavlink_msg_manual_setpoint.hpp \
    mavlink-out/common/mavlink_msg_memory_vect.h \
    mavlink-out/common/mavlink_msg_memory_vect.hpp \
    mavlink-out/common/mavlink_msg_message_interval.h \
    mavlink-out/common/mavlink_msg_message_interval.hpp \
    mavlink-out/common/mavlink_msg_mission_ack.h \
    mavlink-out/common/mavlink_msg_mission_ack.hpp \
    mavlink-out/common/mavlink_msg_mission_clear_all.h \
    mavlink-out/common/mavlink_msg_mission_clear_all.hpp \
    mavlink-out/common/mavlink_msg_mission_count.h \
    mavlink-out/common/mavlink_msg_mission_count.hpp \
    mavlink-out/common/mavlink_msg_mission_current.h \
    mavlink-out/common/mavlink_msg_mission_current.hpp \
    mavlink-out/common/mavlink_msg_mission_item.h \
    mavlink-out/common/mavlink_msg_mission_item.hpp \
    mavlink-out/common/mavlink_msg_mission_item_int.h \
    mavlink-out/common/mavlink_msg_mission_item_int.hpp \
    mavlink-out/common/mavlink_msg_mission_item_reached.h \
    mavlink-out/common/mavlink_msg_mission_item_reached.hpp \
    mavlink-out/common/mavlink_msg_mission_request.h \
    mavlink-out/common/mavlink_msg_mission_request.hpp \
    mavlink-out/common/mavlink_msg_mission_request_int.h \
    mavlink-out/common/mavlink_msg_mission_request_int.hpp \
    mavlink-out/common/mavlink_msg_mission_request_list.h \
    mavlink-out/common/mavlink_msg_mission_request_list.hpp \
    mavlink-out/common/mavlink_msg_mission_request_partial_list.h \
    mavlink-out/common/mavlink_msg_mission_request_partial_list.hpp \
    mavlink-out/common/mavlink_msg_mission_set_current.h \
    mavlink-out/common/mavlink_msg_mission_set_current.hpp \
    mavlink-out/common/mavlink_msg_mission_write_partial_list.h \
    mavlink-out/common/mavlink_msg_mission_write_partial_list.hpp \
    mavlink-out/common/mavlink_msg_mount_orientation.h \
    mavlink-out/common/mavlink_msg_mount_orientation.hpp \
    mavlink-out/common/mavlink_msg_named_value_float.h \
    mavlink-out/common/mavlink_msg_named_value_float.hpp \
    mavlink-out/common/mavlink_msg_named_value_int.h \
    mavlink-out/common/mavlink_msg_named_value_int.hpp \
    mavlink-out/common/mavlink_msg_nav_controller_output.h \
    mavlink-out/common/mavlink_msg_nav_controller_output.hpp \
    mavlink-out/common/mavlink_msg_obstacle_distance.h \
    mavlink-out/common/mavlink_msg_obstacle_distance.hpp \
    mavlink-out/common/mavlink_msg_odometry.h \
    mavlink-out/common/mavlink_msg_odometry.hpp \
    mavlink-out/common/mavlink_msg_onboard_computer_status.h \
    mavlink-out/common/mavlink_msg_onboard_computer_status.hpp \
    mavlink-out/common/mavlink_msg_open_drone_id_arm_status.h \
    mavlink-out/common/mavlink_msg_open_drone_id_arm_status.hpp \
    mavlink-out/common/mavlink_msg_open_drone_id_authentication.h \
    mavlink-out/common/mavlink_msg_open_drone_id_authentication.hpp \
    mavlink-out/common/mavlink_msg_open_drone_id_basic_id.h \
    mavlink-out/common/mavlink_msg_open_drone_id_basic_id.hpp \
    mavlink-out/common/mavlink_msg_open_drone_id_location.h \
    mavlink-out/common/mavlink_msg_open_drone_id_location.hpp \
    mavlink-out/common/mavlink_msg_open_drone_id_message_pack.h \
    mavlink-out/common/mavlink_msg_open_drone_id_message_pack.hpp \
    mavlink-out/common/mavlink_msg_open_drone_id_operator_id.h \
    mavlink-out/common/mavlink_msg_open_drone_id_operator_id.hpp \
    mavlink-out/common/mavlink_msg_open_drone_id_self_id.h \
    mavlink-out/common/mavlink_msg_open_drone_id_self_id.hpp \
    mavlink-out/common/mavlink_msg_open_drone_id_system.h \
    mavlink-out/common/mavlink_msg_open_drone_id_system.hpp \
    mavlink-out/common/mavlink_msg_open_drone_id_system_update.h \
    mavlink-out/common/mavlink_msg_open_drone_id_system_update.hpp \
    mavlink-out/common/mavlink_msg_optical_flow.h \
    mavlink-out/common/mavlink_msg_optical_flow.hpp \
    mavlink-out/common/mavlink_msg_optical_flow_rad.h \
    mavlink-out/common/mavlink_msg_optical_flow_rad.hpp \
    mavlink-out/common/mavlink_msg_orbit_execution_status.h \
    mavlink-out/common/mavlink_msg_orbit_execution_status.hpp \
    mavlink-out/common/mavlink_msg_param_ext_ack.h \
    mavlink-out/common/mavlink_msg_param_ext_ack.hpp \
    mavlink-out/common/mavlink_msg_param_ext_request_list.h \
    mavlink-out/common/mavlink_msg_param_ext_request_list.hpp \
    mavlink-out/common/mavlink_msg_param_ext_request_read.h \
    mavlink-out/common/mavlink_msg_param_ext_request_read.hpp \
    mavlink-out/common/mavlink_msg_param_ext_set.h \
    mavlink-out/common/mavlink_msg_param_ext_set.hpp \
    mavlink-out/common/mavlink_msg_param_ext_value.h \
    mavlink-out/common/mavlink_msg_param_ext_value.hpp \
    mavlink-out/common/mavlink_msg_param_map_rc.h \
    mavlink-out/common/mavlink_msg_param_map_rc.hpp \
    mavlink-out/common/mavlink_msg_param_request_list.h \
    mavlink-out/common/mavlink_msg_param_request_list.hpp \
    mavlink-out/common/mavlink_msg_param_request_read.h \
    mavlink-out/common/mavlink_msg_param_request_read.hpp \
    mavlink-out/common/mavlink_msg_param_set.h \
    mavlink-out/common/mavlink_msg_param_set.hpp \
    mavlink-out/common/mavlink_msg_param_value.h \
    mavlink-out/common/mavlink_msg_param_value.hpp \
    mavlink-out/common/mavlink_msg_ping.h \
    mavlink-out/common/mavlink_msg_ping.hpp \
    mavlink-out/common/mavlink_msg_play_tune.h \
    mavlink-out/common/mavlink_msg_play_tune.hpp \
    mavlink-out/common/mavlink_msg_play_tune_v2.h \
    mavlink-out/common/mavlink_msg_play_tune_v2.hpp \
    mavlink-out/common/mavlink_msg_position_target_global_int.h \
    mavlink-out/common/mavlink_msg_position_target_global_int.hpp \
    mavlink-out/common/mavlink_msg_position_target_local_ned.h \
    mavlink-out/common/mavlink_msg_position_target_local_ned.hpp \
    mavlink-out/common/mavlink_msg_power_status.h \
    mavlink-out/common/mavlink_msg_power_status.hpp \
    mavlink-out/common/mavlink_msg_radio_status.h \
    mavlink-out/common/mavlink_msg_radio_status.hpp \
    mavlink-out/common/mavlink_msg_raw_imu.h \
    mavlink-out/common/mavlink_msg_raw_imu.hpp \
    mavlink-out/common/mavlink_msg_raw_pressure.h \
    mavlink-out/common/mavlink_msg_raw_pressure.hpp \
    mavlink-out/common/mavlink_msg_raw_rpm.h \
    mavlink-out/common/mavlink_msg_raw_rpm.hpp \
    mavlink-out/common/mavlink_msg_rc_channels.h \
    mavlink-out/common/mavlink_msg_rc_channels.hpp \
    mavlink-out/common/mavlink_msg_rc_channels_override.h \
    mavlink-out/common/mavlink_msg_rc_channels_override.hpp \
    mavlink-out/common/mavlink_msg_rc_channels_raw.h \
    mavlink-out/common/mavlink_msg_rc_channels_raw.hpp \
    mavlink-out/common/mavlink_msg_rc_channels_scaled.h \
    mavlink-out/common/mavlink_msg_rc_channels_scaled.hpp \
    mavlink-out/common/mavlink_msg_request_data_stream.h \
    mavlink-out/common/mavlink_msg_request_data_stream.hpp \
    mavlink-out/common/mavlink_msg_request_event.h \
    mavlink-out/common/mavlink_msg_request_event.hpp \
    mavlink-out/common/mavlink_msg_resource_request.h \
    mavlink-out/common/mavlink_msg_resource_request.hpp \
    mavlink-out/common/mavlink_msg_response_event_error.h \
    mavlink-out/common/mavlink_msg_response_event_error.hpp \
    mavlink-out/common/mavlink_msg_safety_allowed_area.h \
    mavlink-out/common/mavlink_msg_safety_allowed_area.hpp \
    mavlink-out/common/mavlink_msg_safety_set_allowed_area.h \
    mavlink-out/common/mavlink_msg_safety_set_allowed_area.hpp \
    mavlink-out/common/mavlink_msg_scaled_imu.h \
    mavlink-out/common/mavlink_msg_scaled_imu.hpp \
    mavlink-out/common/mavlink_msg_scaled_imu2.h \
    mavlink-out/common/mavlink_msg_scaled_imu2.hpp \
    mavlink-out/common/mavlink_msg_scaled_imu3.h \
    mavlink-out/common/mavlink_msg_scaled_imu3.hpp \
    mavlink-out/common/mavlink_msg_scaled_pressure.h \
    mavlink-out/common/mavlink_msg_scaled_pressure.hpp \
    mavlink-out/common/mavlink_msg_scaled_pressure2.h \
    mavlink-out/common/mavlink_msg_scaled_pressure2.hpp \
    mavlink-out/common/mavlink_msg_scaled_pressure3.h \
    mavlink-out/common/mavlink_msg_scaled_pressure3.hpp \
    mavlink-out/common/mavlink_msg_serial_control.h \
    mavlink-out/common/mavlink_msg_serial_control.hpp \
    mavlink-out/common/mavlink_msg_servo_output_raw.h \
    mavlink-out/common/mavlink_msg_servo_output_raw.hpp \
    mavlink-out/common/mavlink_msg_set_actuator_control_target.h \
    mavlink-out/common/mavlink_msg_set_actuator_control_target.hpp \
    mavlink-out/common/mavlink_msg_set_attitude_target.h \
    mavlink-out/common/mavlink_msg_set_attitude_target.hpp \
    mavlink-out/common/mavlink_msg_set_gps_global_origin.h \
    mavlink-out/common/mavlink_msg_set_gps_global_origin.hpp \
    mavlink-out/common/mavlink_msg_set_home_position.h \
    mavlink-out/common/mavlink_msg_set_home_position.hpp \
    mavlink-out/common/mavlink_msg_set_mode.h \
    mavlink-out/common/mavlink_msg_set_mode.hpp \
    mavlink-out/common/mavlink_msg_set_position_target_global_int.h \
    mavlink-out/common/mavlink_msg_set_position_target_global_int.hpp \
    mavlink-out/common/mavlink_msg_set_position_target_local_ned.h \
    mavlink-out/common/mavlink_msg_set_position_target_local_ned.hpp \
    mavlink-out/common/mavlink_msg_setup_signing.h \
    mavlink-out/common/mavlink_msg_setup_signing.hpp \
    mavlink-out/common/mavlink_msg_sim_state.h \
    mavlink-out/common/mavlink_msg_sim_state.hpp \
    mavlink-out/common/mavlink_msg_smart_battery_info.h \
    mavlink-out/common/mavlink_msg_smart_battery_info.hpp \
    mavlink-out/common/mavlink_msg_statustext.h \
    mavlink-out/common/mavlink_msg_statustext.hpp \
    mavlink-out/common/mavlink_msg_storage_information.h \
    mavlink-out/common/mavlink_msg_storage_information.hpp \
    mavlink-out/common/mavlink_msg_supported_tunes.h \
    mavlink-out/common/mavlink_msg_supported_tunes.hpp \
    mavlink-out/common/mavlink_msg_sys_status.h \
    mavlink-out/common/mavlink_msg_sys_status.hpp \
    mavlink-out/common/mavlink_msg_system_time.h \
    mavlink-out/common/mavlink_msg_system_time.hpp \
    mavlink-out/common/mavlink_msg_terrain_check.h \
    mavlink-out/common/mavlink_msg_terrain_check.hpp \
    mavlink-out/common/mavlink_msg_terrain_data.h \
    mavlink-out/common/mavlink_msg_terrain_data.hpp \
    mavlink-out/common/mavlink_msg_terrain_report.h \
    mavlink-out/common/mavlink_msg_terrain_report.hpp \
    mavlink-out/common/mavlink_msg_terrain_request.h \
    mavlink-out/common/mavlink_msg_terrain_request.hpp \
    mavlink-out/common/mavlink_msg_time_estimate_to_target.h \
    mavlink-out/common/mavlink_msg_time_estimate_to_target.hpp \
    mavlink-out/common/mavlink_msg_timesync.h \
    mavlink-out/common/mavlink_msg_timesync.hpp \
    mavlink-out/common/mavlink_msg_trajectory_representation_bezier.h \
    mavlink-out/common/mavlink_msg_trajectory_representation_bezier.hpp \
    mavlink-out/common/mavlink_msg_trajectory_representation_waypoints.h \
    mavlink-out/common/mavlink_msg_trajectory_representation_waypoints.hpp \
    mavlink-out/common/mavlink_msg_tunnel.h \
    mavlink-out/common/mavlink_msg_tunnel.hpp \
    mavlink-out/common/mavlink_msg_uavcan_node_info.h \
    mavlink-out/common/mavlink_msg_uavcan_node_info.hpp \
    mavlink-out/common/mavlink_msg_uavcan_node_status.h \
    mavlink-out/common/mavlink_msg_uavcan_node_status.hpp \
    mavlink-out/common/mavlink_msg_utm_global_position.h \
    mavlink-out/common/mavlink_msg_utm_global_position.hpp \
    mavlink-out/common/mavlink_msg_v2_extension.h \
    mavlink-out/common/mavlink_msg_v2_extension.hpp \
    mavlink-out/common/mavlink_msg_vfr_hud.h \
    mavlink-out/common/mavlink_msg_vfr_hud.hpp \
    mavlink-out/common/mavlink_msg_vibration.h \
    mavlink-out/common/mavlink_msg_vibration.hpp \
    mavlink-out/common/mavlink_msg_vicon_position_estimate.h \
    mavlink-out/common/mavlink_msg_vicon_position_estimate.hpp \
    mavlink-out/common/mavlink_msg_video_stream_information.h \
    mavlink-out/common/mavlink_msg_video_stream_information.hpp \
    mavlink-out/common/mavlink_msg_video_stream_status.h \
    mavlink-out/common/mavlink_msg_video_stream_status.hpp \
    mavlink-out/common/mavlink_msg_vision_position_estimate.h \
    mavlink-out/common/mavlink_msg_vision_position_estimate.hpp \
    mavlink-out/common/mavlink_msg_vision_speed_estimate.h \
    mavlink-out/common/mavlink_msg_vision_speed_estimate.hpp \
    mavlink-out/common/mavlink_msg_wheel_distance.h \
    mavlink-out/common/mavlink_msg_wheel_distance.hpp \
    mavlink-out/common/mavlink_msg_wifi_config_ap.h \
    mavlink-out/common/mavlink_msg_wifi_config_ap.hpp \
    mavlink-out/common/mavlink_msg_winch_status.h \
    mavlink-out/common/mavlink_msg_winch_status.hpp \
    mavlink-out/common/mavlink_msg_wind_cov.h \
    mavlink-out/common/mavlink_msg_wind_cov.hpp \
    mavlink-out/common/testsuite.h \
    mavlink-out/common/version.h \
    mavlink-out/cubepilot/cubepilot.h \
    mavlink-out/cubepilot/cubepilot.hpp \
    mavlink-out/cubepilot/gtestsuite.hpp \
    mavlink-out/cubepilot/mavlink.h \
    mavlink-out/cubepilot/mavlink_msg_cubepilot_firmware_update_resp.h \
    mavlink-out/cubepilot/mavlink_msg_cubepilot_firmware_update_resp.hpp \
    mavlink-out/cubepilot/mavlink_msg_cubepilot_firmware_update_start.h \
    mavlink-out/cubepilot/mavlink_msg_cubepilot_firmware_update_start.hpp \
    mavlink-out/cubepilot/mavlink_msg_cubepilot_raw_rc.h \
    mavlink-out/cubepilot/mavlink_msg_cubepilot_raw_rc.hpp \
    mavlink-out/cubepilot/mavlink_msg_herelink_telem.h \
    mavlink-out/cubepilot/mavlink_msg_herelink_telem.hpp \
    mavlink-out/cubepilot/mavlink_msg_herelink_video_stream_information.h \
    mavlink-out/cubepilot/mavlink_msg_herelink_video_stream_information.hpp \
    mavlink-out/cubepilot/testsuite.h \
    mavlink-out/cubepilot/version.h \
    mavlink-out/mavlink_conversions.h \
    mavlink-out/mavlink_get_info.h \
    mavlink-out/mavlink_helpers.h \
    mavlink-out/mavlink_sha256.h \
    mavlink-out/mavlink_types.h \
    mavlink-out/message.hpp \
    mavlink-out/minimal/gtestsuite.hpp \
    mavlink-out/minimal/mavlink.h \
    mavlink-out/minimal/mavlink_msg_heartbeat.h \
    mavlink-out/minimal/mavlink_msg_heartbeat.hpp \
    mavlink-out/minimal/mavlink_msg_protocol_version.h \
    mavlink-out/minimal/mavlink_msg_protocol_version.hpp \
    mavlink-out/minimal/minimal.h \
    mavlink-out/minimal/minimal.hpp \
    mavlink-out/minimal/testsuite.h \
    mavlink-out/minimal/version.h \
    mavlink-out/msgmap.hpp \
    mavlink-out/protocol.h \
    mavlink-out/standard/gtestsuite.hpp \
    mavlink-out/standard/mavlink.h \
    mavlink-out/standard/standard.h \
    mavlink-out/standard/standard.hpp \
    mavlink-out/standard/testsuite.h \
    mavlink-out/standard/version.h \
    mavlink/mavlinkudp.h \
    mavlink/plane.h \
    mavlink/planecontroller.h \
    serial/serialmanager.h \
    utils/httpclient.h \
    utils/mavlinkproperties.h \
    utils/teknofestproperties.h \
    utils/udpmanager.h \
    video/videoitem.h
