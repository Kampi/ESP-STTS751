#include "stts751.h"

#define STTS751_REG_TEMPERATURE_UPPER           0x00
#define STTS751_REG_STATUS                      0x01
#define STTS751_REG_TEMPERATURE_LOWER           0x02
#define STTS751_REG_CONFIGURATION               0x03
#define STTS751_REG_CONVERSION_RATE             0x04
#define STTS751_REG_LIMIT_HIGH_UPPER            0x05
#define STTS751_REG_LIMIT_HIGH_LOWER            0x06
#define STTS751_REG_LIMIT_LOW_UPPER             0x07
#define STTS751_REG_LIMIT_LOW_LOWER             0x08
#define STTS751_REG_ONE_SHOT                    0x0F
#define STTS751_REG_THERM_LIMIT                 0x20
#define STTS751_REG_THERM_HYSTERESIS            0x21
#define STTS751_REG_PRODUCT_ID                  0xFD
#define STTS751_REG_MANUFACTURER_ID             0xFE
#define STTS751_REG_REVISION_ID                 0xFF

/** @brief          Modify the content of a register.
 *  @param p_Device STTS751 device object
 *  @param Port     Register address
 *  @param Mask     Bit mask
 *  @param Level    Bit level
 *  @return         STTS751_ERR_OK when successful
 */
static STTS751_Error_t STTS751_ModifyRegister(STTS751_t& p_Device, uint8_t Address, uint8_t Mask, uint8_t Value)
{
    uint8_t Data[2] = { Address, 0x00 };
    STTS751_Error_t Error;

    if(p_Device.isInitialized == false)
    {
        return STTS751_ERR_NOT_INITIALIZED;
    }

    Error = p_Device.I2C_Write(p_Device.DevAddr, &Data[0], 1) | p_Device.I2C_Read(p_Device.DevAddr, &Data[1], 1);
    if(Error)
    {
        return STTS751_ERR_INVALID_RESPONSE;
    }

    Data[1] &= ~Mask;
    Data[1] |= Value;

    if(p_Device.I2C_Write(p_Device.DevAddr, Data, sizeof(Data)))
    {
        return STTS751_ERR_INVALID_RESPONSE;
    }

    return STTS751_ERR_OK;
}

/** @brief          Poll the status register and wait until the sensor is ready.
 *  @param p_Device STTS751 device object
 *  @return         STTS751_ERR_OK when successful
 */
static STTS751_Error_t STTS751_WaitBusy(STTS751_t& p_Device)
{
    uint8_t Buffer[2];

    if(p_Device.isInitialized == false)
    {
        return STTS751_ERR_NOT_INITIALIZED;
    }

    Buffer[0] = STTS751_REG_STATUS;
    Buffer[1] = 0;

    do
    {
        if((p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &Buffer[1], 1) != 0))
        {
            return STTS751_ERR_INVALID_RESPONSE;
        }

        p_Device.Delay(100);
    } while((Buffer[1] >> 0x07) & 0x01);

    return STTS751_ERR_OK;
}

STTS751_Error_t STTS751_Init(STTS751_t& p_Device, STTS751_Resolution_t Resolution)
{
    uint8_t Buffer[2];

    p_Device.isInitialized = false;

    if((p_Device.I2C_Read == NULL) || (p_Device.I2C_Write == NULL) || (p_Device.Delay == NULL))
    {
        return STTS751_ERR_INVALID_ARG;
    }

    Buffer[0] = STTS751_REG_MANUFACTURER_ID;
    if((p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &Buffer[1], 1) != 0))
    {
        return STTS751_ERR_INVALID_RESPONSE;
    }

    if(Buffer[1] != STTS751_MANUFACTURER_ID)
    {
        return STTS751_ERR_NO_DEVICE;
    }

    p_Device.ManufacturerID = Buffer[1];

    Buffer[0] = STTS751_REG_PRODUCT_ID;
    if((p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &Buffer[1], 1) != 0))
    {
        return STTS751_ERR_INVALID_RESPONSE;
    }

    if((Buffer[1] != STTS751_0_PRODUCT_ID) && (Buffer[1] != STTS751_1_PRODUCT_ID))
    {
        return STTS751_ERR_NO_DEVICE;
    }

    p_Device.ProductID = Buffer[1];

    Buffer[0] = STTS751_REG_CONFIGURATION;
    if((p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &Buffer[1], 1) != 0))
    {
        return STTS751_ERR_INVALID_RESPONSE;
    }

    p_Device.isStopped = static_cast<bool>((Buffer[1] >> 0x06) & 0x01);
    p_Device.isInitialized = true;

    return STTS751_SetResolution(p_Device, Resolution);
}

STTS751_Error_t STTS751_Deinit(STTS751_t& p_Device)
{
    STTS751_ERROR_CHECK(STTS751_Enable(p_Device, false));

    p_Device.isInitialized = false;

    return STTS751_ERR_OK;
}

STTS751_Error_t STTS751_SetConversionRate(STTS751_t& p_Device, STTS751_Rate_t Rate)
{
    uint8_t Buffer[2];

    if(p_Device.isInitialized == false)
    {
        return STTS751_ERR_NOT_INITIALIZED;
    }

    Buffer[0] = STTS751_REG_CONVERSION_RATE;
    Buffer[1] = Rate & 0x0F;
    if(p_Device.I2C_Write(p_Device.DevAddr, Buffer, 2) != 0)
    {
        return STTS751_ERR_INVALID_RESPONSE;
    }

    return STTS751_ERR_OK;
}

STTS751_Error_t STTS751_SetResolution(STTS751_t& p_Device, STTS751_Resolution_t Resolution)
{
    STTS751_ERROR_CHECK(STTS751_ModifyRegister(p_Device, STTS751_REG_CONFIGURATION, 0x03 << 2, (Resolution & 0x03) << 2));

    p_Device.Resolution = Resolution;

    return STTS751_ERR_OK;
}

STTS751_Error_t STTS751_SetThermalLimit(STTS751_t& p_Device, int8_t Limit, int8_t Hyst)
{
    uint8_t Buffer[4];

    if(p_Device.isInitialized == false)
    {
        return STTS751_ERR_NOT_INITIALIZED;
    }

    Buffer[0] = STTS751_REG_THERM_LIMIT;
    Buffer[1] = Limit;
    Buffer[2] = STTS751_REG_THERM_HYSTERESIS;
    Buffer[3] = Hyst;
    if((p_Device.I2C_Write(p_Device.DevAddr, Buffer, 2) != 0) || (p_Device.I2C_Write(p_Device.DevAddr, &Buffer[2], 2) != 0))
    {
        return STTS751_ERR_INVALID_RESPONSE;
    }

    return STTS751_ERR_OK;
}

STTS751_Error_t STTS751_GetConversionRate(STTS751_t& p_Device, STTS751_Rate_t* p_Rate)
{
    uint8_t Buffer[2];

    if(p_Rate == NULL)
    {
        return STTS751_ERR_INVALID_ARG;
    }
    else if(p_Device.isInitialized == false)
    {
        return STTS751_ERR_NOT_INITIALIZED;
    }

    Buffer[0] = STTS751_REG_CONVERSION_RATE;
    if((p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &Buffer[1], 1) != 0))
    {
        return STTS751_ERR_INVALID_RESPONSE;
    }

    *p_Rate = static_cast<STTS751_Rate_t>(Buffer[1] & 0x0F);

    return STTS751_ERR_OK;
}

STTS751_Error_t STTS751_Enable(STTS751_t& p_Device, bool Enable)
{
    if(p_Device.isStopped == !Enable)
    {
        return STTS751_ERR_OK;
    }

    STTS751_ERROR_CHECK(STTS751_ModifyRegister(p_Device, STTS751_REG_CONFIGURATION, 0x01 << 0x06, (!Enable) << 0x06));

    p_Device.isStopped = !Enable;

    return STTS751_ERR_OK;
}

STTS751_Error_t STTS751_EnableEvent(STTS751_t& p_Device, bool Enable)
{
    return STTS751_ModifyRegister(p_Device, STTS751_REG_CONFIGURATION, 0x01 << 0x07, (!Enable) << 0x07);
}

STTS751_Error_t STTS751_OneShot(STTS751_t& p_Device, int8_t* p_Temperature, int8_t* p_Fractional)
{
    uint8_t TempLower;
    uint8_t TempHigher;
    uint8_t Buffer[2];

    if(p_Temperature == NULL)
    {
        return STTS751_ERR_INVALID_ARG;
    }
    else if(p_Device.isStopped == false)
    {
        return STTS751_ERR_INVALID_STATE;
    }

    STTS751_ERROR_CHECK(STTS751_WaitBusy(p_Device));

    Buffer[0] = STTS751_REG_ONE_SHOT;
    Buffer[1] = 0x01;
    if(p_Device.I2C_Write(p_Device.DevAddr, Buffer, 2) != 0)
    {
        return STTS751_ERR_INVALID_RESPONSE;
    }

    STTS751_ERROR_CHECK(STTS751_WaitBusy(p_Device));

    Buffer[0] = STTS751_REG_TEMPERATURE_LOWER;
    Buffer[1] = STTS751_REG_TEMPERATURE_UPPER;
    if((p_Device.I2C_Write(p_Device.DevAddr, &Buffer[1], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &TempHigher, 1) != 0) ||
       (p_Device.I2C_Write(p_Device.DevAddr, &Buffer[0], 1) != 0) || (p_Device.I2C_Read(p_Device.DevAddr, &TempLower, 1) != 0)
    )
    {
        return STTS751_ERR_INVALID_RESPONSE;
    }

    if(TempHigher & (0x01 << 7))
    {
        TempHigher = !TempHigher;
        TempHigher++;

        TempLower = !TempLower;
        TempLower++;
    }

    TempLower >>= 4;

    *p_Temperature = TempHigher;

    if(p_Fractional != NULL)
    {
        *p_Fractional = TempLower;
    }

    return STTS751_ERR_OK;
}

STTS751_Error_t STTS751_OneShot(STTS751_t& p_Device, int32_t* p_Temperature)
{
    int8_t Number;
    int8_t Frac;
    int32_t Temp;

    if(p_Temperature == NULL)
    {
        return STTS751_ERR_INVALID_ARG;
    }

    STTS751_ERROR_CHECK(STTS751_OneShot(p_Device, &Number, &Frac));

    switch(p_Device.Resolution)
    {
        case STTS751_RES_10:
        {
            Temp *= 100;

            break;
        }
        case STTS751_RES_11:
        {
            Temp *= 1000;

            break;
        }
        case STTS751_RES_12:
        {
            Temp *= 1000;

            break;
        }
        default:
        {
            Temp *= 10;

            break;
        }
    }

    // TODO: Add conversion for both numbers into a single number

    return STTS751_ERR_OK;
}