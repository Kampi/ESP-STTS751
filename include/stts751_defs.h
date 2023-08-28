#ifndef STTS751_DEFS_H_
#define STTS751_DEFS_H_

#include <stdint.h>
#include <stdbool.h>

#define STTS751_0_PRODUCT_ID            0x00
#define STTS751_1_PRODUCT_ID            0x01

#define STTS751_MANUFACTURER_ID         0x53

/** @brief 				I2C write function prototype.
 *  @param DeviceAddr	Device address
 *  @param p_Buffer		Pointer to data buffer
 *  @param Length		Buffer length
 *  @return				0 when successful
 * 						-1 when not successful
 */
typedef int32_t (*I2C_Write_t)(uint8_t DeviceAddr, const uint8_t* p_Buffer, uint32_t Length);

/** @brief 				I2C read function prototype.
 *  @param DeviceAddr	Device address
 *  @param p_Buffer		Pointer to data buffer
 *  @param Length		Buffer length
 *  @return				0 when successful
 * 						-1 when not successful
 */
typedef int32_t (*I2C_Read_t)(uint8_t DeviceAddr, uint8_t* p_Buffer, uint32_t Length);

/** @brief          Milliseconds delay function prototype.
 *  @param Delay	Delay time
 */
typedef void (*Delay_ms_t)(uint32_t Delay);

/** @brief STTS751 measurement resolutions.
 */
typedef enum
{
    STTS751_RES_10 = 0,                     /**< 10-bit resolution (default). */
    STTS751_RES_11,                         /**< 11-bit resolution. */
    STTS751_RES_12,                         /**< 12-bit resolution. */
    STTS751_RES_9,                          /**< 9-bit resolution. */
} STTS751_Resolution_t;

/** @brief STTS751 conversion rates.
 */
typedef enum
{
    STTS751_RATE_0 = 0,                     /**< 0.0625 conversions per second. */
    STTS751_RATE_1,                         /**< 0.125 conversions per second. */
    STTS751_RATE_2,                         /**< 0.25 conversions per second. */
    STTS751_RATE_3,                         /**< 0.5 conversions per second. */
    STTS751_RATE_4,                         /**< 1 conversions per second. */
    STTS751_RATE_5,                         /**< 2 conversions per second. */
    STTS751_RATE_6,                         /**< 4 conversions per second. */
    STTS751_RATE_7,                         /**< 8 conversions per second. */
    STTS751_RATE_8,                         /**< 16 conversions per second. \n
                                                 NOTE: Only in 9, 10 or 11-bit resolution mode. */
    STTS751_RATE_9,                         /**< 32 conversions per second. \n
                                                 NOTE: Only in 9 or 10-bit resolution mode. */
} STTS751_Rate_t;

/** @brief STTS751 device object definition.
 */
typedef struct
{
    I2C_Read_t I2C_Read;                    /**< Function pointer for I2C read function. */
    I2C_Write_t I2C_Write;                  /**< Function pointer for I2C read function. */
    Delay_ms_t Delay;                       /**< Function pointer for a milliseconds delay function. */
    uint8_t DevAddr;                        /**< Device address. */
    uint8_t ProductID;                      /**< Product ID from the initialization process. */
    uint8_t ManufacturerID;                 /**< Manufacturer ID from the initialization process. */
    bool isStopped;                         /**< #true when the device has been stopped. */
    bool isInitialized;                     /**< #true when the device is initialized. */
    STTS751_Resolution_t Resolution;        /**< Device resolution settings. */
} STTS751_t;

#endif /* STTS751_DEFS_H_ */