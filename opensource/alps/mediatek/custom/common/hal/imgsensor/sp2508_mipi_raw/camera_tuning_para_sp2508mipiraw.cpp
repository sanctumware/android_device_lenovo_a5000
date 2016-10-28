#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_sp2508mipiraw.h"
#include "camera_info_sp2508mipiraw.h"
#include "camera_custom_AEPlinetable.h"
#include "camera_custom_tsf_tbl.h"

#include <cutils/xlog.h>//gpw
const NVRAM_CAMERA_ISP_PARAM_STRUCT CAMERA_ISP_DEFAULT_VALUE =
{{
    //Version
    Version: NVRAM_CAMERA_PARA_FILE_VERSION,
    //SensorId
    SensorId: SENSOR_ID,
    ISPComm:{
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    },
    ISPPca:{
        #include INCLUDE_FILENAME_ISP_PCA_PARAM
    },
    ISPRegs:{
        #include INCLUDE_FILENAME_ISP_REGS_PARAM
        },
    ISPMfbMixer:{{
        {//00: MFB mixer for ISO 100
            0x00000000, 0x00000000
        },
        {//01: MFB mixer for ISO 200
            0x00000000, 0x00000000
        },
        {//02: MFB mixer for ISO 400
            0x00000000, 0x00000000
        },
        {//03: MFB mixer for ISO 800
            0x00000000, 0x00000000
        },
        {//04: MFB mixer for ISO 1600
            0x00000000, 0x00000000
        },
        {//05: MFB mixer for ISO 2400
            0x00000000, 0x00000000
        },
        {//06: MFB mixer for ISO 3200
            0x00000000, 0x00000000
        }
    }},
    ISPCcmPoly22:{
        57675,    // i4R_AVG
        11162,    // i4R_STD
        68650,    // i4B_AVG
        10056,    // i4B_STD
        {  // i4P00[9]
            5010000, -1870000, -582500, -965000, 4715000, -1192500, -857500, -3617500, 7032500
        },
        {  // i4P10[9]
            2844517, -4730355, 1892803, 129254, -1560288, 1437998, 616491, -562123, -51662
        },
        {  // i4P01[9]
            1923389, -3048765, 1129137, -100431, -1213965, 1318157, 102303, -1009765, 909144
        },
        {  // i4P20[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P11[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P02[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    }
}};

const NVRAM_CAMERA_3A_STRUCT CAMERA_3A_NVRAM_DEFAULT_VALUE =
{
    NVRAM_CAMERA_3A_FILE_VERSION, // u4Version
    SENSOR_ID, // SensorId

    // AE NVRAM
    {
        // rDevicesInfo
        {
            1160,    // u4MinGain, 1024 base = 1x
            9216,    // u4MaxGain, 16x
            54,    // u4MiniISOGain, ISOxx  
            64,    // u4GainStepUnit, 1x/8 
            36269,    // u4PreExpUnit 
            29,    // u4PreMaxFrameRate
            36269,    // u4VideoExpUnit  
            29,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            36269,    // u4CapExpUnit 
            29,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
            28,    // u4LensFno, Fno = 2.8
            350    // u4FocusLength_100x
        },
        // rHistConfig
        {
            2,    // u4HistHighThres
            40,    // u4HistLowThres
            2,    // u4MostBrightRatio
            1,    // u4MostDarkRatio
            160,    // u4CentralHighBound
            20,    // u4CentralLowBound
            {240, 230, 220, 210, 200},    // u4OverExpThres[AE_CCT_STRENGTH_NUM] 
            {86, 108, 128, 148, 170},    // u4HistStretchThres[AE_CCT_STRENGTH_NUM] 
            {18, 22, 26, 30, 34}    // u4BlackLightThres[AE_CCT_STRENGTH_NUM] 
        },
        // rCCTConfig
        {
            TRUE,    // bEnableBlackLight
            TRUE,    // bEnableHistStretch
            FALSE,    // bEnableAntiOverExposure
            TRUE,    // bEnableTimeLPF
            FALSE,    // bEnableCaptureThres
           TRUE ,    // bEnableVideoThres       FALSE
            FALSE,    // bEnableStrobeThres
            47,    // u4AETarget
            47,    // u4StrobeAETarget
            50,    // u4InitIndex
            4,    // u4BackLightWeight
            32,    // u4HistStretchWeight
            4,    // u4AntiOverExpWeight
            2,    // u4BlackLightStrengthIndex
            2,    // u4HistStretchStrengthIndex
            2,    // u4AntiOverExpStrengthIndex
            2,    // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8},    // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM] 
            90,    // u4InDoorEV = 9.0, 10 base 
            -10,    // i4BVOffset delta BV = value/10 
            64,    // u4PreviewFlareOffset
            64,    // u4CaptureFlareOffset
            5,    // u4CaptureFlareThres
            64,    // u4VideoFlareOffset
            5,    // u4VideoFlareThres
            32,    // u4StrobeFlareOffset
            2,    // u4StrobeFlareThres
            8,    // u4PrvMaxFlareThres
            0,    // u4PrvMinFlareThres
            8,    // u4VideoMaxFlareThres
            0,    // u4VideoMinFlareThres
            18,    // u4FlatnessThres    // 10 base for flatness condition.
            75    // u4FlatnessStrength
        }
    },
    // AWB NVRAM
    {
        // AWB calibration data
        {
            // rUnitGain (unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rGoldenGain (golden sample gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rTuningUnitGain (Tuning sample unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rD65Gain (D65 WB gain: 1.0 = 512)
            {
                731,    // i4R
                512,    // i4G
                528    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                -13,    // i4X
                -214    // i4Y
            },
            // Horizon
            {
                -243,    // i4X
                -96    // i4Y
            },
            // A
            {
                -163,    // i4X
                -125    // i4Y
            },
            // TL84
            {
                -87,    // i4X
                -134    // i4Y
            },
            // CWF
            {
                -72,    // i4X
                -198    // i4Y
            },
            // DNP
            {
                27,    // i4X
                -150    // i4Y
            },
            // D65
            {
                120,    // i4X
                -143    // i4Y
            },
            // DF
            {
                0,    // i4X
                0    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                -13,    // i4X
                -214    // i4Y
            },
            // Horizon
            {
                -243,    // i4X
                -96    // i4Y
            },
            // A
            {
                -163,    // i4X
                -125    // i4Y
            },
            // TL84
            {
                -87,    // i4X
                -134    // i4Y
            },
            // CWF
            {
                -72,    // i4X
                -198    // i4Y
            },
            // DNP
            {
                27,    // i4X
                -150    // i4Y
            },
            // D65
            {
                120,    // i4X
                -143    // i4Y
            },
            // DF
            {
                0,    // i4X
                0    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                672,    // i4R
                512,    // i4G
                696    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                625,    // i4G
                990    // i4B
            },
            // A 
            {
                512,    // i4R
                539,    // i4G
                796    // i4B
            },
            // TL84 
            {
                545,    // i4R
                512,    // i4G
                691    // i4B
            },
            // CWF 
            {
                607,    // i4R
                512,    // i4G
                738    // i4B
            },
            // DNP 
            {
                651,    // i4R
                512,    // i4G
                605    // i4B
            },
            // D65 
            {
                731,    // i4R
                512,    // i4G
                528    // i4B
            },
            // DF 
            {
                512,    // i4R
                512,    // i4G
                512    // i4B
            }
        },
        // Rotation matrix parameter
        {
            0,    // i4RotationAngle
            256,    // i4Cos
            0    // i4Sin
        },
        // Daylight locus parameter
        {
            -106,    // i4SlopeNumerator
            128    // i4SlopeDenominator
        },
        // AWB light area
        {
            // Strobe:FIXME
            {
            0,    // i4RightBound
            0,    // i4LeftBound
            0,    // i4UpperBound
            0    // i4LowerBound
            },
            // Tungsten
            {
            -125,    // i4RightBound
            -775,    // i4LeftBound
            -60,    // i4UpperBound
            -160    // i4LowerBound
            },
            // Warm fluorescent
            {
            -125,    // i4RightBound
            -775,    // i4LeftBound
            -160,    // i4UpperBound
            -280    // i4LowerBound
            },
            // Fluorescent
            {
            -23,    // i4RightBound
            -125,    // i4LeftBound
            -61,    // i4UpperBound
            -166    // i4LowerBound
            },
            // CWF
            {
            -23,    // i4RightBound
            -125,    // i4LeftBound
            -166,    // i4UpperBound
            -248    // i4LowerBound
            },
            // Daylight
            {
            145,    // i4RightBound
            -23,    // i4LeftBound
            -63,    // i4UpperBound
            -223    // i4LowerBound
            },
            // Shade
            {
            505,    // i4RightBound
            145,    // i4LeftBound
            -63,    // i4UpperBound
            -223    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            145,    // i4RightBound
            -23,    // i4LeftBound
            -63,    // i4UpperBound
            -300    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            505,    // i4RightBound
            -775,    // i4LeftBound
            0,    // i4UpperBound
            -300    // i4LowerBound
            },
            // Daylight
            {
            170,    // i4RightBound
            -23,    // i4LeftBound
            -63,    // i4UpperBound
            -223    // i4LowerBound
            },
            // Cloudy daylight
            {
            270,    // i4RightBound
            95,    // i4LeftBound
            -63,    // i4UpperBound
            -223    // i4LowerBound
            },
            // Shade
            {
            370,    // i4RightBound
            95,    // i4LeftBound
            -63,    // i4UpperBound
            -223    // i4LowerBound
            },
            // Twilight
            {
            -23,    // i4RightBound
            -183,    // i4LeftBound
            -63,    // i4UpperBound
            -223    // i4LowerBound
            },
            // Fluorescent
            {
            170,    // i4RightBound
            -187,    // i4LeftBound
            -84,    // i4UpperBound
            -248    // i4LowerBound
            },
            // Warm fluorescent
            {
            -63,    // i4RightBound
            -263,    // i4LeftBound
            -84,    // i4UpperBound
            -248    // i4LowerBound
            },
            // Incandescent
            {
            -63,    // i4RightBound
            -263,    // i4LeftBound
            -63,    // i4UpperBound
            -223    // i4LowerBound
            },
            // Gray World
            {
            5000,    // i4RightBound
            -5000,    // i4LeftBound
            5000,    // i4UpperBound
            -5000    // i4LowerBound
            }
        },
        // PWB default gain	
        {
            // Daylight
            {
            686,    // i4R
            512,    // i4G
            563    // i4B
            },
            // Cloudy daylight
            {
            796,    // i4R
            512,    // i4G
            485    // i4B
            },
            // Shade
            {
            851,    // i4R
            512,    // i4G
            454    // i4B
            },
            // Twilight
            {
            540,    // i4R
            512,    // i4G
            714    // i4B
            },
            // Fluorescent
            {
            634,    // i4R
            512,    // i4G
            648    // i4B
            },
            // Warm fluorescent
            {
            514,    // i4R
            512,    // i4G
            799    // i4B
            },
            // Incandescent
            {
            498,    // i4R
            512,    // i4G
            775    // i4B
            },
            // Gray World
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        // AWB preference color	
        {
            // Tungsten
            {
            0,    // i4SliderValue
            4219    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            4352    // i4OffsetThr
            },
            // Shade
            {
            0,    // i4SliderValue
            1342    // i4OffsetThr
            },
            // Daylight WB gain
            {
            644,    // i4R
            512,    // i4G
            599    // i4B
            },
            // Preference gain: strobe
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: tungsten
            {
            489,    // i4R
            512,    // i4G
            500    // i4B
            },
            // Preference gain: warm fluorescent
            {
            480,    // i4R
            512,    // i4G
            523    // i4B
            },
            // Preference gain: fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: CWF
            {
            540,    // i4R
            512,    // i4G
            510    // i4B
            },
            // Preference gain: daylight
            {
            520,    // i4R
            512,    // i4G
            490    // i4B
            },
            // Preference gain: shade
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        {// CCT estimation
            {// CCT
                2300,    // i4CCT[0]
                2850,    // i4CCT[1]
                4100,    // i4CCT[2]
                5100,    // i4CCT[3]
                6500    // i4CCT[4]
            },
            {// Rotated X coordinate
                -363,    // i4RotatedXCoordinate[0]
                -283,    // i4RotatedXCoordinate[1]
                -207,    // i4RotatedXCoordinate[2]
                -93,    // i4RotatedXCoordinate[3]
                0    // i4RotatedXCoordinate[4]
            }
        }
    },
    {0}
};

#include INCLUDE_FILENAME_ISP_LSC_PARAM
//};  //  namespace
const CAMERA_TSF_TBL_STRUCT CAMERA_TSF_DEFAULT_VALUE =
{
    #include INCLUDE_FILENAME_TSF_PARA
    #include INCLUDE_FILENAME_TSF_DATA
};


typedef NSFeature::RAWSensorInfo<SENSOR_ID> SensorInfoSingleton_T;


namespace NSFeature {
template <>
UINT32
SensorInfoSingleton_T::
impGetDefaultData(CAMERA_DATA_TYPE_ENUM const CameraDataType, VOID*const pDataBuf, UINT32 const size) const
{
    UINT32 dataSize[CAMERA_DATA_TYPE_NUM] = {sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT),
                                             sizeof(NVRAM_CAMERA_3A_STRUCT),
                                             sizeof(NVRAM_CAMERA_SHADING_STRUCT),
                                             sizeof(NVRAM_LENS_PARA_STRUCT),
                                             sizeof(AE_PLINETABLE_T),
                                             0,
                                             sizeof(CAMERA_TSF_TBL_STRUCT)};

    if (CameraDataType > CAMERA_DATA_AE_PLINETABLE || NULL == pDataBuf || (size < dataSize[CameraDataType]))
    {
        return 1;
    }
    if (CameraDataType > CAMERA_DATA_TSF_TABLE || NULL == pDataBuf || (size < dataSize[CameraDataType]))
    {
        return 1;
    }
    switch(CameraDataType)
    {
        case CAMERA_NVRAM_DATA_ISP:
			XLOGD("gpwtest CAMERA_NVRAM_DATA_ISP" );
            memcpy(pDataBuf,&CAMERA_ISP_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_3A:
			XLOGD("gpwtest CAMERA_NVRAM_DATA_3A" );
            memcpy(pDataBuf,&CAMERA_3A_NVRAM_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_3A_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_SHADING:
			XLOGD("gpwtest CAMERA_NVRAM_DATA_SHADING" );
            memcpy(pDataBuf,&CAMERA_SHADING_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_SHADING_STRUCT));
            break;
        case CAMERA_DATA_AE_PLINETABLE:
			XLOGD("gpwtest CAMERA_DATA_AE_PLINETABLE" );
            memcpy(pDataBuf,&g_PlineTableMapping,sizeof(AE_PLINETABLE_T));
            break;
        case CAMERA_DATA_TSF_TABLE:
			XLOGD("gpwtest CAMERA_DATA_TSF_TABLE" );
            memcpy(pDataBuf,&CAMERA_TSF_DEFAULT_VALUE,sizeof(CAMERA_TSF_TBL_STRUCT));
            break;
        default:
            break;
    }
    return 0;
}}; // NSFeature


