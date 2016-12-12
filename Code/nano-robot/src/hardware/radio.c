#include "hardware.h"
#include "radio.h"
#include "radio_xn297.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>

#define SPI1_MOSI_PORT  GPIOB
#define SPI1_MOSI_PIN   GPIO5

#define SPI1_MISO_PORT  GPIOB
#define SPI1_MISO_PIN   GPIO4

#define SPI1_SCK_PORT   GPIOB
#define SPI1_SCK_PIN    GPIO3

#define SPI1_NSS_PORT   GPIOA
#define SPI1_NSS_PIN    GPIO15

void nrf_select() {
    gpio_clear(GPIOA, GPIO15);
}
void nrf_deselect() {
    gpio_set  (GPIOA, GPIO15);
}
void nrf_initialize();

void init_radio() {
    rcc_periph_clock_enable(RCC_SPI1);
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

    // Setup GPIOE3 pin for spi select
    gpio_mode_setup(
        GPIOA,
        GPIO_MODE_OUTPUT,
        GPIO_PUPD_NONE,
        GPIO15);
    // Start with spi communication disabled
    nrf_deselect();

    // Setup GPIO pins for AF5 for SPI1 signals
    gpio_mode_setup(
        GPIOB,
        GPIO_MODE_AF,
        GPIO_PUPD_NONE,
        GPIO3 | GPIO4 | GPIO5);
    gpio_set_af(
        GPIOB,
        GPIO_AF0,
        GPIO3 | GPIO4 | GPIO5);

    //spi initialization;
    spi_set_master_mode         (SPI1);
    spi_set_baudrate_prescaler  (SPI1, SPI_CR1_BR_FPCLK_DIV_64);
    spi_set_clock_polarity_0    (SPI1);
    spi_set_clock_phase_0       (SPI1);
    spi_set_full_duplex_mode    (SPI1);
    spi_set_unidirectional_mode (SPI1); /* bidirectional but in 3-wire */
    spi_set_data_size           (SPI1, SPI_CR2_DS_8BIT);
    spi_enable_software_slave_management(SPI1);
    spi_send_msb_first          (SPI1);
    spi_set_nss_high            (SPI1);
    //spi_enable_ss_output(SPI1);
    spi_fifo_reception_threshold_8bit(SPI1);
    SPI_I2SCFGR(SPI1) &= ~SPI_I2SCFGR_I2SMOD;
    spi_enable(SPI1);

    // Dafuk, I need to configure twice ?!
    nrf_initialize();
    nrf_initialize();
}

// Generic functions

inline
void nrf_byte(uint8_t byte) {
    spi_send8(SPI1, byte);
}
inline
uint8_t nrf_read() {
    return spi_read8(SPI1);
}

uint8_t nrf_readReg(uint8_t reg) {
    nrf_select();
    spi_send8(SPI1, R_REGISTER | reg);
    spi_read8(SPI1); // Status byte sent first
    spi_send8(SPI1, 0x00);
    uint8_t value = spi_read8(SPI1); // One-byte value
    nrf_deselect();
    delay_nop(100);
    return value;
}
void nrf_writeReg(uint8_t reg, uint8_t value) {
    nrf_select();
    spi_send8(SPI1, W_REGISTER | reg);
    spi_read8(SPI1); // Status byte sent first
    spi_send8(SPI1, value);
    spi_read8(SPI1); // Void MISO bytes
    nrf_deselect();
    delay_nop(100);
}
void nrf_writeRegLong(uint8_t reg, int length, const uint8_t* values) {
    nrf_select();
    spi_send8(SPI1, W_REGISTER | reg);
    spi_read8(SPI1); // Status byte sent first
    for (int i = 0; i < length; ++i) {
        spi_send8(SPI1, values[i]);
        spi_read8(SPI1); // Void MISO bytes
    }
    nrf_deselect();
    delay_nop(100);
}

void nrf_command(uint8_t command) {
    nrf_select();
    spi_send8(SPI1, command);
    spi_read8(SPI1); // Status byte sent first
    delay_nop(100);
    nrf_deselect();
}

void nrf_readPayload(int size, uint8_t *data) {
    nrf_select();
    spi_send8(SPI1, R_RX_PAYLOAD);
    spi_read8(SPI1); // Status byte sent first
    for (int i = 0; i < size; ++i) {
        spi_send8(SPI1, 0);
        data[i] = spi_read8(SPI1);
    }
    nrf_deselect();
}

// XN297 specific
const uint8_t bb_config[] = { 0x4c, 0x84, 0x6F, 0x9c, 0x20 };
const uint8_t rf_config[] = { 0xc9, 0x9a, 0x80, 0x61, 0xbb, 0xab, 0x9c };
const uint8_t dm_config[] = { 0x0b, 0xdf, 0xc4, 0xa7, 0x03 };

const uint8_t rxaddress[] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };

// Dafuk, i need to initialize twice ?

void nrf_initialize() {
    // XN297-specific initialization :
    // * BB_CAL (baseband)
    nrf_writeRegLong(BB_CAL,        5, bb_config);
    // * RF_CAL
    nrf_writeRegLong(RF_CAL,        7, rf_config);
    // * DEMOD_CAL
    nrf_writeRegLong(DEMOD_CAL,     5, dm_config);

    // RX adress P0
    nrf_writeRegLong(RX_ADDR_P0,    5, rxaddress);

    nrf_command (FLUSH_RX);
    nrf_command (FLUSH_TX);

    nrf_writeReg(STATUS,    0b01110000); // Clear status flags
    nrf_writeReg(EN_AA,     0b00000000); // No auto ack
    nrf_writeReg(SETUP_AW,  0b00000011); // Address size (5 bits)
    nrf_setChann(                    2); // Initial channel
    nrf_writeReg(RF_CH,     0b00000010); // Write byte RF_channel
    nrf_writeReg(EN_RXADDR, 0b00000001); // Pipe 0 only
    nrf_writeReg(SETUP_RETR,0b00000000); // No retransmissions (redundant ?)
    // nrf_writeReg(RF_SETUP,  0b00000001); // lna high current on (better performance)
    nrf_writeReg(RF_SETUP,  0b00000111); // lna high current on (better performance)
    nrf_writeReg(RX_PW_P0,        0x0F); // Payload size : 15 bytes


    // XN297 : ACTIVATE + 0x73
    nrf_select();
        nrf_byte(ACTIVATE);
            nrf_read();
        nrf_byte(0x73);
            nrf_read();
    nrf_deselect();

    nrf_receive_data(15, 0);

    // Write byte config (Power up, CRC 1B enabled, PRIM_RX)
    nrf_writeReg(CONFIG,    0b00001111);
}

void nrf_setChann(int chan) {
    nrf_writeReg(RF_CH, chan);
}

bool nrf_receive_data(int size_max, uint8_t *data_buffer) {
    // TODO better detection ?
    if (nrf_readReg(STATUS) != 0b00001110) {
        nrf_readPayload(size_max, data_buffer);

        // TODO check if other flag resets needed
        // Reset status flags
        nrf_writeReg(STATUS,    0b01000000);
        return true;
    } else
        return false;
}
