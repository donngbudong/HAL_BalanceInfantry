#ifndef __REFEREE_H
#define __REFEREE_H
#include <string.h>

#include "System.h"
/***************命令码ID********************/

/* 

	ID: 0x0001  Byte:  11		比赛状态数据       			固定 3Hz 频率发送
	ID: 0x0002  Byte:  1    比赛结果数据         		比赛结束触发发送
	ID: 0x0003  Byte:  32   比赛机器人血量数据   		固定 3Hz 频率发送
	
	ID: 0x0101  Byte:  4    场地事件数据							固定 3Hz 频率发送
	ID: 0x0102  Byte:  4    补给站动作标识数据				补给站弹丸释放时触发发送 
	ID: 0X0104  Byte:  2    裁判警告数据							己方判罚/判负时触发发送
	ID: 0x0105  Byte:  1    飞镖发射时间数据					固定 3Hz 频率发送
	
	ID: 0X0201  Byte: 27    机器人性能体系数据				固定 10Hz 频率发送
	ID: 0X0202  Byte: 16    实时功率热量数据   			固定 50Hz 频率发送       
	ID: 0x0203  Byte: 16    机器人位置数据						固定 10Hz 频率发送
	ID: 0x0204  Byte:  1    机器人增益数据						固定 3Hz 频率发送
	ID: 0x0205  Byte:  1    空中支援时间数据					固定 10Hz 频率发送
	ID: 0x0206  Byte:  1    伤害状态数据           	伤害发生后发送
	ID: 0x0207  Byte:  7    实时射击数据           	弹丸发射后发送
	ID: 0x0208  Byte:  6    允许发弹量								固定 10Hz 频率发送
	ID: 0x0209  Byte:  4    机器人 RFID 状态					固定 3Hz 频率发送
	
	ID: 0x0301  Byte:  128	机器人交互数据													发送方触发发送，频率上限为 10Hz
	ID: 0x0302  Byte:  30		自定义控制器与机器人交互数据						发送方触发发送，频率上限为 30Hz
	ID: 0x0303  Byte:  15		选手端小地图交互数据										选手端触发发送
	ID: 0x0304  Byte:  12 	键鼠遥控数据														固定 30Hz 频率发送
	ID: 0x0305  Byte:  10 	选手端小地图接收雷达数据								频率上限为10Hz
	ID: 0x0306  Byte:  8		自定义控制器与选手端交互数据						频率上限为 30Hz

*/

//#define JUDGE_BUFFER_LEN 200              //缓存区长度

#define 		JUDGE_FRAME_HEADER 0xA5           //帧头

#define    JUDGE_DATA_ERROR      0
#define    JUDGE_DATA_CORRECT    1     //裁判系统可以不可以用

#define shoot_speed_limit 20;

#define TRUE 1
#define FALSE 0

#define BLUE 0
#define RED 1

//长度根据协议定义,数据段长度为n需要根据帧头第二字节来获取
#define    LEN_HEADER    5        //帧头长
#define    LEN_CMDID     2        //命令码长度
#define    LEN_TAIL      2	      //帧尾CRC16



/*心跳*/
bool Judge_IF_Refer_Normal(void);
#define     IF_REFER_NORMAL      Judge_IF_Refer_Normal()


//通信协议格式
typedef enum  
{
	FRAME_HEADER         = 5,
	CMD_ID               = 2,
	DATA                 = 7,
}JudgeFrameOffset;

// frame_header 格式
typedef enum
{
	SOF          = 0,//1		数据帧起始字节，固定值为0xA5
	DATA_LENGTH  = 1,//2		数据帧中date的长度
	SEQ          = 3,//1		包序号
	CRC8         = 4 //1		帧头CRC8	
}	FrameHeaderOffset;


typedef enum
{
	ID_game_status											= 0x0001,
	ID_game_result											= 0x0002,
	ID_game_robot_HP										= 0x0003,//比赛机器人存活数据
	
	ID_event_data  											= 0x0101,//场地事件数据 
	ID_supply_projectile_action   			= 0x0102,//场地补给站动作标识数据
	ID_referee_warning								 	= 0x0104,//裁判系统警告数据
	ID_dart_remaining_time 							= 0x0105  , //飞镖发射口倒计时
	
	ID_robot_status    									= 0x0201,//机器人状态数据
	ID_power_heat_data    							= 0x0202,//实时功率热量数据
	ID_robot_pos        								= 0x0203,//机器人位置数据
	ID_buff															= 0x0204,//机器人增益数据
	ID_air_support_data									= 0x0205,//空中机器人能量状态数据
	ID_hurt_data												= 0x0206,//伤害状态数据
	ID_shoot_data												= 0x0207,//实时射击数据
	ID_projectile_allowance         		= 0x0208,//剩余发射数
	ID_rfid_status											= 0x0209,//机器人RFID状态，1Hz
  
	ID_robot_interactive_header_data		= 0x0301,//机器人交互数据，发送方触发——发送 10Hz
	ID_custom_robot_data								= 0x0302,//自定义控制器交互数据接口，通过——自定义控制器—>图传连接的机器人
	ID_map_command        							= 0x0303,//选手端小地图交互数据，——触发发送——
	ID_remote_control				            = 0x0304,//键鼠遥控数据，通过——图传串口——发送
	ID_map_robot_data              			= 0x0305,//选手端小地图接收雷达数据，频率上限为10Hz
	ID_custom_client_data								= 0x0306 //自定义控制器与选手端交互数据，发送方触发发送，频率上限为 30Hz
}CmdID;

typedef enum
{
	/* Std */
	LEN_FRAME_HEAD 	               	 		= 5,	// 帧头长度
	LEN_CMD_ID 		                 	 		=	2,	// 命令码长度
	LEN_FRAME_TAIL 	               	 		= 2,	// 帧尾CRC16
	
	/* Ext */  
	LEN_game_status											= 11,	//0x0001	比赛状态数据
	LEN_game_result       							= 1,	//0x0002	比赛结果数据
	LEN_game_robot_HP       						=	32,	//0x0003  比赛机器人血量数据
	
	LEN_event_data  										= 4,	//0x0101  场地事件数据 
	LEN_supply_projectile_action     		= 4,	//0x0102	补给站动作标识数据
	LEN_referee_warning        					=	2, 	//0x0104	裁判系统警告
	LEN_dart_remaining_time 						=	1, 	//0x0105	飞镖发射时间数据
	
	LEN_robot_status    								= 27,	//0x0201	机器人性能体系数据
	LEN_power_heat_data   							= 16,	//0x0202	实时功率热量数据
	LEN_robot_pos        								= 16,	//0x0203	机器人位置数据
	LEN_buff        										= 1,	//0x0204	机器人增益数据
	LEN_air_support_data        				= 1,	//0x0205	空中支援时间数据
	LEN_hurt_data        								= 1,	//0x0206	伤害状态数据
	LEN_shoot_data       								= 7,	//0x0207	实时射击数据
	LEN_projectile_allowance          	= 6,	//0x0208	允许发单量
	LEN_rfid_status					         		= 4,	//0x0209	机器人RFID状态
	
	// 0x030x
	//LEN_robot_interactive_header_data      = n,
	//LEN_controller_interactive_header_data = n,
	LEN_map_interactive_headerdata           = 15,
	LEN_keyboard_information                 = 12,//0x0304

}JudgeDataLength;

/* 自定义帧头 */
typedef __packed struct
{
	uint8_t  SOF;
	uint16_t DataLength;
	uint8_t  Seq;
	uint8_t  CRC8;
	
} xFrameHeader;

/* ID: 0x0001  Byte:  11    比赛状态数据 */
typedef __packed struct
{
	uint8_t game_type : 4;
	uint8_t game_progress : 4;
	uint16_t stage_remain_time;
	uint64_t SyncTimeStamp;
}game_status_t;

/* ID: 0x0002  Byte:  1			比赛结果数据 */
typedef __packed struct
{
	uint8_t winner;
}game_result_t;

/* ID: 0x0003  Byte:  32    比赛机器人血量数据 */
typedef __packed struct
{
	uint16_t red_1_robot_HP;
	uint16_t red_2_robot_HP;
	uint16_t red_3_robot_HP;
	uint16_t red_4_robot_HP;
	uint16_t red_5_robot_HP;
	uint16_t red_7_robot_HP;
	uint16_t red_outpost_HP;
	uint16_t red_base_HP;
	uint16_t blue_1_robot_HP;
	uint16_t blue_2_robot_HP;
	uint16_t blue_3_robot_HP;
	uint16_t blue_4_robot_HP;
	uint16_t blue_5_robot_HP;
	uint16_t blue_7_robot_HP;
	uint16_t blue_outpost_HP;
	uint16_t blue_base_HP;
}game_robot_HP_t;


/* ID: 0x0101  Byte:  4    场地事件数据 */
typedef __packed struct
{
	uint32_t event_data;
}event_data_t;


/* ID: 0x0102  Byte:  4    补给站动作标识数据 */
typedef __packed struct
{
	uint8_t supply_projectile_id;
	uint8_t supply_robot_id;
	uint8_t supply_projectile_step;
	uint8_t supply_projectile_num;
}supply_projectile_action_t;

/* ID: 0x0104  Byte: 2		裁判系统警告信息 */
typedef __packed struct
{
	uint8_t level;
	uint8_t offending_robot_id;
}referee_warning_t;

/* ID: 0x0105  Byte:1			飞镖发射时间数据 */
typedef __packed struct
{
	uint8_t dart_remaining_time;
}dart_remaining_time_t;

/* ID: 0x0201  Byte: 27		机器人性能体系数据 */
typedef __packed struct
{
	uint8_t robot_id;
	uint8_t robot_level;
	uint16_t current_HP;
	uint16_t maximum_HP;
	uint16_t shooter_id1_17mm_barrel_cooling_value;
	uint16_t shooter_id1_17mm_barrel_heat_limit;
	uint16_t shooter_id1_17mm_initial_launching_speed_limit;
	uint16_t shooter_id2_17mm_barrel_cooling_valuecooling_rate;
	uint16_t shooter_id2_17mm_barrel_heatcooling_limit; 
	uint16_t shooter_id2_17mm_initial_launching_speed_limit;
	uint16_t shooter_id1_42mm_barrel_cooling_value;
	uint16_t shooter_id1_42mm_barrel_heat_cooling_limit;
	uint16_t shooter_id1_42mm_initial_launching_speed_limit;
	uint16_t chassis_power_limit;
	uint8_t power_management_gimbal_output : 1;
	uint8_t power_management_chassis_output : 1;
	uint8_t power_management_shooter_output : 1;
}robot_status_t;

/* ID: 0X0202  Byte: 16    实时功率热量数据 */
typedef __packed struct
{
	uint16_t chassis_voltage;
	uint16_t chassis_current;
	float chassis_power;
	uint16_t buffer_energy;
	uint16_t shooter_17mm_1_barrel_heat;
	uint16_t shooter_17mm_2_barrel_heat;
	uint16_t shooter_42mm_barrel_heat;
}power_heat_data_t;

/* ID: 0x0203  Byte: 16    机器人位置数据 */
typedef __packed struct
{
	float x;
	float y;
	float z;
	float angle;
}robot_pos_t;

/* ID: 0x0204  Byte:  5    机器人增益数据 */
typedef __packed struct
{
 uint8_t recovery_buff;
 uint8_t cooling_buff;
 uint8_t defence_buff;
 uint16_t attack_buff;
}buff_t;

/* ID: 0x0205  Byte:  1    空中支援时间数据 */
typedef __packed struct
{
	uint8_t airforce_status;
	uint8_t time_remain;
}air_support_data_t;

/* ID: 0x0206  Byte:  1    伤害状态数据 */
typedef __packed struct
{
	uint8_t armor_id : 4;
	uint8_t HP_deduction_reason : 4;
}hurt_data_t;

/* ID: 0x0207  Byte:  7    实时射击数据 */
typedef __packed struct
{
	uint8_t bullet_type;
	uint8_t shooter_number;
	uint8_t launching_frequency;
	float initial_speed;
}shoot_data_t;


/* ID: 0x0208  Byte:  6    允许发弹量 */
typedef __packed struct
{
	uint16_t projectile_allowance_17mm;
	uint16_t projectile_allowance_42mm;
	uint16_t remaining_gold_coin;
}projectile_allowance_t;

/* ID: 0x0209  Byte:  4 	机器人RFID状态 */
typedef __packed struct
{
	uint32_t rfid_status;
}rfid_status_t;


/* 
	
	交互数据，包括一个统一的数据段头结构，
	数据段头结构包括内容 ID、发送者和接收者的 ID、内容数据段
	学生交互数据包的总长不超过 128 个字节
	减去 frame_header、cmd_id 和 frame_tail 的 9 个字节以及数据段头结构的 6 个字节，
	故学生交互数据的内容数据段最大为 113个字节
	整个交互数据的包上行频率为 10Hz。

	机器人 ID：
	1，英雄(红)；
	2，工程(红)；
	3/4/5，步兵(红)；
	6，空中(红)；
	7，哨兵(红)；
	11，英雄(蓝)；
	12，工程(蓝)；
	13/14/15，步兵(蓝)；
	16，空中(蓝)；
	17，哨兵(蓝)。 
	客户端 ID： 
	0x0101 为英雄操作手客户端( 红) ；
	0x0102 ，工程操作手客户端 ((红 )；
	0x0103/0x0104/0x0105，步兵操作手客户端(红)；
	0x0106，空中操作手客户端((红)； 
	0x0111，英雄操作手客户端(蓝)；
	0x0112，工程操作手客户端(蓝)；
	0x0113/0x0114/0x0115，操作手客户端步兵(蓝)；
	0x0116，空中操作手客户端(蓝)。 
*/

/* ID: 0x0301  Byte:  128 	机器人交互数据 */
typedef __packed struct
{
	uint16_t data_cmd_id;
	uint16_t sender_id;
	uint16_t receiver_id;
//	uint8_t user_data[x];
}robot_interaction_data_t;


/* ID: 0x0302  Byte:  30 	自定义控制器与机器人交互数据 */
typedef __packed struct
{
	uint8_t data[30];
}custom_robot_data_t;

/* ID: 0x0303  Byte:  15 	选手端小地图交互数据 */
typedef __packed struct
{
	float target_position_x;
	float target_position_y;
	float target_position_z;
	uint8_t commd_keyboard;
	uint16_t target_robot_id;
}map_command_t;
/* ID: 0x0304  Byte:  12 	键鼠遥控数据 */
typedef __packed struct
{
	int16_t mouse_x;
	int16_t mouse_y;
	int16_t mouse_z;
	int8_t left_button_down;
	int8_t right_button_down;
	uint16_t keyboard_value;
	uint16_t reserved;
}remote_control_t;

/* ID: 0x0305  Byte:  10 	选手端小地图接收雷达数据 */
typedef __packed struct
{
	uint16_t target_robot_id;
	float target_position_x;
	float target_position_y;
}map_robot_data_t;

/* ID: 0x0306  Byte:  8 	自定义控制器与选手端交互数据 */
typedef __packed struct
{
	uint16_t key_value;
	uint16_t x_position:12;
	uint16_t mouse_left:4;
	uint16_t y_position:12;
	uint16_t mouse_right:4;
	uint16_t reserved;
}custom_client_data_t;


/*
	子内容	ID			内容数据段长度 				功能说明
0x0200~0x02FF 		x≤113 						机器人之间通信
0x0100 						2 								选手端删除图层
0x0101						15 								选手端绘制一个图形
0x0102 						30 								选手端绘制两个图形
0x0103 						75 								选手端绘制五个图形
0x0104 						105 							选手端绘制七个图形
0x0110 						5 								选手端绘制字符图形
*/




enum judge_robot_ID{
	hero_red       = 1,
	engineer_red   = 2,
	infantry3_red  = 3,
	infantry4_red  = 4,
	infantry5_red  = 5,
	plane_red      = 6,
	
	hero_blue      = 101,
	engineer_blue  = 102,
	infantry3_blue = 103,
	infantry4_blue = 104,
	infantry5_blue = 105,
	plane_blue     = 106,
};
typedef struct{
	uint16_t teammate_hero;
	uint16_t teammate_engineer;
	uint16_t teammate_infantry3;
	uint16_t teammate_infantry4;
	uint16_t teammate_infantry5;
	uint16_t teammate_plane;
	uint16_t teammate_sentry;
	
	uint16_t client_hero;
	uint16_t client_engineer;
	uint16_t client_infantry3;
	uint16_t client_infantry4;
	uint16_t client_infantry5;
	uint16_t client_plane;
} ext_interact_id_t;



/* 
	学生机器人间通信 cmd_id 0x0301，内容 ID:0x0200~0x02FF
	交互数据 机器人间通信：0x0301。
	发送频率：上限 10Hz  

	字节偏移量 	大小 	说明 			备注 
	0 			2 		数据的内容 ID 	0x0200~0x02FF 
										可以在以上 ID 段选取，具体 ID 含义由参赛队自定义 
	
	2 			2 		发送者的 ID 	需要校验发送者的 ID 正确性， 
	
	4 			2 		接收者的 ID 	需要校验接收者的 ID 正确性，
										例如不能发送到敌对机器人的ID 
	
	6 			n 		数据段 			n 需要小于 113 

*/



/**
  * @brief  读取裁判数据,loop中循环调用此函数来读取数据
  * @param  缓存数据
  * @retval 是否对正误判断做处理
  * @attention  在此判断帧头和CRC校验,无误再写入数据
  */
bool Judege_read_data(uint8_t  *ReadFromUsart );





typedef struct judge_info_struct 
{
	xFrameHeader										FrameHeader;							// 帧头信息
	
	game_status_t 									Game_Status;							// 0x0001         比赛状态数据
	game_result_t										Game_Result;							// 0x0002         比赛结果数据
	game_robot_HP_t 								Game_Robot_HP;						// 0x0003         机器人血量数据
	
	event_data_t										Event_Data;								// 0x0101         场地事件数据
	supply_projectile_action_t			Supply_Projectile_Action;	// 0x0102 				补给站动作标识数据
	referee_warning_t								Referee_Warning;					// 0x0104         裁判警告信息
	dart_remaining_time_t						Dart_Remaining_Time;			// 0x0105         飞镖发射时间数据
	
	robot_status_t									Robot_Status;							// 0x0201         机器人性能体系数据
	power_heat_data_t								Power_Heat_Data;					// 0x0202         实时功率热量数据
	robot_pos_t											Robot_Pos;								// 0x0203         机器人位置数据
	buff_t													Buff;											// 0x0204     		机器人增益数据
	air_support_data_t							Air_Support_date;					// 0x0205         空中支援时间数据
	hurt_data_t											Hurt_Date;								// 0x0206         伤害状态数据
	shoot_data_t										Shoot_Data;								// 0x0207         实时射击信息(射频  射速  子弹信息)
	projectile_allowance_t					Projectile_allowance;			// 0x0208	        允许发弹量
	rfid_status_t										Rfid_Status;							// 0x0209	        机器人 RFID 状态
	
//	ext_interact_id_t								ids;								//与本机交互的机器人id
//	uint16_t                        self_client;        //本机客户端
//	bool	 		IF_REF_ONL;

} Referee_info_t;

#endif 


