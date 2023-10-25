#ifndef __REFEREE_H
#define __REFEREE_H
#include <string.h>

#include "System.h"
/***************������ID********************/

/* 

	ID: 0x0001  Byte:  11		����״̬����       			�̶� 3Hz Ƶ�ʷ���
	ID: 0x0002  Byte:  1    �����������         		����������������
	ID: 0x0003  Byte:  32   ����������Ѫ������   		�̶� 3Hz Ƶ�ʷ���
	
	ID: 0x0101  Byte:  4    �����¼�����							�̶� 3Hz Ƶ�ʷ���
	ID: 0x0102  Byte:  4    ����վ������ʶ����				����վ�����ͷ�ʱ�������� 
	ID: 0X0104  Byte:  2    ���о�������							�����з�/�и�ʱ��������
	ID: 0x0105  Byte:  1    ���ڷ���ʱ������					�̶� 3Hz Ƶ�ʷ���
	
	ID: 0X0201  Byte: 27    ������������ϵ����				�̶� 10Hz Ƶ�ʷ���
	ID: 0X0202  Byte: 16    ʵʱ������������   			�̶� 50Hz Ƶ�ʷ���       
	ID: 0x0203  Byte: 16    ������λ������						�̶� 10Hz Ƶ�ʷ���
	ID: 0x0204  Byte:  1    ��������������						�̶� 3Hz Ƶ�ʷ���
	ID: 0x0205  Byte:  1    ����֧Ԯʱ������					�̶� 10Hz Ƶ�ʷ���
	ID: 0x0206  Byte:  1    �˺�״̬����           	�˺���������
	ID: 0x0207  Byte:  7    ʵʱ�������           	���跢�����
	ID: 0x0208  Byte:  6    ��������								�̶� 10Hz Ƶ�ʷ���
	ID: 0x0209  Byte:  4    ������ RFID ״̬					�̶� 3Hz Ƶ�ʷ���
	
	ID: 0x0301  Byte:  128	�����˽�������													���ͷ��������ͣ�Ƶ������Ϊ 10Hz
	ID: 0x0302  Byte:  30		�Զ��������������˽�������						���ͷ��������ͣ�Ƶ������Ϊ 30Hz
	ID: 0x0303  Byte:  15		ѡ�ֶ�С��ͼ��������										ѡ�ֶ˴�������
	ID: 0x0304  Byte:  12 	����ң������														�̶� 30Hz Ƶ�ʷ���
	ID: 0x0305  Byte:  10 	ѡ�ֶ�С��ͼ�����״�����								Ƶ������Ϊ10Hz
	ID: 0x0306  Byte:  8		�Զ����������ѡ�ֶ˽�������						Ƶ������Ϊ 30Hz

*/

//#define JUDGE_BUFFER_LEN 200              //����������

#define 		JUDGE_FRAME_HEADER 0xA5           //֡ͷ

#define    JUDGE_DATA_ERROR      0
#define    JUDGE_DATA_CORRECT    1     //����ϵͳ���Բ�������

#define shoot_speed_limit 20;

#define TRUE 1
#define FALSE 0

#define BLUE 0
#define RED 1

//���ȸ���Э�鶨��,���ݶγ���Ϊn��Ҫ����֡ͷ�ڶ��ֽ�����ȡ
#define    LEN_HEADER    5        //֡ͷ��
#define    LEN_CMDID     2        //�����볤��
#define    LEN_TAIL      2	      //֡βCRC16



/*����*/
bool Judge_IF_Refer_Normal(void);
#define     IF_REFER_NORMAL      Judge_IF_Refer_Normal()


//ͨ��Э���ʽ
typedef enum  
{
	FRAME_HEADER         = 5,
	CMD_ID               = 2,
	DATA                 = 7,
}JudgeFrameOffset;

// frame_header ��ʽ
typedef enum
{
	SOF          = 0,//1		����֡��ʼ�ֽڣ��̶�ֵΪ0xA5
	DATA_LENGTH  = 1,//2		����֡��date�ĳ���
	SEQ          = 3,//1		�����
	CRC8         = 4 //1		֡ͷCRC8	
}	FrameHeaderOffset;


typedef enum
{
	ID_game_status											= 0x0001,
	ID_game_result											= 0x0002,
	ID_game_robot_HP										= 0x0003,//���������˴������
	
	ID_event_data  											= 0x0101,//�����¼����� 
	ID_supply_projectile_action   			= 0x0102,//���ز���վ������ʶ����
	ID_referee_warning								 	= 0x0104,//����ϵͳ��������
	ID_dart_remaining_time 							= 0x0105  , //���ڷ���ڵ���ʱ
	
	ID_robot_status    									= 0x0201,//������״̬����
	ID_power_heat_data    							= 0x0202,//ʵʱ������������
	ID_robot_pos        								= 0x0203,//������λ������
	ID_buff															= 0x0204,//��������������
	ID_air_support_data									= 0x0205,//���л���������״̬����
	ID_hurt_data												= 0x0206,//�˺�״̬����
	ID_shoot_data												= 0x0207,//ʵʱ�������
	ID_projectile_allowance         		= 0x0208,//ʣ�෢����
	ID_rfid_status											= 0x0209,//������RFID״̬��1Hz
  
	ID_robot_interactive_header_data		= 0x0301,//�����˽������ݣ����ͷ������������� 10Hz
	ID_custom_robot_data								= 0x0302,//�Զ���������������ݽӿڣ�ͨ�������Զ����������>ͼ�����ӵĻ�����
	ID_map_command        							= 0x0303,//ѡ�ֶ�С��ͼ�������ݣ������������͡���
	ID_remote_control				            = 0x0304,//����ң�����ݣ�ͨ������ͼ�����ڡ�������
	ID_map_robot_data              			= 0x0305,//ѡ�ֶ�С��ͼ�����״����ݣ�Ƶ������Ϊ10Hz
	ID_custom_client_data								= 0x0306 //�Զ����������ѡ�ֶ˽������ݣ����ͷ��������ͣ�Ƶ������Ϊ 30Hz
}CmdID;

typedef enum
{
	/* Std */
	LEN_FRAME_HEAD 	               	 		= 5,	// ֡ͷ����
	LEN_CMD_ID 		                 	 		=	2,	// �����볤��
	LEN_FRAME_TAIL 	               	 		= 2,	// ֡βCRC16
	
	/* Ext */  
	LEN_game_status											= 11,	//0x0001	����״̬����
	LEN_game_result       							= 1,	//0x0002	�����������
	LEN_game_robot_HP       						=	32,	//0x0003  ����������Ѫ������
	
	LEN_event_data  										= 4,	//0x0101  �����¼����� 
	LEN_supply_projectile_action     		= 4,	//0x0102	����վ������ʶ����
	LEN_referee_warning        					=	2, 	//0x0104	����ϵͳ����
	LEN_dart_remaining_time 						=	1, 	//0x0105	���ڷ���ʱ������
	
	LEN_robot_status    								= 27,	//0x0201	������������ϵ����
	LEN_power_heat_data   							= 16,	//0x0202	ʵʱ������������
	LEN_robot_pos        								= 16,	//0x0203	������λ������
	LEN_buff        										= 1,	//0x0204	��������������
	LEN_air_support_data        				= 1,	//0x0205	����֧Ԯʱ������
	LEN_hurt_data        								= 1,	//0x0206	�˺�״̬����
	LEN_shoot_data       								= 7,	//0x0207	ʵʱ�������
	LEN_projectile_allowance          	= 6,	//0x0208	��������
	LEN_rfid_status					         		= 4,	//0x0209	������RFID״̬
	
	// 0x030x
	//LEN_robot_interactive_header_data      = n,
	//LEN_controller_interactive_header_data = n,
	LEN_map_interactive_headerdata           = 15,
	LEN_keyboard_information                 = 12,//0x0304

}JudgeDataLength;

/* �Զ���֡ͷ */
typedef __packed struct
{
	uint8_t  SOF;
	uint16_t DataLength;
	uint8_t  Seq;
	uint8_t  CRC8;
	
} xFrameHeader;

/* ID: 0x0001  Byte:  11    ����״̬���� */
typedef __packed struct
{
	uint8_t game_type : 4;
	uint8_t game_progress : 4;
	uint16_t stage_remain_time;
	uint64_t SyncTimeStamp;
}game_status_t;

/* ID: 0x0002  Byte:  1			����������� */
typedef __packed struct
{
	uint8_t winner;
}game_result_t;

/* ID: 0x0003  Byte:  32    ����������Ѫ������ */
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


/* ID: 0x0101  Byte:  4    �����¼����� */
typedef __packed struct
{
	uint32_t event_data;
}event_data_t;


/* ID: 0x0102  Byte:  4    ����վ������ʶ���� */
typedef __packed struct
{
	uint8_t supply_projectile_id;
	uint8_t supply_robot_id;
	uint8_t supply_projectile_step;
	uint8_t supply_projectile_num;
}supply_projectile_action_t;

/* ID: 0x0104  Byte: 2		����ϵͳ������Ϣ */
typedef __packed struct
{
	uint8_t level;
	uint8_t offending_robot_id;
}referee_warning_t;

/* ID: 0x0105  Byte:1			���ڷ���ʱ������ */
typedef __packed struct
{
	uint8_t dart_remaining_time;
}dart_remaining_time_t;

/* ID: 0x0201  Byte: 27		������������ϵ���� */
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

/* ID: 0X0202  Byte: 16    ʵʱ������������ */
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

/* ID: 0x0203  Byte: 16    ������λ������ */
typedef __packed struct
{
	float x;
	float y;
	float z;
	float angle;
}robot_pos_t;

/* ID: 0x0204  Byte:  5    �������������� */
typedef __packed struct
{
 uint8_t recovery_buff;
 uint8_t cooling_buff;
 uint8_t defence_buff;
 uint16_t attack_buff;
}buff_t;

/* ID: 0x0205  Byte:  1    ����֧Ԯʱ������ */
typedef __packed struct
{
	uint8_t airforce_status;
	uint8_t time_remain;
}air_support_data_t;

/* ID: 0x0206  Byte:  1    �˺�״̬���� */
typedef __packed struct
{
	uint8_t armor_id : 4;
	uint8_t HP_deduction_reason : 4;
}hurt_data_t;

/* ID: 0x0207  Byte:  7    ʵʱ������� */
typedef __packed struct
{
	uint8_t bullet_type;
	uint8_t shooter_number;
	uint8_t launching_frequency;
	float initial_speed;
}shoot_data_t;


/* ID: 0x0208  Byte:  6    �������� */
typedef __packed struct
{
	uint16_t projectile_allowance_17mm;
	uint16_t projectile_allowance_42mm;
	uint16_t remaining_gold_coin;
}projectile_allowance_t;

/* ID: 0x0209  Byte:  4 	������RFID״̬ */
typedef __packed struct
{
	uint32_t rfid_status;
}rfid_status_t;


/* 
	
	�������ݣ�����һ��ͳһ�����ݶ�ͷ�ṹ��
	���ݶ�ͷ�ṹ�������� ID�������ߺͽ����ߵ� ID���������ݶ�
	ѧ���������ݰ����ܳ������� 128 ���ֽ�
	��ȥ frame_header��cmd_id �� frame_tail �� 9 ���ֽ��Լ����ݶ�ͷ�ṹ�� 6 ���ֽڣ�
	��ѧ���������ݵ��������ݶ����Ϊ 113���ֽ�
	�����������ݵİ�����Ƶ��Ϊ 10Hz��

	������ ID��
	1��Ӣ��(��)��
	2������(��)��
	3/4/5������(��)��
	6������(��)��
	7���ڱ�(��)��
	11��Ӣ��(��)��
	12������(��)��
	13/14/15������(��)��
	16������(��)��
	17���ڱ�(��)�� 
	�ͻ��� ID�� 
	0x0101 ΪӢ�۲����ֿͻ���( ��) ��
	0x0102 �����̲����ֿͻ��� ((�� )��
	0x0103/0x0104/0x0105�����������ֿͻ���(��)��
	0x0106�����в����ֿͻ���((��)�� 
	0x0111��Ӣ�۲����ֿͻ���(��)��
	0x0112�����̲����ֿͻ���(��)��
	0x0113/0x0114/0x0115�������ֿͻ��˲���(��)��
	0x0116�����в����ֿͻ���(��)�� 
*/

/* ID: 0x0301  Byte:  128 	�����˽������� */
typedef __packed struct
{
	uint16_t data_cmd_id;
	uint16_t sender_id;
	uint16_t receiver_id;
//	uint8_t user_data[x];
}robot_interaction_data_t;


/* ID: 0x0302  Byte:  30 	�Զ��������������˽������� */
typedef __packed struct
{
	uint8_t data[30];
}custom_robot_data_t;

/* ID: 0x0303  Byte:  15 	ѡ�ֶ�С��ͼ�������� */
typedef __packed struct
{
	float target_position_x;
	float target_position_y;
	float target_position_z;
	uint8_t commd_keyboard;
	uint16_t target_robot_id;
}map_command_t;
/* ID: 0x0304  Byte:  12 	����ң������ */
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

/* ID: 0x0305  Byte:  10 	ѡ�ֶ�С��ͼ�����״����� */
typedef __packed struct
{
	uint16_t target_robot_id;
	float target_position_x;
	float target_position_y;
}map_robot_data_t;

/* ID: 0x0306  Byte:  8 	�Զ����������ѡ�ֶ˽������� */
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
	������	ID			�������ݶγ��� 				����˵��
0x0200~0x02FF 		x��113 						������֮��ͨ��
0x0100 						2 								ѡ�ֶ�ɾ��ͼ��
0x0101						15 								ѡ�ֶ˻���һ��ͼ��
0x0102 						30 								ѡ�ֶ˻�������ͼ��
0x0103 						75 								ѡ�ֶ˻������ͼ��
0x0104 						105 							ѡ�ֶ˻����߸�ͼ��
0x0110 						5 								ѡ�ֶ˻����ַ�ͼ��
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
	ѧ�������˼�ͨ�� cmd_id 0x0301������ ID:0x0200~0x02FF
	�������� �����˼�ͨ�ţ�0x0301��
	����Ƶ�ʣ����� 10Hz  

	�ֽ�ƫ���� 	��С 	˵�� 			��ע 
	0 			2 		���ݵ����� ID 	0x0200~0x02FF 
										���������� ID ��ѡȡ������ ID �����ɲ������Զ��� 
	
	2 			2 		�����ߵ� ID 	��ҪУ�鷢���ߵ� ID ��ȷ�ԣ� 
	
	4 			2 		�����ߵ� ID 	��ҪУ������ߵ� ID ��ȷ�ԣ�
										���粻�ܷ��͵��жԻ����˵�ID 
	
	6 			n 		���ݶ� 			n ��ҪС�� 113 

*/



/**
  * @brief  ��ȡ��������,loop��ѭ�����ô˺�������ȡ����
  * @param  ��������
  * @retval �Ƿ�������ж�������
  * @attention  �ڴ��ж�֡ͷ��CRCУ��,������д������
  */
bool Judege_read_data(uint8_t  *ReadFromUsart );





typedef struct judge_info_struct 
{
	xFrameHeader										FrameHeader;							// ֡ͷ��Ϣ
	
	game_status_t 									Game_Status;							// 0x0001         ����״̬����
	game_result_t										Game_Result;							// 0x0002         �����������
	game_robot_HP_t 								Game_Robot_HP;						// 0x0003         ������Ѫ������
	
	event_data_t										Event_Data;								// 0x0101         �����¼�����
	supply_projectile_action_t			Supply_Projectile_Action;	// 0x0102 				����վ������ʶ����
	referee_warning_t								Referee_Warning;					// 0x0104         ���о�����Ϣ
	dart_remaining_time_t						Dart_Remaining_Time;			// 0x0105         ���ڷ���ʱ������
	
	robot_status_t									Robot_Status;							// 0x0201         ������������ϵ����
	power_heat_data_t								Power_Heat_Data;					// 0x0202         ʵʱ������������
	robot_pos_t											Robot_Pos;								// 0x0203         ������λ������
	buff_t													Buff;											// 0x0204     		��������������
	air_support_data_t							Air_Support_date;					// 0x0205         ����֧Ԯʱ������
	hurt_data_t											Hurt_Date;								// 0x0206         �˺�״̬����
	shoot_data_t										Shoot_Data;								// 0x0207         ʵʱ�����Ϣ(��Ƶ  ����  �ӵ���Ϣ)
	projectile_allowance_t					Projectile_allowance;			// 0x0208	        ��������
	rfid_status_t										Rfid_Status;							// 0x0209	        ������ RFID ״̬
	
//	ext_interact_id_t								ids;								//�뱾�������Ļ�����id
//	uint16_t                        self_client;        //�����ͻ���
//	bool	 		IF_REF_ONL;

} Referee_info_t;

#endif 


