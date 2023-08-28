#ifndef STTS751_H_
#define STTS751_H_

#include <esp_err.h>

#include "stts751_defs.h"
#include "stts751_errors.h"

/** @brief              Initialize the driver.
 *  @param p_Device     STTS751 device object
 *  @param Resolution   (Optional) Measurement resolution
 *  @return             STTS751_ERR_OK when successful
 */
STTS751_Error_t STTS751_Init(STTS751_t& p_Device, STTS751_Resolution_t Resolution = STTS751_RES_10);

/** @brief          Deinitialize the driver.
 *  @param p_Device STTS751 device object
 *  @return         STTS751_ERR_OK when successful
 */
STTS751_Error_t STTS751_Deinit(STTS751_t& p_Device);

/** @brief          Set the thermal limit and the hysteresis.
 *  @param p_Device STTS751 device object
 *  @param Limit    Thermal limit
 *  @param Hyst     Thermal hysteresis
 *  @return         STTS751_ERR_OK when successful
 */
STTS751_Error_t STTS751_SetThermalLimit(STTS751_t& p_Device, int8_t Limit, int8_t Hyst);

/** @brief          Set the conversion rate.
 *  @param p_Device STTS751 device object
 *  @param Rate     Conversion rate
 *  @return         STTS751_ERR_OK when successful
 */
STTS751_Error_t STTS751_SetConversionRate(STTS751_t& p_Device, STTS751_Rate_t Rate);

/** @brief              Set the resolution.
 *  @param p_Device     STTS751 device object
 *  @param Resolution   Sensor resolution
 *  @return             STTS751_ERR_OK when successful
 */
STTS751_Error_t STTS751_SetResolution(STTS751_t& p_Device, STTS751_Resolution_t Resolution);

/** @brief          Get the conversion rate.
 *  @param p_Device STTS751 device object
 *  @param p_Rate   Pointer to conversion rate
 *  @return         STTS751_ERR_OK when successful
 */
STTS751_Error_t STTS751_GetConversionRate(STTS751_t& p_Device, STTS751_Rate_t* p_Rate);

/** @brief          Put the device in RUN or STOP mode.
 *  @param p_Device STTS751 device object
 *  @param Enable   Enable / Disable the device
 *  @return         STTS751_ERR_OK when successful
 */
STTS751_Error_t STTS751_Enable(STTS751_t& p_Device, bool Enable);

/** @brief          Enable / Disable the event output.
 *  @param p_Device STTS751 device object
 *  @param Enable   Enable / Disable the event output
 *  @return         STTS751_ERR_OK when successful
 */
STTS751_Error_t STTS751_EnableEvent(STTS751_t& p_Device, bool Enable);

/** @brief                  Perform a one shot temperature conversion.
 *  @param p_Device         STTS751 device object
 *  @param p_Temperature    Pointer to temperature data in degree
 *  @param p_Fractional     (Optional) Pointer to fractional temperature data in degree
 *  @return                 STTS751_ERR_OK when successful
 *                          STTS751_ERR_INVALID_STATE when the device is not stopped 
 */
STTS751_Error_t STTS751_OneShot(STTS751_t& p_Device, int8_t* p_Temperature, int8_t* p_Fractional = NULL);

/** @brief                  Perform a one shot temperature conversion.
 *  @param p_Device         STTS751 device object
 *  @param p_Temperature    Pointer to temperature data in millidegree
 *  @return                 STTS751_ERR_OK when successful
 *                          STTS751_ERR_INVALID_STATE when the device is not stopped 
 */
STTS751_Error_t STTS751_OneShot(STTS751_t& p_Device, int32_t* p_Temperature);

#endif /* STTS751_H_ */