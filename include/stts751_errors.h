#ifndef STTS751_ERRORS_H_
#define STTS751_ERRORS_H_

#include <esp_log.h>

#include <sdkconfig.h>

#ifndef CONFIG_STTS751_MISC_ERROR_BASE
    #define CONFIG_STTS751_MISC_ERROR_BASE                      0x7000
#endif

typedef int STTS751_Error_t;

/** @brief      Generic error check macro.
 *  @param Func Function that should be checked
 */
#define STTS751_ERROR_CHECK(Func)                               do                                                                                                              \
                                                                {                                                                                                               \
                                                                    STTS751_Error_t Error = Func;                                                                              \
                                                                    if(Error != STTS751_ERR_OK)                                                                                \
                                                                    {                                                                                                           \
                                                                        ESP_LOGE("STTS751", "Error check failed in (%s) at line (%u): 0x%X!", __FUNCTION__, __LINE__, Error);  \
                                                                        return Error;                                                                                           \
                                                                    }                                                                                                           \
                                                                } while(0);

/** @brief STTS751 error base.
 */
#define STTS751_BASE                                            CONFIG_STTS751_MISC_ERROR_BASE

/** @brief Everything okay.
 */
#define STTS751_ERR_OK                                          (STTS751_BASE + 0)

/** @brief Invalid function parameter.
 */
#define STTS751_ERR_INVALID_ARG                                 (STTS751_BASE + 1)

/** @brief Invalid response from sensor.
 */
#define STTS751_ERR_INVALID_RESPONSE                            (STTS751_BASE + 2)

/** @brief No device found.
 */
#define STTS751_ERR_NO_DEVICE                                   (STTS751_BASE + 3)

/** @brief Device is not ready.
 */
#define STTS751_ERR_INVALID_STATE                               (STTS751_BASE + 4)

/** @brief Device not initialized.
 */
#define STTS751_ERR_NOT_INITIALIZED                             (STTS751_BASE + 5)

#endif /* STTS751_ERRORS_H_ */