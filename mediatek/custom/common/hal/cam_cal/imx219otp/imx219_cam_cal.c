#include "camera_custom_cam_cal.h"//for 658x new compilier option#include "camera_custom_eeprom.h"
#include <string.h>
 
CAM_CAL_TYPE_ENUM IMX219_CAM_CALInit(void)
{
	return CAM_CAL_USED;
}

unsigned int IMX219_CAM_CALDeviceName(char* DevName)
{
       char* str= "CAM_CAL_DRV";
       strcat (DevName, str );
	return 0;
}


