#ifndef _BEAR_H_
#define _BEAR_H_

//��س��
const unsigned char gImage_battery_charge[1280] = { /* 0X10,0X10,0X00,0X20,0X00,0X14,0X01,0X1B, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X10,0XA1,0X31,0X81,0X41,0XE1,0X41,0XE1,0X39,0XC1,0X39,0XC1,
0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,
0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,
0X39,0XC1,0X41,0XE1,0X39,0XC1,0X21,0X21,0X08,0X40,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X18,0XC1,0X6B,0X65,0XB5,0X87,0XAD,0X25,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,
0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,
0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,
0X9C,0XA4,0X9C,0XA4,0XAD,0X46,0XA4,0XE7,0X42,0X23,0X08,0X40,0X00,0X00,0X00,0X00,
0X10,0X81,0X62,0XE5,0XBD,0X88,0XAD,0X48,0X6B,0X45,0X52,0X84,0X5A,0XA4,0X5A,0XC4,
0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X52,0XA4,0X52,0XA4,
0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,
0X5A,0XC4,0X52,0X64,0X7B,0XA6,0XB5,0X67,0X9C,0XA5,0X31,0XA2,0X00,0X00,0X00,0X00,
0X29,0X43,0XA4,0XE8,0XCE,0X09,0X52,0X64,0X08,0X42,0X00,0X01,0X00,0X01,0X00,0X01,
0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X00,0X00,0X00,
0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,
0X00,0X01,0X00,0X01,0X10,0X82,0X63,0X04,0XB5,0X45,0X52,0X83,0X00,0X00,0X00,0X00,
0X39,0XE3,0XBD,0XA9,0XBD,0XC9,0X21,0X21,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X10,0X81,0X10,0XA1,
0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X31,0X80,0X9C,0XA4,0X4A,0X43,0X00,0X00,0X00,0X00,
0X39,0XE2,0XBD,0XA8,0XC5,0XE9,0X29,0X62,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X10,0XA2,0X39,0XC3,0X84,0X06,0X9C,0X86,
0X31,0XA3,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X39,0XC1,0XAD,0X05,0X5A,0XC4,0X00,0X01,0X00,0X00,
0X39,0XC2,0XB5,0X68,0XC5,0XE9,0X31,0XA2,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X08,0X42,0X29,0X43,0X52,0X83,0X94,0X66,0XBD,0XE6,0XE7,0X26,0XEF,0X28,
0X83,0XE7,0X18,0XC2,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X39,0XE1,0XC6,0X05,0XB5,0X47,0X39,0XE4,0X08,0X41,
0X39,0XC2,0XB5,0X68,0XC5,0XE9,0X31,0XA2,0X00,0X00,0X00,0X00,0X00,0X20,0X21,0X02,
0X42,0X03,0X73,0X65,0XA5,0X07,0XDE,0XA6,0XFF,0XE7,0XFF,0XE6,0XFF,0XE5,0XFF,0XE6,
0XB5,0X87,0X31,0X62,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X20,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X31,0X80,0XD6,0X64,0XF7,0X88,0X8C,0X46,0X21,0X02,
0X39,0XC2,0XB5,0X68,0XC5,0XE9,0X31,0XA2,0X00,0X00,0X00,0X00,0X08,0X61,0X42,0X05,
0X8C,0X26,0XB5,0X66,0XCE,0X46,0XDE,0XC5,0XDE,0XA4,0XEF,0X25,0XFF,0XE6,0XFF,0XC5,
0XD6,0X44,0X62,0XE2,0X29,0X20,0X31,0XA1,0X41,0XE1,0X42,0X22,0X31,0X62,0X08,0X61,
0X00,0X00,0X00,0X00,0X00,0X00,0X29,0X60,0XD6,0X23,0XFF,0XE7,0XB5,0X87,0X39,0XC3,
0X39,0XC2,0XB5,0X68,0XC5,0XE9,0X31,0XA2,0X00,0X00,0X00,0X00,0X00,0X20,0X18,0XE2,
0X39,0XE2,0X42,0X01,0X39,0XC1,0X31,0X81,0X42,0X02,0XA4,0XC6,0XF7,0X47,0XFF,0XA4,
0XF7,0X24,0XDE,0X63,0XD6,0X84,0XDE,0XC5,0XCE,0X05,0XAD,0X25,0X6B,0X25,0X18,0XE2,
0X00,0X00,0X00,0X00,0X00,0X00,0X29,0X60,0XCE,0X23,0XFF,0XE7,0XB5,0X67,0X39,0XC3,
0X39,0XC2,0XB5,0X68,0XC5,0XE9,0X31,0XA2,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X5A,0XA3,0XCE,0X25,0XFF,0XE5,
0XFF,0XE5,0XFF,0XE7,0XFF,0XC8,0XCE,0X47,0X94,0X86,0X5A,0XC4,0X31,0X82,0X08,0X61,
0X00,0X00,0X00,0X00,0X00,0X00,0X31,0X80,0XD6,0X44,0XEF,0X47,0X84,0X06,0X21,0X02,
0X39,0XC2,0XB5,0X68,0XC5,0XE9,0X31,0XA2,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X39,0XC2,0XB5,0X66,0XF7,0XA8,
0XD6,0X86,0XB5,0X67,0X84,0X06,0X4A,0X44,0X18,0XE3,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X39,0XE1,0XC5,0XE5,0XAD,0X06,0X31,0XA3,0X08,0X41,
0X39,0XE2,0XBD,0XA8,0XC5,0XE9,0X29,0X62,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X29,0X42,0X83,0XE7,0XA5,0X08,
0X5A,0XC4,0X21,0X22,0X08,0X62,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X39,0XC1,0XAD,0X25,0X5A,0XC4,0X00,0X01,0X00,0X00,
0X39,0XE3,0XBD,0XA9,0XBD,0XC9,0X29,0X21,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X20,0X10,0XA1,0X18,0XC2,
0X08,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X31,0X61,0X9C,0XA4,0X4A,0X63,0X00,0X01,0X00,0X00,
0X29,0X43,0XA4,0XE8,0XCE,0X09,0X52,0X64,0X08,0X42,0X00,0X01,0X00,0X01,0X00,0X01,
0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X00,0X00,0X00,
0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,0X00,0X01,
0X00,0X01,0X00,0X01,0X18,0XC2,0X63,0X04,0XB5,0X45,0X52,0X83,0X00,0X00,0X00,0X00,
0X10,0X81,0X62,0XE5,0XBD,0X88,0XAD,0X28,0X6B,0X45,0X52,0X84,0X5A,0XA4,0X5A,0XC4,
0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XA4,0X5A,0XA4,0X52,0XA4,
0X5A,0XA4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,0X5A,0XC4,
0X5A,0XA4,0X52,0XA4,0X83,0XE6,0XBD,0X88,0X9C,0XA5,0X31,0XA1,0X00,0X00,0X00,0X00,
0X00,0X00,0X18,0XC1,0X6B,0X45,0XB5,0X67,0XAD,0X25,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,
0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,
0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,0X9C,0XA4,
0X9C,0XA4,0X9C,0XC4,0XB5,0X66,0XA5,0X07,0X42,0X23,0X08,0X40,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X10,0X81,0X31,0X82,0X39,0XE1,0X41,0XE1,0X39,0XC1,0X39,0XC1,
0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,
0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,0X39,0XC1,
0X39,0XC1,0X41,0XE1,0X39,0XC1,0X21,0X21,0X08,0X40,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

#endif  /* _BEAR_H_ */
