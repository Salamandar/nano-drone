#pragma once

// Commands

#define R_REGISTER      0b00000000
#define W_REGISTER      0b00100000
#define R_RX_PAYLOAD    0b01100001
#define W_TX_PAYLOAD    0b10100000
#define FLUSH_TX        0b11100001
#define FLUSH_RX        0b11100010
#define REUSE_TX_PL     0b11100011

#define ACTIVATE        0b01010000
#define R_RX_PL_WID     0b10100000
#define W_ACK_PAYLOAD   0b10101000
#define W_TX_PAYLOAD_NO 0b10110000

#define NOP             0b11111111

// Registers
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17

#define DEMOD_CAL   0x19
#define DYNPD       0x1C
#define FEATURE     0x1D
#define RF_CAL      0x1E
#define BB_CAL      0x1F
