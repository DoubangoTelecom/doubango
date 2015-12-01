/*
 * Copyright 1993-2010 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

#ifndef TYPES_H
#define TYPES_H

#include "NVEncodeDataTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct NVEncoderParams
{
    char configFile[256];
    char inputFile[256];
    char outputFile[256];
	int measure_psnr;
	int measure_fps;
	int force_device;
	int iSurfaceFormat;
	int iPictureType;
	int nDeviceMemPitch;

	int						iCodecType;	      //	NVVE_CODEC_TYPE,
	int                     GPU_count;        //    Choose the specific GPU count
	int                     GPU_devID;        //    Choose the specific GPU device ID
	int                     iUseDeviceMem;    //    CUDA with DEVICE_MEMORY_INPUT (for encoding)
	int                     iForcedGPU;       //    NVVE_FORCE_GPU_SELECTION            //F22
    int                     iOutputSize[2];   //    NVVE_OUT_SIZE,
    int                     iInputSize[2];    //    NVVE_IN_SIZE,
	float                   fAspectRatio;     //
    int                     iAspectRatio[3];  //    NVVE_ASPECT_RATIO,
    NVVE_FIELD_MODE         Fieldmode;        //    NVVE_FIELD_ENC_MODE,
    int                     iP_Interval;      //    NVVE_P_INTERVAL,
    int                     iIDR_Period;      //    NVVE_IDR_PERIOD,
    int                     iDynamicGOP;      //    NVVE_DYNAMIC_GOP,
    NVVE_RateCtrlType       RCType;           //    NVVE_RC_TYPE,
    int                     iAvgBitrate;      //    NVVE_AVG_BITRATE,
    int                     iPeakBitrate;     //    NVVE_PEAK_BITRATE,
    int                     iQP_Level_Intra;  //    NVVE_QP_LEVEL_INTRA,
    int                     iQP_Level_InterP; //    NVVE_QP_LEVEL_INTER_P,
    int                     iQP_Level_InterB; //    NVVE_QP_LEVEL_INTER_B,
    int                     iFrameRate[2];    //    NVVE_FRAME_RATE,
    int                     iDeblockMode;     //    NVVE_DEBLOCK_MODE,
    int                     iProfileLevel;    //    NVVE_PROFILE_LEVEL,
    int                     iForceIntra;      //    NVVE_FORCE_INTRA,
    int                     iForceIDR;        //    NVVE_FORCE_IDR,
    int                     iClearStat;       //    NVVE_CLEAR_STAT,
    NVVE_DI_MODE            DIMode;           //    NVVE_SET_DEINTERLACE,
    NVVE_PRESETS_TARGET     Presets;          //    NVVE_PRESETS,
    int                     iDisableCabac;    //    NVVE_DISABLE_CABAC,
    int                     iNaluFramingType; //    NVVE_CONFIGURE_NALU_FRAMING_TYPE
    int                     iDisableSPSPPS;   //    NVVE_DISABLE_SPS_PPS
	NVVE_GPUOffloadLevel    GPUOffloadLevel;  //    NVVE_GPU_OFFLOAD_LEVEL
	NVVE_GPUOffloadLevel    MaxOffloadLevel;  //    NVVE_GPU_OFFLOAD_LEVEL_MAX
	int                     iSliceCnt;        //    NVVE_SLICE_COUNT                    //F19
	int                     iMultiGPU;        //    NVVE_MULTI_GPU                      //F21
    int                     iDeviceMemInput;  //    NVVE_DEVICE_MEMORY_INPUT            //F23

//    NVVE_STAT_NUM_CODED_FRAMES,
//    NVVE_STAT_NUM_RECEIVED_FRAMES,
//    NVVE_STAT_BITRATE,
//    NVVE_STAT_NUM_BITS_GENERATED,
//    NVVE_GET_PTS_DIFF_TIME,
//    NVVE_GET_PTS_BASE_TIME,
//    NVVE_GET_PTS_CODED_TIME,
//    NVVE_GET_PTS_RECEIVED_TIME,
//    NVVE_STAT_ELAPSED_TIME,
//    NVVE_STAT_QBUF_FULLNESS,
//    NVVE_STAT_PERF_FPS,
//    NVVE_STAT_PERF_AVG_TIME,
};

typedef struct  {
	char *name;
	char *yuv_type;
	int  bpp;
} _sYUVParams;

static _sYUVParams sSurfaceFormat[] =
{
	{ "UYVY", "4:2:2", 16 },
	{ "YUY2", "4:2:2", 16 },
	{ "YV12", "4:2:0", 12 },
	{ "NV12", "4:2:0", 12 },
	{ "IYUV", "4:2:0", 12 },
	{ NULL  , 0 }
};

typedef struct  {
	char *name;
	int  params;
} _sNVVEEncodeParams;

static _sNVVEEncodeParams sNVVE_EncodeParams[] = 
{
	{ "UNDEFINED", 1 },
    { "NVVE_OUT_SIZE", 2 },
    { "NVVE_ASPECT_RATIO", 3 },
    { "NVVE_FIELD_ENC_MODE", 1 },
    { "NVVE_P_INTERVAL", 1 },
    { "NVVE_IDR_PERIOD", 1 },
    { "NVVE_DYNAMIC_GOP", 1 },
    { "NVVE_RC_TYPE", 1 },
	{ "NVVE_AVG_BITRATE", 1 },
    { "NVVE_PEAK_BITRATE", 1 },
    { "NVVE_QP_LEVEL_INTRA", 1 },
    { "NVVE_QP_LEVEL_INTER_P", 1 },
    { "NVVE_QP_LEVEL_INTER_B", 1 },
    { "NVVE_FRAME_RATE", 2 },
    { "NVVE_DEBLOCK_MODE", 1 },
    { "NVVE_PROFILE_LEVEL", 1 },
    { "NVVE_FORCE_INTRA (DS)", 1 },            //DShow only
    { "NVVE_FORCE_IDR   (DS)", 1 },            //DShow only
    { "NVVE_CLEAR_STAT  (DS)", 1 },            //DShow only
    { "NVVE_SET_DEINTERLACE", 1 },
    { "NVVE_PRESETS", 1 },
	{ "NVVE_IN_SIZE", 2 },
    { "NVVE_STAT_NUM_CODED_FRAMES (DS)", 1 },       //DShow only
    { "NVVE_STAT_NUM_RECEIVED_FRAMES (DS)", 1 },    //DShow only
    { "NVVE_STAT_BITRATE (DS)", 1 },                //DShow only
    { "NVVE_STAT_NUM_BITS_GENERATED (DS)", 1 },     //DShow only
    { "NVVE_GET_PTS_DIFF_TIME (DS)", 1 },           //DShow only
    { "NVVE_GET_PTS_BASE_TIME (DS)", 1 },           //DShow only
    { "NVVE_GET_PTS_CODED_TIME (DS)", 1 },          //DShow only
    { "NVVE_GET_PTS_RECEIVED_TIME (DS)", 1 },       //DShow only
    { "NVVE_STAT_ELAPSED_TIME (DS)", 1 },           //DShow only
    { "NVVE_STAT_QBUF_FULLNESS (DS)", 1 },          //DShow only
    { "NVVE_STAT_PERF_FPS (DS)", 1 },               //DShow only
    { "NVVE_STAT_PERF_AVG_TIME (DS)", 1 },          //DShow only
    { "NVVE_DISABLE_CABAC", 1 },
	{ "NVVE_CONFIGURE_NALU_FRAMING_TYPE", 1 },
    { "NVVE_DISABLE_SPS_PPS", 1 },
    { "NVVE_SLICE_COUNT", 1 },
    { "NVVE_GPU_OFFLOAD_LEVEL", 1 },
    { "NVVE_GPU_OFFLOAD_LEVEL_MAX", 1 },
    { "NVVE_MULTI_GPU", 1 },
    { "NVVE_GET_GPU_COUNT", 1 },
    { "NVVE_GET_GPU_ATTRIBUTES", 1 },
    { "NVVE_FORCE_GPU_SELECTION", 1 },
	{ "NVVE_DEVICE_MEMORY_INPUT", 1 },
	{ "NVVE_DEVICE_CTX_LOCK", 1 },
	{ NULL, 0 }
};

static _sNVVEEncodeParams sProfileName[] = {
	{ "Baseline", 0x42 },
	{ "Main"    , 0x4d },
	{ "High"    , 0x64 },
	{ NULL      , 0    }
};

static _sNVVEEncodeParams sProfileLevel[] = {
	{ "1.0", 0x0a },
	{ "1.1", 0x0b },
	{ "1.2", 0x0c },
	{ "1.3", 0x0d },
	{ "2.0", 0x14 },
	{ "2.1", 0x15 },
	{ "2.2", 0x16 },
	{ "3.0", 0x1e },
	{ "3.1", 0x1f },
	{ "3.2", 0x20 },
	{ "4.0", 0x28 },
	{ "4.1", 0x29 },
	{ "4.2", 0x29 },
	{ "5.0", 0x32 },
	{ "5.1", 0x33 },
	{ "Auto", 0xff },
	{ NULL , 0    }
};

inline char * sProfileIDX2Char(_sNVVEEncodeParams *pProfile, int ID)
{
	int index;
	for (index = 0; pProfile[index].name != NULL; index++) {
		if (pProfile[index].params == ID) {
			// found the profile return the string
			return pProfile[index].name;
		}
	}
	return NULL;
}

static char *sVideoEncodePresets[] = {
	"PSP         ( 320x 240)",
	"iPod/iPhone ( 320x 240)",
	"AVCHD",
	"BluRay",
	"HDV_1440",
	"ZuneHD",
	"FlipCam",
	NULL
};

static char *sGPUOffloadLevel[] = {
	"CPU PEL processing",
	"Motion Estimation",
	"Full Encode",
	NULL
};

static char *sPictureType[] = {
	"Frame Mode",
	"Field Mode (top first)",
	"Field Mode (bottom first)",
	"Field Mode (picaff) unsupported",
	NULL
};

static char *sPictureStructure[] = {
	"Unknown",
	"Top Field",
	"Bottom Field",
	"Frame Picture",
	NULL
};

// Rate Control Method (NVVE_RC_TYPE)
static char *sNVVE_RateCtrlType[] = {
	"Rate Control CQP",
    "Rate Control VBR",
	"Rate Control CBR with QP",
	"Rate Control VBR with Min QP",
	NULL
};

#ifdef __cplusplus
}
#endif

#endif
