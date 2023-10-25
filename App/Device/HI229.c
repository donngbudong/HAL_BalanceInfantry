#include "HI229.h"
#include "Device.h"
static Packet_t RxPkt; /* used for data receive */
static int16_t acc[3];
static int16_t gyo[3];
static int16_t mag[3];
static float eular[3];
static float quat[4];
static uint8_t id;


IMU_GET_DATA_t IMU_Get_Data;
void IMU_get(void)
{
	//ÍÓÂÝÒÇÅ·À­½Ç
	get_eular(IMU_Get_Data.IMU_Eular);
	//ÍÓÂÝÒÇ½ÇËÙ¶È
	get_raw_gyo(IMU_Get_Data.IMU_Gyo);
}


/**
 * @brief IMUÊý¾Ý×´Ì¬ÉÏ±¨
 * @param 
 */
void IMU_State_Report(void)
{
  System.Imu_State =IMU_Check();
}


/**
 * @brief Ò£¿ØÆ÷×Ü×Ô¼ì
 * @param ·µ»Ø×´Ì¬±àºÅ
 */
Imu_State_t IMU_Check(void)
{
  static Imu_State_t res;
  res = IMU_NORMAL;
  if(IF_IMU_LOST)
    res = IMU_LOST;
//  if(IF_RC_DATAERR)
//    res = SYSTEM_RCERR;
  return res;
}



/**
 * @brief IMUÁ¬½Ó×Ô¼ì
 * @param 
 */
bool Judge_IMU_Lost(void)
{
  static bool res ;
  	res = false;
  if(micros() >= Imu_time)
    res = true;
  return res;
}






int get_raw_acc(int16_t* a)
{
    memcpy(a, acc, sizeof(acc));
    return 0;
}

int get_raw_gyo(int16_t* g)
{
    memcpy(g, gyo, sizeof(gyo));
    return 0;
}

int get_raw_mag(int16_t* m)
{
    memcpy(m, mag, sizeof(mag));
    return 0;
}

int get_eular(float* e)
{
    memcpy(e, eular, sizeof(eular));
    return 0;
}

int get_quat(float* q)
{
    memcpy(q, quat, sizeof(quat));
    return 0;
}

int get_id(uint8_t *user_id)
{
    *user_id = id;
    return 0;
}

static void OnDataReceived(Packet_t *pkt)
{
    int offset = 0;
    uint8_t *p = pkt->buf;
    while(offset < pkt->payload_len)
    {
        switch(p[offset])
        {
            case kItemID:
                id = p[1];
                offset += 2;
                break;
            case kItemAccRaw:
            case kItemAccCalibrated:
            case kItemAccFiltered:
            case kItemAccLinear:
                memcpy(acc, p + offset + 1, sizeof(acc));
                offset += 7;
                break;
            case kItemGyoRaw:
            case kItemGyoCalibrated:
            case kItemGyoFiltered:
                memcpy(gyo, p + offset + 1, sizeof(gyo));
                offset += 7;
                break;
            case kItemMagRaw:
            case kItemMagCalibrated:
            case kItemMagFiltered:
                memcpy(mag, p + offset + 1, sizeof(mag));
                offset += 7;
                break;
            case kItemRotationEular:
                eular[0] = ((float)(int16_t)(p[offset+1] + (p[offset+2]<<8)))/100;
                eular[1] = ((float)(int16_t)(p[offset+3] + (p[offset+4]<<8)))/100;
                eular[2] = ((float)(int16_t)(p[offset+5] + (p[offset+6]<<8)))/10;
                offset += 7;
                break;
            case kItemRotationEular2:
                memcpy(eular, p + offset + 1, sizeof(eular));
                offset += 13;
                break;
            case kItemRotationQuat:
                memcpy(quat, p + offset + 1, sizeof(quat));
                offset += 17;
                break;
            case kItemPressure:
                offset += 5;
                break;
            case kItemTemperature:
                offset += 5;
                break;
            default:
				offset++;
                break;
        }
    }
    
}

int IMU_Init(void)
{
    Packet_DecodeInit(&RxPkt, OnDataReceived);
    return 0;
}


//uint8_t ID;
//int16_t Acc[3];
//int16_t Gyo[3];
//int16_t Mag[3];
//float Eular[3];
//float Quat[4];
//int32_t Pressure;

//void imu_ctrl(void)
//{
//	get_raw_acc(Acc);
//	get_raw_gyo(Gyo);
//	get_raw_mag(Mag);
//	get_eular(Eular);
//	get_quat(Quat);
//	get_id(&ID);
//	
//}

