#pragma once
#include <array>
namespace shin {
inline constexpr std::array<const char*,39> gate_frames={"ev3_11.app","ev3_12.app","ev3_13.app","ev3_14.app","ev3_15.app","ev3_16.app","ev3_17.app","ev3_25.app","ev3_26.app","ev3_27.app","ev3_28.app","ev3_31.app","ev3_32.app","ev3_33.app","ev3_34.app","ev3_41.app","ev3_42.app","ev3_43.app","ev3_44.app","ev3_45.app","ev3_46.app","ev3_47.app","ev3_48.app","ev3_51.app","ev3_52.app","ev3_53.app","ev3_54.app","ev3_55.app","ev3_56.app","ev3_57.app","ev3_58.app","ev3_59.app","ev3_61.app","ev3_62.app","ev3_625.app","ev3_631.app","ev3_64.app","ev3_65.app","ev3_66.app"};
inline constexpr std::array<const char*,4> gate_recall_m={"rc_m1.app","rc_m2.app","rc_m3.app","rc_m4.app"};
inline constexpr std::array<const char*,5> gate_recall_s={"rc_s1.app","rc_s2.app","rc_s3.app","rc_s4.app","rc_s5.app"};
inline constexpr std::array<unsigned,51> gate_indices={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,16,17,18,19,20,21,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38};
inline constexpr std::array<unsigned,51> gate_durations={150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,150,688,688,688,688,3920,3920,3920,3155,3155,419,5000,1350,3175,3175};
inline constexpr std::array<unsigned,6> gate_m_sequence={0,1,2,3,2,0};
inline constexpr std::array<unsigned,6> gate_s_sequence={0,1,2,3,4,0};
}
