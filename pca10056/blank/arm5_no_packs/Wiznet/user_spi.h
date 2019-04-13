#ifndef USER_SPI_H__
#define USER_SPI_H__

#include <stdbool.h>
#include <stdint.h>

/** @file
* @brief Software controlled SPI Master driver.
*
*
* @defgroup lib_driver_spi_master Software controlled SPI Master driver
* @{
* @ingroup nrf_drivers
* @brief Software controlled SPI Master driver.
*
* Supported features:
* - Operate two SPI masters independently or in parallel.
* - Transmit and/or Receive given size of data through SPI.
* - configure each SPI module separately through @ref spi_master_init.
*/

//==========================================================
// <h> SPI_CONFIGURATION - Spi configuration

//==========================================================
// <o> SPI_SCK_PIN  - Pin number
 
// <0=> 0 (P0.0) 
// <1=> 1 (P0.1) 
// <2=> 2 (P0.2) 
// <3=> 3 (P0.3) 
// <4=> 4 (P0.4) 
// <5=> 5 (P0.5) 
// <6=> 6 (P0.6) 
// <7=> 7 (P0.7) 
// <8=> 8 (P0.8) 
// <9=> 9 (P0.9) 
// <10=> 10 (P0.10) 
// <11=> 11 (P0.11) 
// <12=> 12 (P0.12) 
// <13=> 13 (P0.13) 
// <14=> 14 (P0.14) 
// <15=> 15 (P0.15) 
// <16=> 16 (P0.16) 
// <17=> 17 (P0.17) 
// <18=> 18 (P0.18) 
// <19=> 19 (P0.19) 
// <20=> 20 (P0.20) 
// <21=> 21 (P0.21) 
// <22=> 22 (P0.22) 
// <23=> 23 (P0.23) 
// <24=> 24 (P0.24) 
// <25=> 25 (P0.25) 
// <26=> 26 (P0.26) 
// <27=> 27 (P0.27) 
// <28=> 28 (P0.28) 
// <29=> 29 (P0.29) 
// <30=> 30 (P0.30) 
// <31=> 31 (P0.31) 
// <32=> 32 (P1.0) 
// <33=> 33 (P1.1) 
// <34=> 34 (P1.2) 
// <35=> 35 (P1.3) 
// <36=> 36 (P1.4) 
// <37=> 37 (P1.5) 
// <38=> 38 (P1.6) 
// <39=> 39 (P1.7) 
// <40=> 40 (P1.8) 
// <41=> 41 (P1.9) 
// <42=> 42 (P1.10) 
// <43=> 43 (P1.11) 
// <44=> 44 (P1.12) 
// <45=> 45 (P1.13) 
// <46=> 46 (P1.14) 
// <47=> 47 (P1.15) 
// <4294967295=> Not connected 

#ifndef SPI_SCK_PIN
#define SPI_SCK_PIN 47
#endif

// <o> SPI_MISO_PIN  - Pin number
 
// <0=> 0 (P0.0) 
// <1=> 1 (P0.1) 
// <2=> 2 (P0.2) 
// <3=> 3 (P0.3) 
// <4=> 4 (P0.4) 
// <5=> 5 (P0.5) 
// <6=> 6 (P0.6) 
// <7=> 7 (P0.7) 
// <8=> 8 (P0.8) 
// <9=> 9 (P0.9) 
// <10=> 10 (P0.10) 
// <11=> 11 (P0.11) 
// <12=> 12 (P0.12) 
// <13=> 13 (P0.13) 
// <14=> 14 (P0.14) 
// <15=> 15 (P0.15) 
// <16=> 16 (P0.16) 
// <17=> 17 (P0.17) 
// <18=> 18 (P0.18) 
// <19=> 19 (P0.19) 
// <20=> 20 (P0.20) 
// <21=> 21 (P0.21) 
// <22=> 22 (P0.22) 
// <23=> 23 (P0.23) 
// <24=> 24 (P0.24) 
// <25=> 25 (P0.25) 
// <26=> 26 (P0.26) 
// <27=> 27 (P0.27) 
// <28=> 28 (P0.28) 
// <29=> 29 (P0.29) 
// <30=> 30 (P0.30) 
// <31=> 31 (P0.31) 
// <32=> 32 (P1.0) 
// <33=> 33 (P1.1) 
// <34=> 34 (P1.2) 
// <35=> 35 (P1.3) 
// <36=> 36 (P1.4) 
// <37=> 37 (P1.5) 
// <38=> 38 (P1.6) 
// <39=> 39 (P1.7) 
// <40=> 40 (P1.8) 
// <41=> 41 (P1.9) 
// <42=> 42 (P1.10) 
// <43=> 43 (P1.11) 
// <44=> 44 (P1.12) 
// <45=> 45 (P1.13) 
// <46=> 46 (P1.14) 
// <47=> 47 (P1.15) 
// <4294967295=> Not connected 

#ifndef SPI_MISO_PIN
#define SPI_MISO_PIN 46
#endif

// <o> SPI_MOSI_PIN  - Pin number
 
// <0=> 0 (P0.0) 
// <1=> 1 (P0.1) 
// <2=> 2 (P0.2) 
// <3=> 3 (P0.3) 
// <4=> 4 (P0.4) 
// <5=> 5 (P0.5) 
// <6=> 6 (P0.6) 
// <7=> 7 (P0.7) 
// <8=> 8 (P0.8) 
// <9=> 9 (P0.9) 
// <10=> 10 (P0.10) 
// <11=> 11 (P0.11) 
// <12=> 12 (P0.12) 
// <13=> 13 (P0.13) 
// <14=> 14 (P0.14) 
// <15=> 15 (P0.15) 
// <16=> 16 (P0.16) 
// <17=> 17 (P0.17) 
// <18=> 18 (P0.18) 
// <19=> 19 (P0.19) 
// <20=> 20 (P0.20) 
// <21=> 21 (P0.21) 
// <22=> 22 (P0.22) 
// <23=> 23 (P0.23) 
// <24=> 24 (P0.24) 
// <25=> 25 (P0.25) 
// <26=> 26 (P0.26) 
// <27=> 27 (P0.27) 
// <28=> 28 (P0.28) 
// <29=> 29 (P0.29) 
// <30=> 30 (P0.30) 
// <31=> 31 (P0.31) 
// <32=> 32 (P1.0) 
// <33=> 33 (P1.1) 
// <34=> 34 (P1.2) 
// <35=> 35 (P1.3) 
// <36=> 36 (P1.4) 
// <37=> 37 (P1.5) 
// <38=> 38 (P1.6) 
// <39=> 39 (P1.7) 
// <40=> 40 (P1.8) 
// <41=> 41 (P1.9) 
// <42=> 42 (P1.10) 
// <43=> 43 (P1.11) 
// <44=> 44 (P1.12) 
// <45=> 45 (P1.13) 
// <46=> 46 (P1.14) 
// <47=> 47 (P1.15) 
// <4294967295=> Not connected 

#ifndef SPI_MOSI_PIN
#define SPI_MOSI_PIN 45   // According to top
#endif
#ifndef R_PIN
#define R_PIN 18   // According to top
#endif



// <o> SPI_SS_PIN  - Pin number
 
// <0=> 0 (P0.0) 
// <1=> 1 (P0.1) 
// <2=> 2 (P0.2) 
// <3=> 3 (P0.3) 
// <4=> 4 (P0.4) 
// <5=> 5 (P0.5) 
// <6=> 6 (P0.6) 
// <7=> 7 (P0.7) 
// <8=> 8 (P0.8) 
// <9=> 9 (P0.9) 
// <10=> 10 (P0.10) 
// <11=> 11 (P0.11) 
// <12=> 12 (P0.12) 
// <13=> 13 (P0.13) 
// <14=> 14 (P0.14) 
// <15=> 15 (P0.15) 
// <16=> 16 (P0.16) 
// <17=> 17 (P0.17) 
// <18=> 18 (P0.18) 
// <19=> 19 (P0.19) 
// <20=> 20 (P0.20) 
// <21=> 21 (P0.21) 
// <22=> 22 (P0.22) 
// <23=> 23 (P0.23) 
// <24=> 24 (P0.24) 
// <25=> 25 (P0.25) 
// <26=> 26 (P0.26) 
// <27=> 27 (P0.27) 
// <28=> 28 (P0.28) 
// <29=> 29 (P0.29) 
// <30=> 30 (P0.30) 
// <31=> 31 (P0.31) 
// <32=> 32 (P1.0) 
// <33=> 33 (P1.1) 
// <34=> 34 (P1.2) 
// <35=> 35 (P1.3) 
// <36=> 36 (P1.4) 
// <37=> 37 (P1.5) 
// <38=> 38 (P1.6) 
// <39=> 39 (P1.7) 
// <40=> 40 (P1.8) 
// <41=> 41 (P1.9) 
// <42=> 42 (P1.10) 
// <43=> 43 (P1.11) 
// <44=> 44 (P1.12) 
// <45=> 45 (P1.13) 
// <46=> 46 (P1.14) 
// <47=> 47 (P1.15) 
// <4294967295=> Not connected 

#ifndef SPI_SS_PIN
#define SPI_SS_PIN 44
#endif


#define TIMEOUT_COUNTER          0x3000UL  /*!< timeout for getting rx bytes from slave */

/**
 *  SPI master operating frequency
 */
typedef enum
{
    SPI_FREQ_125KBPS = 0x02,           /*!< drive SClk with frequency 125Kbps */
    SPI_FREQ_250KBPS = 0x04,           /*!< drive SClk with frequency 250Kbps */
    SPI_FREQ_500KBPS = 0x08,           /*!< drive SClk with frequency 500Kbps */
    SPI_FREQ_1MBPS = 0x10,             /*!< drive SClk with frequency 1Mbps */
    SPI_FREQ_2MBPS = 0x20,             /*!< drive SClk with frequency 2Mbps */
    SPI_FREQ_4MBPS = 0x40,             /*!< drive SClk with frequency 4Mbps */
    SPI_FREQ_8MBPS = 0x80              /*!< drive SClk with frequency 8Mbps */
} SPIFrequency_t;

/**
 *  SPI master module number
 */
typedef enum
{
    SPI0 = 0,               /*!< SPI module 0 */
    SPI1                    /*!< SPI module 1 */
} SPIModuleNumber;

/**
 *  SPI mode
 */
typedef enum
{
    //------------------------Clock polarity 0, Clock starts with level 0-------------------------------------------
    SPI_MODE0 = 0,          /*!< Sample data at rising edge of clock and shift serial data at falling edge */
    SPI_MODE1,              /*!< sample data at falling edge of clock and shift serial data at rising edge */
    //------------------------Clock polarity 1, Clock starts with level 1-------------------------------------------
    SPI_MODE2,              /*!< sample data at falling edge of clock and shift serial data at rising edge */
    SPI_MODE3               /*!< Sample data at rising edge of clock and shift serial data at falling edge */
} SPIMode_t;

/**
 *  SPI master bit ordering
 */
typedef enum
{
    SPI_BITORDER_MSB_LSB = 0, /*!< Most significant to least significant bit */
    SPI_BITORDER_LSB_MSB      /*!< Least significant to most significant bit */
} SPIBitOrder_t;

/**
 *  Struct containing all parameters necessary to configure the SPI interface
 */
typedef struct
{
    union
    {
        uint8_t SPI_Cfg;            /*!< Bit mode and bit order merged, as in the SPI CONFIG register */
        struct
        {
            uint8_t BitOrder : 1;   /*!< SPI master bit order */
            uint8_t Mode : 2;       /*!< SPI master mode */
            uint8_t : 5;            /*!< Padding */
        }Fields;
    }Config;
    uint8_t Frequency;              /*!< SPI master frequency */
    uint8_t Pin_SCK;                /*!< SPI master SCK pin */
    uint8_t Pin_MOSI;               /*!< SPI master MOSI pin */
    uint8_t Pin_MISO;               /*!< SPI master MISO pin */
    uint8_t Pin_CSN;                /*!< SPI master chip select pin */
} SPIConfig_t;

/**
 * Initializes given SPI master with given configuration.
 *
 * After initializing the given SPI master with given configuration, this function also test if the
 * SPI slave is responding with the configurations by transmitting few test bytes. If the slave did not
 * respond then error is returned and contents of the rx_data are invalid.
 *
 * @param module_number SPI master number (SPIModuleNumber) to initialize.
 * @param pointer to a struct of type @ref SPIConfig_t containing the SPI configuration parameters.
 * @return
 * @retval pointer to direct physical address of the requested SPI module if init was successful
 * @retval 0, if either init failed or slave did not respond to the test transfer
 */

uint32_t* spi_master_init(SPIModuleNumber spi_num, SPIConfig_t *spi_config);

/**
 * Transfer/Receive data over SPI bus.
 *
 * @note Make sure at least transfer_size number of bytes is allocated in tx_data/rx_data.
 *
 * @param spi_num SPI master number (SPIModuleNumber)
 * @param transfer_size  number of bytes to transmit/receive over SPI master
 * @param tx_data pointer to the data that needs to be transmitted
 * @param rx_data pointer to the data that needs to be received
 * @return
 * @retval true if transmit/reveive of transfer_size were completed.
 * @retval false if transmit/reveive of transfer_size were not complete and tx_data/rx_data points to invalid data.
 */
bool spi_master_tx_rx(SPIModuleNumber spi_num, uint16_t transfer_size, const uint8_t *tx_data, uint8_t *rx_data);
/**
 * Transfer/Receive data over SPI bus.
 *
 * @note Make sure at least transfer_size number of bytes is allocated in tx_data.
 *
 * @param spi_num SPI master number (SPIModuleNumber)
 * @param transfer_size  number of bytes to transmit/receive over SPI master
 * @param tx_data pointer to the data that needs to be transmitted
 * @return
 * @retval true if transmit/reveive of transfer_size were completed.
 * @retval false if transmit/reveive of transfer_size were not complete and tx_data/rx_data points to invalid data.
 */
bool spi_master_tx(SPIModuleNumber spi_num, uint16_t transfer_size, const uint8_t *tx_data);
/**
 * Transfer/Receive data over SPI bus.
 *
 * @note Make sure at least transfer_size number of bytes is allocated in rx_data.
 *
 * @param spi_num SPI master number (SPIModuleNumber)
 * @param transfer_size  number of bytes to transmit/receive over SPI master
 * @param rx_data pointer to the data that needs to be received
 * @return
 * @retval true if transmit/reveive of transfer_size were completed.
 * @retval false if transmit/reveive of transfer_size were not complete and tx_data/rx_data points to invalid data.
 */
bool spi_master_rx(SPIModuleNumber spi_num, uint16_t transfer_size, uint8_t *rx_data);

void	spi0_master_init(void);


/**
 *@}
 **/

#endif
