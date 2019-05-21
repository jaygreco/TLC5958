#ifndef TLC5958_DEFS_H
#define TLC5958_DEFS_H

#define FC1_LOD_THRESH(x)   (((x) & 0x3) << 0)
#define FC1_TD0_SEL(x)      (((x) & 0x3) << 2)
#define FC1_LOD_REMOVE(x)   (((x) & 0x1) << 4)
#define FC1_PRE_CHARGE(x)   (((x) & 0x1) << 8)
#define FC1_PRE_CHG_MODE(x) (((x) & 0x1) << 9)
#define FC1_UNIFORMITY(x)   (((x) & 0x1) << 13)
#define FC1_CCB(x)          (((x) & 0x1FF) << 14)
#define FC1_CCG(x)          (((x) & 0x1FF) << 23)
#define FC1_CCR(x)          (((x) & 0x1FF) << 32)
#define FC1_BC(x)           (((x) & 0x7) << 41)

#define FC2_MAX_LINE(x)     (((x-1) & 0x1F) << 0)
#define FC2_PSAVE(x)        (((x) & 0x1) << 5)
#define FC2_GCLK_EDGE(x)    (((x) & 0x1) << 6)
#define FC2_PRECHARGE(x)    (((x) & 0x1) << 7)
#define FC2_EMI_MODE_B(x)   (((x) & 0x1) << 11)
#define FC2_EMI_MODE_G(x)   (((x) & 0x1) << 12)
#define FC2_EMI_MODE_R(x)   (((x) & 0x1) << 13)
#define FC2_SEL_PWM(x)      (((x) & 0x1) << 14)
#define FC2_LINE1(x)        (((x) & 0x3) << 15)
#define FC2_LGSE1_B(x)      (((x) & 0xF) << 20)
#define FC2_LGSE1_G(x)      (((x) & 0xF) << 24)
#define FC2_LGSE1_R(x)      (((x) & 0xF) << 28)
#define FC2_INF_B(x)        (((x) & 0x3) << 32)
#define FC2_REV_V_B(x)      (((x) & 0x3) << 34)
#define FC2_INF_G(x)        (((x) & 0x3) << 36)
#define FC2_REV_V_G(x)      (((x) & 0x3) << 38)
#define FC2_INF_R(x)        (((x) & 0x3) << 40)
#define FC2_REV_V_R(x)      (((x) & 0x3) << 42)

#define FCx_ADDR(x)         (((x) & 0xF) << 44)
#define ADDR_FC1                            0x9LL
#define ADDR_FC2                            0x6LL

#define FCx_DATA_BITS                       48
#define GS_DATA_BITS                        48
#define GS_LAT_CLKS                         1
#define FCx_LAT_CLKS                        5
#define VSYNC_BITS                          3
#define VSYNC_LAT_CLKS                      3
#define WRTEN_BITS                          15
#define WRTEN_LAT_CLKS                      15
#define DATA_NONE                           0LL

//FC1_LOD_THRESH(x)
#define LOD_THRESH_0V2                      0
#define LOD_THRESH_0V4                      1
#define LOD_THRESH_0V6                      3
#define LOD_THRESH_0V8                      3
//FC1_TD0_SEL(x)
#define TD0_SEL_15NS                        0
#define TD0_SEL_25NS                        1
#define TD0_SEL_35NS                        2
#define TD0_SEL_10NS                        3
//FC1_LOD_REMOVE(x)
#define LOD_REMOVE_DIS                      0
#define LOD_REMOVE_EN                       1
//FC1_PRE_CHARGE(x)
#define PRE_CHARGE_EN                       0
#define PRE_CHARGE_DIS                      1
//FC1_PRE_CHG_MODE(x)
#define PRE_CHG_MODE_DEFAULT                0
#define PRE_CHG_MODE_MODE3                  1
//FC1_UNIFORMITY(x)
#define UNIF_ENCHANCE_EN                    0
#define UNIF_ENCHANCE_DIS                   0
//FC2_PSAVE(x)
#define PSAVE_DIS                           0
#define PSAVE_EN                            1
//FC2_GCLK_EDGE(x)
#define RISING_EDGE                         0
#define DUAL_EDGE                           1
//FC2_EMI_MODE_BGR(x)
#define EMI_REDUCED                         0
#define EMI_NORMAL                          1
//FC2_SEL_PWM(x)
#define PWM_MODE_8_8                        0
#define PWM_MODE_7_9                        1
//FC2_LINE1(x), FC2_INF_B(x)
#define ENCHANCE_NONE                       0LL
#define ENCHANCE_LOW                        1LL
#define ENCHANCE_MED                        2LL
#define ENCHANCE_STRONG                     3LL
//FC2_REV_V_BGR(x)
#define REV_VOLT_HIGH                       0LL
#define REV_VOLT_MED_HIGH                   1LL
#define REV_VOLT_MED                        2LL
#define REV_VOLT_LOW                        3LL

#define GS_DUAL_EDGE    1

#endif