#include "referee.h"

// ����ϵͳ��Ϣ
Referee_info_t 	REF ;

bool Judge_Data_TF = FALSE;//���������Ƿ����,������������
uint8_t Judge_Self_ID;//��ǰ�����˵�ID
uint16_t Judge_SelfClient_ID;//�����߻����˶�Ӧ�Ŀͻ���ID

/**************����ϵͳ���ݸ���****************/
uint16_t ShootNum;//ͳ�Ʒ�����,0x0003����һ������Ϊ������һ��
bool Hurt_Data_Update = FALSE;//װ�װ��˺������Ƿ����,ÿ��һ���˺���TRUE,Ȼ��������FALSE,������������


/**
  * @brief  ��ȡ��������,loop��ѭ�����ô˺�������ȡ����
  * @param  ��������
  * @retval �Ƿ�������ж�������
  * @attention  �ڴ��ж�֡ͷ��CRCУ��,������д������
  */
bool Judege_read_data(uint8_t *ReadFromUsart )
{
	bool retval_tf = FALSE;//������ȷ����־,ÿ�ε��ö�ȡ����ϵͳ���ݺ�������Ĭ��Ϊ����
	
	uint16_t judge_length;//ͳ��һ֡���ݳ��� 
	
	int CmdID = 0;//�������������	
	
	if(ReadFromUsart == NULL)
	{
		return -1;
	}
	
	memcpy(&REF.FrameHeader,ReadFromUsart,LEN_HEADER);   //����֡ͷ����
	
	if(ReadFromUsart[SOF] == JUDGE_FRAME_HEADER)                   //�ж�֡ͷ�Ƿ�Ϊ0xa5
	{
		if(Verify_CRC8_Check_Sum( ReadFromUsart, LEN_HEADER ) == TRUE)  //֡ͷCRC8У��
		{
			judge_length = ReadFromUsart[DATA_LENGTH] + LEN_HEADER + LEN_CMDID + LEN_TAIL;	//ͳ��һ֡���ݳ���,����CR16У��
			
			if(Verify_CRC16_Check_Sum(ReadFromUsart,judge_length) == TRUE)//֡βCRC16У��
			{
				retval_tf = TRUE;//���ݿ���
				
				CmdID = (ReadFromUsart[6] << 8 | ReadFromUsart[5]);//��������������,�����ݿ�������Ӧ�ṹ����(ע�⿽�����ݵĳ���)
				
				switch(CmdID)
				{
					case ID_game_status:     //0x0001
							 memcpy(&REF.Game_Status, (ReadFromUsart + DATA), LEN_game_status);
							 break;
					
					case ID_game_result:    //0x0002
							 memcpy(&REF.Game_Result, (ReadFromUsart + DATA), LEN_game_result);
							 break;
					
					case ID_game_robot_HP:    //0x0003
							 memcpy(&REF.Game_Robot_HP, (ReadFromUsart + DATA), LEN_game_robot_HP);
							 break;
			
					case ID_event_data:    //0x0101
							 memcpy(&REF.Event_Data, (ReadFromUsart + DATA), LEN_event_data);
							 break;
					
					case ID_supply_projectile_action:    //0x0102
							 memcpy(&REF.Supply_Projectile_Action, (ReadFromUsart + DATA), LEN_supply_projectile_action);
							 break;
					
					case ID_referee_warning:    //0x0104
							 memcpy(&REF.Referee_Warning, (ReadFromUsart + DATA), LEN_referee_warning);
							 break;
					
					case ID_dart_remaining_time:    //0x0105
							 memcpy(&REF.Dart_Remaining_Time, (ReadFromUsart + DATA), LEN_dart_remaining_time);
							 break;
					
					case ID_robot_status:    //0x0201
//               Determine_ID();
///*1*/          Referee_Update(GameRobotStat_ID);
							 memcpy(&REF.Robot_Status, (ReadFromUsart + DATA), LEN_robot_status);
							 break;
				
					case ID_power_heat_data:    //0x0202
///*2*/          Referee_Update(PowerHeatData_ID);
							 memcpy(&REF.Power_Heat_Data, (ReadFromUsart + DATA), LEN_power_heat_data);
							 break;
					
					case ID_robot_pos:    //0x0203
							 memcpy(&REF.Robot_Pos, (ReadFromUsart + DATA), LEN_robot_pos);
							 break;
					
					case ID_buff:    //0x0204
							 memcpy(&REF.Buff, (ReadFromUsart + DATA), LEN_buff);
							 break;
					
					case ID_air_support_data:    //0x0205
							 memcpy(&REF.Air_Support_date, (ReadFromUsart + DATA), LEN_air_support_data);
							 break;
					
					case ID_hurt_data:      			//0x0206
							memcpy(&REF.Hurt_Date, (ReadFromUsart + DATA), LEN_hurt_data);
//							if(REF.RobotHurt.hurt_type == 0)//��װ�װ���������˺�
//							{	
//								Hurt_Data_Update = TRUE;
//							}//װ������ÿ����һ�����ж�Ϊ�ܵ�һ���˺�
							break;
					case ID_shoot_data:      			//0x0207
///*3*/          Referee_Update(ShootSpeed_ID);
							 memcpy(&REF.Shoot_Data, (ReadFromUsart + DATA), LEN_shoot_data);
					     break;	
					
					case ID_projectile_allowance:    //0x0208
							 memcpy(&REF.Projectile_allowance, (ReadFromUsart + DATA), LEN_projectile_allowance);
							 break;
          
					case ID_rfid_status: //0x0209
               memcpy(&REF.Rfid_Status, (ReadFromUsart+DATA), LEN_rfid_status);
               break;
	
				}
					
			}
		}
		//�׵�ַ��֡����,ָ��CRC16��һ�ֽ�,�����ж��Ƿ�Ϊ0xA5,�����ж�һ�����ݰ��Ƿ��ж�֡����
		if(*(ReadFromUsart + sizeof(xFrameHeader) + LEN_CMDID + REF.FrameHeader.DataLength + LEN_TAIL) == 0xA5)
		{
			//���һ�����ݰ������˶�֡����,���ٴζ�ȡ
			Judege_read_data(ReadFromUsart + sizeof(xFrameHeader) + LEN_CMDID + REF.FrameHeader.DataLength + LEN_TAIL);
		}
	}
	if (retval_tf == TRUE)
	{
		Judge_Data_TF = TRUE;//����������
	}
	else		//ֻҪCRC16У�鲻ͨ����ΪFALSE
	{
		Judge_Data_TF = FALSE;//����������
	}	
	return retval_tf;
}


/***************************����ϵͳ���ڳ�ʼ��***********************************/

//extern uint32_t Refer_time ;//����ϵͳ


/**
  * @brief  ����һ�η���һ���ֽ�����
  * @param  �Լ�����õ�Ҫ�������е�����
  * @retval void
  * @attention  ������λ����
  */
//void UART5_SendChar(uint8_t cData)
//{
//	while (USART_GetFlagStatus( UART5, USART_FLAG_TC ) == RESET);
//	
//	USART_SendData( UART5, cData );   
//}

///**
// * @brief �жϲ���ϵͳ�Ƿ�����
// * @param 
// */
//bool Judge_IF_Refer_Normal(void)
//{
//  bool res = true;
//  if(micros() >= Refer_time)
//  {
//    res = false;
//  }
//  return res;
//}

//void Judge_IF_REF_ONL(void)
//{
//  REF.IF_REF_ONL = Judge_IF_Refer_Normal();
//}


////------------PowerHeatData------------//
//float REF_Report_CHAS_Power(void)   //4
//{
//  return REF.PowerHeatData.chassis_power;
//}

//uint16_t REF_Report_CHAS_PowerBuffer(void)   //2
//{
//  return REF.PowerHeatData.chassis_power_buffer;
//}  

//uint16_t REF_Report_Shooter_Heat(void)    //2
//{
//  return REF.PowerHeatData.shooter_heat0;
//}
////------------ShootInfo----------------//
//float REF_Report_RealShootSpeed(void)
//{
//  return REF.ShootData.bullet_speed;
//}
//uint8_t REF_Report_IF_shooter_output(void)
//{
//  return REF.GameRobotStat.mains_power_shooter_output;
//}

////------------GameRobotStat------------//
//uint8_t REF_Report_robot_ID(void)   //1
//{
//  return REF.GameRobotStat.robot_id;
//}

//uint8_t REF_Report_Shoot_SpeedLimit(void)  //1
//{
//  return REF.GameRobotStat.shooter1_17mm_speed_limit;
//}

//uint16_t REF_Report_Shoot_CoolingLimit(void)  //2
//{
//  return REF.GameRobotStat.shooter1_17mm_cooling_limit;
//}

//uint16_t REF_Report_Shoot_CoolingRate(void)  //2
//{
//  return REF.GameRobotStat.shooter1_17mm_cooling_rate;
//}

//uint16_t REF_Report_CHAS_MaxPower(void)  //2
//{
//  return REF.GameRobotStat.max_chassis_power;
//}
////------------------RFID-------------------------//
//uint8_t REF_Report_RFID_State(void)
//{
//  uint8_t res = 0;
//  res = res | REF.rfid_status.rfid_status;
//  return res;
////  return (uint8_t)REF.rfid_status.rfid_status;
//}


////------------------------------------------------------------------------


//void Determine_ID(void)//�ж��Լ����ĸ�����
//{
//	if(REF.GameRobotStat.robot_id < 10)//�������˵�ID���췽
//	{ 
//		REF.ids.teammate_hero 		 	= 1;
//		REF.ids.teammate_engineer  = 2;
//		REF.ids.teammate_infantry3 = 3;
//		REF.ids.teammate_infantry4 = 4;
//		REF.ids.teammate_infantry5 = 5;
//		REF.ids.teammate_plane		 	= 6;
//		REF.ids.teammate_sentry		= 7;
//		
//		REF.ids.client_hero 		 	= 0x0101;
//		REF.ids.client_engineer  = 0x0102;
//		REF.ids.client_infantry3 = 0x0103;
//		REF.ids.client_infantry4 = 0x0104;
//		REF.ids.client_infantry5 = 0x0105;
//		REF.ids.client_plane			= 0x0106;
//		
//		if     (REF.GameRobotStat.robot_id == hero_red)//����ˢ�·����ڱ����и�����ɫ
//			REF.self_client = REF.ids.client_hero;
//		else if(REF.GameRobotStat.robot_id == engineer_red)
//			REF.self_client = REF.ids.client_engineer;
//		else if(REF.GameRobotStat.robot_id == infantry3_red)
//			REF.self_client = REF.ids.client_infantry3;
//		else if(REF.GameRobotStat.robot_id == infantry4_red)
//			REF.self_client = REF.ids.client_infantry4;
//		else if(REF.GameRobotStat.robot_id == infantry5_red)
//			REF.self_client = REF.ids.client_infantry5;
//		else if(REF.GameRobotStat.robot_id == plane_red)
//			REF.self_client = REF.ids.client_plane;
//	}
//	else //����
//	{
//		REF.ids.teammate_hero 		 	= 101;
//		REF.ids.teammate_engineer  = 102;
//		REF.ids.teammate_infantry3 = 103;
//		REF.ids.teammate_infantry4 = 104;
//		REF.ids.teammate_infantry5 = 105;
//		REF.ids.teammate_plane		 	= 106;
//		REF.ids.teammate_sentry		= 107;
//		
//		REF.ids.client_hero 		 	= 0x0165;
//		REF.ids.client_engineer  = 0x0166;
//		REF.ids.client_infantry3 = 0x0167;
//		REF.ids.client_infantry4 = 0x0168;
//		REF.ids.client_infantry5 = 0x0169;
//		REF.ids.client_plane			= 0x016A;
//		
//		if     (REF.GameRobotStat.robot_id == hero_blue)
//			REF.self_client = REF.ids.client_hero;
//		else if(REF.GameRobotStat.robot_id == engineer_blue)
//			REF.self_client = REF.ids.client_engineer;
//		else if(REF.GameRobotStat.robot_id == infantry3_blue)
//			REF.self_client = REF.ids.client_infantry3;
//		else if(REF.GameRobotStat.robot_id == infantry4_blue)
//			REF.self_client = REF.ids.client_infantry4;
//		else if(REF.GameRobotStat.robot_id == infantry5_blue)
//			REF.self_client = REF.ids.client_infantry5;
//		else if(REF.GameRobotStat.robot_id == plane_blue)
//			REF.self_client = REF.ids.client_plane;
//		
//	}
//}

