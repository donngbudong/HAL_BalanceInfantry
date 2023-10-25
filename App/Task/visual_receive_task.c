//#include "stm32f4xx.h"                  
//#include "usart.h"
//#include "visual_receive_task.h"
//#include "referee.h"
//extern float Eular[3];
//extern uint8_t visual_mode;								//视觉模式（普通|哨兵）
//extern uint8_t Visual_Date_t;							//视觉是否识别

//uint8_t  Visual_Buffer[VISAUAL_DATE_LEN];	//数据类型长度
//Visual_data From_Vision_Data;
//Visual_data Visual_Data;
//float  Vision_Coff=1;


//void visual_date(uint8_t *data)
//{
//	 if(*data==0x70 && (*(data+12))==0x6F)
// {
//	 visual_rc visual_pitch;
//	 visual_rc visual_yaw;

//	 for(int i=0;i<4;i++)
//	 {
//		 visual_yaw.date_t[i]=*(data+1+i);
//	 }
//	 
//	 	 for(int i=0;i<4;i++)
//	 {
//		 visual_pitch.date_t[i]=*(data+5+i);
//	 }

//	From_Vision_Data.distance[0]=*(data+9);
//	From_Vision_Data.distance[1]=*(data+10);
//	From_Vision_Data.distance[2]=From_Vision_Data.distance[0]*100+From_Vision_Data.distance[1];
//	From_Vision_Data.Visual_State=*(data+11);//状态  默认0		1自启成功无数据		2有数据
//	From_Vision_Data.Vision_Pitch = visual_pitch.date*Vision_Coff;
//	From_Vision_Data.Vision_Yaw  =visual_yaw.date*Vision_Coff; 

////Yaw
//		 if(From_Vision_Data.Vision_Yaw<-10)
//		{
//			Visual_Data.Vision_Yaw=-10;
//		}
//		else if(From_Vision_Data.Vision_Yaw>10)
//				{
//			Visual_Data.Vision_Pitch=10;
//		}
//		else
//		{
//			Visual_Data.Vision_Yaw=From_Vision_Data.Vision_Yaw;
//		}
////Pitch
//		if(From_Vision_Data.Vision_Pitch<-10)
//		{
//			Visual_Data.Vision_Pitch=-10;
//		}
//		else if(From_Vision_Data.Vision_Pitch>10)
//				{
//			Visual_Data.Vision_Pitch=10;
//		}
//		else
//		{
//			Visual_Data.Vision_Pitch=From_Vision_Data.Vision_Pitch;
//		}
//		
// }
//}

//extern Referee_info_t REF;

//visual_send send[3];
//void visual_send_date(void)
//{
//	uint8_t state;
//	float speed;
//	uint16_t i;
//	send[0].date=Eular[0];
//	send[1].date=Eular[2];
//	if(REF.GameRobotStat.robot_id < 10){
//		state=BLUE;//0//BLUR
//	}
//	else{
//		state=RED;//1//RED
//	}
//		
//	speed=(uint8_t)(REF_FOOT_Shoot_Speed()*10);//实时弹速
//	uint8_t exe[13] = {0x80,state,speed,
//								send[0].date_t[0],send[0].date_t[1],send[0].date_t[2],send[0].date_t[3],
//								send[1].date_t[0],send[1].date_t[1],send[1].date_t[2],send[1].date_t[3],
//										visual_mode,0x7f}; 
////百位 1正常   2 哨兵   十位 1 普通   2 预测    个位 1  15m/s    2  30m/s 
//	for(i = 0; i<13; i++)
//	{
//		HAL_UART_Transmit(&huart4,&exe[i],1,100);
//	}
//}

