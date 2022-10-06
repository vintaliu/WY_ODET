/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	����1�ſ�����������Գ��� V1
��д�ˣ�	����	
��дʱ�䣺	2017��9��19��
Ӳ��֧�֣�	STM32F103C8   �ⲿ����8MHz RCC����������Ƶ72MHz��  

�޸���־������
1-	
	
							
˵����
 # ��ģ�������STM32F103�ڲ���RCCʱ�����ã������������õδ�ʱ������ʱ������
 # �ɸ����Լ�����Ҫ���ӻ�ɾ����

*********************************************************************************************/
#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "rcc.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "rtc.h"
#include "buzzer.h"
#include "oled0561.h"
#include "lm75a.h"
#include "touch_key.h"
#include "adc.h"
#include "relay.h"
#include "step_motor.h"
#include "encoder.h"
#include "my1690.h"
#include "spi.h"
#include "ch376.h"
#include"filesys.h"
#include "TM1640.h"
#include "rs485.h"
#include "can.h"


extern vu16 ADC_ConvertedValue[4]; //�����ⲿ����
u8 buf[64];

void IO_Init (void){
	GPIO_InitTypeDef  GPIO_InitStructure; //����GPIO�ĳ�ʼ��ö�ٽṹ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //ѡ��˿ں�                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //ѡ��IO�ӿڹ�����ʽ //��������       
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //ѡ��˿ںţ�0~15��all��                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ѡ��IO�ӿڹ�����ʽ       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(GPIOA, &GPIO_InitStructure);
			
}

int main (void){//������
	u8 s,i;
	u8 buf_encoder;
	u8 c=0x01;
	u8 MP3=0;
	u8 MENU;
   	u8 TarName[64];
	u8 Temp_buffer[3]; 
	u8 buff[8];
	u8 x;
	x=0;
	RCC_Configuration(); //ʱ������
	RTC_Config();  //RTC��ʼ��������
	RELAY_Init(); //�̵�����ʼ��
	STEP_MOTOR_Init(); //���������ʼ��
	TM1640_Init(); //TM640��ʼ��
	BUZZER_Init(); //��������ʼ��
	BUZZER_BEEP1(); //����������һ��
	USART1_Init(115200);
	LED_Init();	//LED��ʼ��
	TOUCH_KEY_Init(); //����������ʼ��
	ADC_Configuration(); //ADC��ʼ������
	I2C_Configuration();//I2C��ʼ��
	OLED0561_Init(); //OLED��ʼ��
	//��ʾOLED�ϵĹ̶��ַ�
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(3,"  ALL TEST P1   "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(5,"                "); //��ʾ�ַ���
//-OLED����ʾ���--------------|0123456789012345|------
	MY1690_Init(); //MP3оƬ��ʼ��

	//�̵�������
	LED1(1);
	LED2(1);
	RELAY_Control(3);
	delay_ms(500);
	RELAY_Control(2);
	delay_ms(500);
	RELAY_Control(1);
	delay_ms(500);
	RELAY_Control(0);
	delay_ms(500);


	LED1(0);//LED�ƹ�
	LED2(0);
	//��ʾOLED�ϵĹ̶��ַ�
	OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(2,"A4:0000  A5:0000"); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(4,"A6:0000  A7:0000"); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(6,"  Temp:         "); //��ʾ�ַ���
	MENU=0;
	IO_Init();
	while(1){
		if(MENU==0){//��������������LED����������λ����OLED�����������������
			GPIO_WriteBit(GPIOA,GPIO_Pin_15,(BitAction)(1));//�յ�����LED1��һ��
			delay_ms(100); //��ʱ
			GPIO_WriteBit(GPIOA,GPIO_Pin_15,(BitAction)(0));//


			LED1(KEY1());//�ð������ƣ̣ţ�
			LED2(KEY2());
	
			//LM75A�¶���ʾ
			LM75A_GetTemp(Temp_buffer); //��ȡLM75A���¶�����	
			if(Temp_buffer[0])OLED_DISPLAY_8x16(6,7*8,'-'); //�����1��Ϊ1���Ǹ��¶�
			OLED_DISPLAY_8x16(6,8*8,Temp_buffer[1]/10+0x30);//��ʾ�¶�ֵ
			OLED_DISPLAY_8x16(6,9*8,Temp_buffer[1]%10+0x30);//
			OLED_DISPLAY_8x16(6,10*8,'.');//
			OLED_DISPLAY_8x16(6,11*8,Temp_buffer[2]/10+0x30);//
			OLED_DISPLAY_8x16(6,12*8,Temp_buffer[2]%10+0x30);//
			OLED_DISPLAY_8x16(6,13*8,'C');//
	//		delay_ms(500); //��ʱ
	
			//����λ����ADC������ʾ��OLED��
			OLED_DISPLAY_8x16(2,3*8,ADC_ConvertedValue[0]/1000+0x30);//
			OLED_DISPLAY_8x16(2,4*8,ADC_ConvertedValue[0]%1000/100+0x30);//
			OLED_DISPLAY_8x16(2,5*8,ADC_ConvertedValue[0]%100/10+0x30);//
			OLED_DISPLAY_8x16(2,6*8,ADC_ConvertedValue[0]%10+0x30);//
			//�����������ADC������ʾ��OLED��
			OLED_DISPLAY_8x16(2,12*8,ADC_ConvertedValue[1]/1000+0x30);//
			OLED_DISPLAY_8x16(2,13*8,ADC_ConvertedValue[1]%1000/100+0x30);//
			OLED_DISPLAY_8x16(2,14*8,ADC_ConvertedValue[1]%100/10+0x30);//
			OLED_DISPLAY_8x16(2,15*8,ADC_ConvertedValue[1]%10+0x30);//
			//��ҡ��X���ADC������ʾ��OLED��
			OLED_DISPLAY_8x16(4,3*8,ADC_ConvertedValue[2]/1000+0x30);//
			OLED_DISPLAY_8x16(4,4*8,ADC_ConvertedValue[2]%1000/100+0x30);//
			OLED_DISPLAY_8x16(4,5*8,ADC_ConvertedValue[2]%100/10+0x30);//
			OLED_DISPLAY_8x16(4,6*8,ADC_ConvertedValue[2]%10+0x30);//
			//��ҡ��Y���ADC������ʾ��OLED��
			OLED_DISPLAY_8x16(4,12*8,ADC_ConvertedValue[3]/1000+0x30);//
			OLED_DISPLAY_8x16(4,13*8,ADC_ConvertedValue[3]%1000/100+0x30);//
			OLED_DISPLAY_8x16(4,14*8,ADC_ConvertedValue[3]%100/10+0x30);//
			OLED_DISPLAY_8x16(4,15*8,ADC_ConvertedValue[3]%10+0x30);//

			if(RTC_Get()==0){ //����RTCʱ��
				TM1640_display(0,rday/10);	//��
				TM1640_display(1,rday%10+10);
				TM1640_display(2,rhour/10); //ʱ
				TM1640_display(3,rhour%10+10);
				TM1640_display(4,rmin/10);	//��
				TM1640_display(5,rmin%10+10);
				TM1640_display(6,rsec/10); //��
				TM1640_display(7,rsec%10);
	
				TM1640_led(c); //��TM1640���ӵ�8��LEDȫ��
				c<<=1; //�������� ��ˮ��
				if(c==0x00)c=0x01; //8������ʾ������¿�ʼ
				//delay_ms(125); //��ʱ
			}
			if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2))OLED_DISPLAY_8x16(0,0,'Y');//

			if(TOUCH_KEY_READ()==0x01)OLED_DISPLAY_8x16(0,0,'A');//
			if(TOUCH_KEY_READ()==0x02)OLED_DISPLAY_8x16(0,0,'B');//
			if(TOUCH_KEY_READ()==0x04)OLED_DISPLAY_8x16(0,0,'C');//
			if(TOUCH_KEY_READ()==0x08)MENU=2;
	   }

	   if(MENU==2){	//
			ENCODER_Init();	//��ת��������ʼ��
	   		BUZZER_BEEP1(); //����������һ��
			//��ʾOLED�ϵĹ̶��ַ�
			OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(2," TEST: encoder  "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(4,"   MP3 PLAY     "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(6,"    D:NEXT      "); //��ʾ�ַ���
			MENU=3;
	   }
	   if(MENU==3){	//MP3 ����ת������
			if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0    //�ж�4�������Ƿ���
				||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0 
				||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)==0 
				||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)==0){
				delay_ms(20); //��ʱ
				if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0){	//4��������A��һ����B��һ����
					MY1690_PREV(); //��һ��
					OLED_DISPLAY_8x16_BUFFER(6,"  -- PREV --    "); //��ʾ�ַ���
					delay_ms(500); //��ʱ
					OLED_DISPLAY_8x16_BUFFER(6,"  -- PLAY --    "); //��ʾ�ַ���
				}
				if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0){
					MY1690_NEXT(); //��һ��
					OLED_DISPLAY_8x16_BUFFER(6,"  -- NEXT --    "); //��ʾ�ַ���
					delay_ms(500); //��ʱ
					OLED_DISPLAY_8x16_BUFFER(6,"  -- PLAY --    "); //��ʾ�ַ���
				}
				while(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0	//�ȴ������ſ�
				||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0 
				||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_C)==0 
				||GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_D)==0);
			}
			buf_encoder=ENCODER_READ();	//������ת����������ֵ	
			if(buf_encoder==1){MY1690_VUP();} //��ת�����ӡ�
			if(buf_encoder==2){MY1690_VDOWN();}//��ת��������
			if(buf_encoder==3){	//���²��Ż���ͣ
				if(MP3==0){	//�жϵ�ǰ�ǲ��Ż�����ͣ
					MP3=1;MY1690_PLAY();
					OLED_DISPLAY_8x16_BUFFER(6,"  -- PLAY --    "); //��ʾ�ַ���
				}else if(MP3==1){
					MP3=0;MY1690_PAUSE();
					OLED_DISPLAY_8x16_BUFFER(6,"  -- PAUSE --   "); //��ʾ�ַ���
				}
			}
			if(USART3_RX_STA==1){ //�����־λ��1 ��ʾ�յ�STOP
				MP3=0;
				OLED_DISPLAY_8x16_BUFFER(6,"  -- STOP --    "); //��ʾ�ַ���
				USART3_RX_STA=0; //���������ݱ�־λ��0
			}else if(USART3_RX_STA==2){	//�����־λ��1 ��ʾ�յ�OK
				//������صĴ������
				USART3_RX_STA=0; //���������ݱ�־λ��0
			}
			if(TOUCH_KEY_READ()==0x08)MENU=4;
	   }
	   if(MENU==4){	//�������
	   		BUZZER_BEEP1(); //����������һ��
			//��ʾOLED�ϵĹ̶��ַ�
			OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(2,"     TEST:      "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(4,"   step_motor   "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(6,"    A:NEXT      "); //��ʾ�ַ���
			MENU=5;
	   }
	   if(MENU==5){	//�������
			MOTOLOOP_8_R(2); //�����������ת��
			if(TOUCH_KEY_READ()==0x01)MENU=6;
	   }
	   if(MENU==6){	//RS485 CAN
	   		BUZZER_BEEP1(); //����������һ��
			//��ʾOLED�ϵĹ̶��ַ�
			OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(2,"A:RS232/485     "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(4,"B:CAN           "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(6,"    D:NEXT      "); //��ʾ�ַ���
			USART3_RX_STA=0;
			CAN1_Configuration(); //CAN���߳�ʼ�� ����0��ʾ�ɹ�
			USART3_Init(115200);//����3��ʼ��������
			USART3_RX_STA=0xC000; //��ʼֵ��Ϊ�лس���״̬������ʾһ�λ�ӭ��
			RS485_Init();//RS485���߳�ʼ������Ҫ����USART3��ʼ���·�
			MENU=7;
			s=1;i=1;

	   }
	   if(MENU==7){	//RS485 CAN
			if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0){    //
				RS485_printf("%x",i);
				i++;
				LED1(1);//��������LED1��һ��
				delay_ms(100); //��ʱ
				LED1(0);
				while(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_A)==0);
			}
			if(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0){    //
				buff[0]=s;
				CAN_Send_Msg(buff,1);//CAN���ݷ��Ͳ��������飬�����������8������
				s++;
				LED1(1);//��������LED1��һ��
				delay_ms(100); //��ʱ
				LED1(0);
				while(GPIO_ReadInputDataBit(TOUCH_KEYPORT,TOUCH_KEY_B)==0);
			}

			//CAN��Ѱ��ʽ�Ľ��մ���
			x = CAN_Receive_Msg(buff); //����Ƿ��յ�����
			if(x){ //�жϽ������ݵ���������Ϊ0��ʾ���յ�����
				OLED_DISPLAY_8x16(4,9*8,buff[0]/10+0x30);//��ʾ	
				OLED_DISPLAY_8x16(4,10*8,buff[0]%10+0x30);//��ʾ
			}

			if(USART3_RX_STA){
				OLED_DISPLAY_8x16(2,13*8,USART3_RX_STA/10+0x30);//��ʾ	
				OLED_DISPLAY_8x16(2,14*8,USART3_RX_STA%10+0x30);//��ʾ
				USART3_RX_STA=0;
			}
			if(TOUCH_KEY_READ()==0x08)MENU=8;
	   }
	   if(MENU==8){	//����CH376��U�̽ӿڣ�
	   		BUZZER_BEEP1(); //����������һ��
			//��ʾOLED�ϵĹ̶��ַ�
			OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(2,"                "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(4,"  U DISK TEST   "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(6,"    PUT IN!!    "); //��ʾ�ַ���
			SPI2_Init();//SPI�ӿڳ�ʼ��
			printf( "Start\r\n" );
			delay_ms(100);//��ʱ100����
			s = mInitCH376Host();//CH376��ʼ��		
		////////////////////////////////////////////////////////////////
		 	printf("i=%02x \r\n",(unsigned short)s);
		    printf( "Wait Disk\r\n" );
		    while ( CH376DiskConnect( ) != USB_INT_SUCCESS ){/* ���U���Ƿ�����,�ȴ�U�̲���,����SD��,�����ɵ�Ƭ��ֱ�Ӳ�ѯSD�����Ĳ��״̬���� */
		      		delay_ms( 100 );
		    }
		   	delay_ms( 200 );  // ���ڼ�⵽USB�豸��,���ȴ�100*50mS,��Ҫ�����ЩMP3̫��,���ڼ�⵽USB�豸
		                           //��������DISK_MOUNTED��,���ȴ�5*50mS,��Ҫ���DiskReady������  
		         for ( i = 0; i < 100; i ++ ) 
		            {   
		               delay_ms( 50 );
		               printf( "Ready ?\r\n" );
		               s = CH376DiskMount( );  //��ʼ�����̲����Դ����Ƿ����.   
		               if ( s == USB_INT_SUCCESS ) /* ׼���� */
		                  break;                                          
		               else if ( s == ERR_DISK_DISCON )/* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
		                  break;  
		               if ( CH376GetDiskStatus( ) >= DEF_DISK_MOUNTED && i >= 5 ) /* �е�U�����Ƿ���δ׼����,�������Ժ���,ֻҪ�佨������MOUNTED�ҳ���5*50mS */
		                  break; 
		            }          
		         if ( s == ERR_DISK_DISCON ) /* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
		            {  
		               printf( "Device gone\r\n" );
		               //continue;
		            }		
		         if ( CH376GetDiskStatus( ) < DEF_DISK_MOUNTED ) /* δ֪USB�豸,����USB���̡���ӡ���� */
		            {  
		                printf( "Unknown device\r\n" );
		            }
		/////////////////////////////////////��ȡ������Ϣ
		         i = CH376ReadBlock( buf );  /* �����Ҫ,���Զ�ȡ���ݿ�CH376_CMD_DATA.DiskMountInq,���س��� */
		            if ( i == sizeof( INQUIRY_DATA ) )  /* U�̵ĳ��̺Ͳ�Ʒ��Ϣ */
		            {  
		                  buf[ i ] = 0;
		                  printf( "UdiskInfo: %s\r\n", ((P_INQUIRY_DATA)buf) -> VendorIdStr );
		            }
				printf( "DiskQuery: " );		/* ���U�̻���SD����ʣ��ռ� */
				s = CH376DiskQuery( (PUINT32)buf );	/* ��ѯ����ʣ��ռ���Ϣ,������ */
				printf("s=%02x \r\n",(unsigned short)s );
				printf( "free cap = %ld MB\n", *(PUINT32)buf / ( 1000000 / DEF_SECTOR_SIZE ) );
		/////////////////////////////////////  ����
				printf( "Create file :" );
				strcpy( (char *)TarName, "\\YoungTalk.TXT" ); /* Ŀ���ļ��� */
		      	s = CH376FileCreatePath( TarName );  	/* �½��༶Ŀ¼�µ��ļ�,֧�ֶ༶Ŀ¼·��,���뻺����������RAM�� */
		 		printf("s=%02x \r\n",(unsigned short)s );
		////////////////////////////////////// д��
		      	printf( "Write ��" );
		      	strcpy((char *)buf, "  OK  " );
				s = CH376ByteWrite( buf, strlen((const char *)buf), NULL ); /* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
		 		printf("s=%02x \n",(unsigned short)s );
		
		 		printf("close file " ); 
		 		s = CH376FileClose( TRUE );   /* �ر��ļ�,�����ֽڶ�д�����Զ������ļ����� */
		 		printf("s=%02x \r\n",(unsigned short)s );
		
				printf( "Take out\n" );
				while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) {  /* ���U���Ƿ�����,�ȴ�U�̰γ� */
					delay_ms( 100 );
				}
			BUZZER_BEEP1(); //����������һ��
			//��ʾOLED�ϵĹ̶��ַ�
			OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(2,"                "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(4,"   U DISK OK!   "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(6,"                "); //��ʾ�ַ���
			MENU=9;
	   }
	   if(MENU==9){	//
			//��ʾOLED�ϵĹ̶��ַ�
			OLED_DISPLAY_8x16_BUFFER(0,"   YoungTalk    "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(2,"     TEST:      "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(4,"  RESET Button  "); //��ʾ�ַ���
			OLED_DISPLAY_8x16_BUFFER(6,"                "); //��ʾ�ַ���
	   		BUZZER_BEEP1(); //����������һ��
	   		BUZZER_BEEP1(); //����������һ��
	   		BUZZER_BEEP1(); //����������һ��
			
	   }
	}
}


/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/
/*

���������塿
u32     a; //����32λ�޷��ű���a
u16     a; //����16λ�޷��ű���a
u8     a; //����8λ�޷��ű���a
vu32     a; //�����ױ��32λ�޷��ű���a
vu16     a; //�����ױ�� 16λ�޷��ű���a
vu8     a; //�����ױ�� 8λ�޷��ű���a
uc32     a; //����ֻ����32λ�޷��ű���a
uc16     a; //����ֻ�� ��16λ�޷��ű���a
uc8     a; //����ֻ�� ��8λ�޷��ű���a

#define ONE  1   //�궨��

delay_us(1); //��ʱ1΢��
delay_ms(1); //��ʱ1����
delay_s(1); //��ʱ1��

*/



