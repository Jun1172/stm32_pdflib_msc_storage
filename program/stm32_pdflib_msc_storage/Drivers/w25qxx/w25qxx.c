/*
*@ ��;��Flash����
*@ ʱ�䣺20190310
*@ ���ߣ�JUN
*/
#include "w25qxx.h"

SPI_HandleTypeDef hspi1;

uint16_t FlashType = 0;//FLASH����
uint32_t ConfigStartAddr = 0; //�����ļ���FLASH�����λ��

static void HAL_W25QXX_CS_ENABLE(void);
static void HAL_W25QXX_CS_DISABLE(void);
static void HAL_SPI_Send_Byte(uint8_t byte);
static uint8_t HAL_SPI_Receive_Byte(void);
void HAL_W25QXX_Wait_Busy(void);

//����flash�ռ��С
const uint32_t FlashSpace[6] ={
	W25Q80SPACE   ,
	W25Q16SPACE   ,	
	W25Q32SPACE   ,	
	W25Q64SPACE 	,
	W25Q128SPACE	,
	W25Q256SPACE	,
};
/*
 @�������ܣ��ⲿflash��ʼ������
 @�����������
 @�����������
*/
uint8_t W25qxxInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(W25Q64_CS_GPIO_Port, W25Q64_CS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin */
	GPIO_InitStruct.Pin = W25Q64_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(W25Q64_CS_GPIO_Port, &GPIO_InitStruct);

	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		while(1);
	}
	
	return 0;
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;
	if(hspi->Instance==SPI1)
	{
		/* USER CODE BEGIN SPI1_MspInit 0 */

		/* USER CODE END SPI1_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_SPI1_CLK_ENABLE();

		/**SPI1 GPIO Configuration    
		*/
		GPIO_InitStruct.Pin = SPI_CLK_Pin|SPI_MOSI_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(SPI_CLK_GPIO_Port, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = SPI_MISO_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;//
		HAL_GPIO_Init(SPI_MISO_GPIO_Port, &GPIO_InitStruct);
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{

  if(hspi->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();
  
    /**SPI1 GPIO Configuration   */
    HAL_GPIO_DeInit(GPIOA, SPI_CLK_Pin|SPI_MISO_Pin|SPI_MOSI_Pin);
  }
}

static void HAL_W25QXX_CS_ENABLE(void)
{
	HAL_GPIO_WritePin(W25Q64_CS_GPIO_Port,W25Q64_CS_Pin, GPIO_PIN_RESET);
}

static void HAL_W25QXX_CS_DISABLE(void)
{
	HAL_GPIO_WritePin(W25Q64_CS_GPIO_Port,W25Q64_CS_Pin, GPIO_PIN_SET);
}

static void HAL_SPI_Send_Byte(uint8_t byte)
{
	HAL_SPI_Transmit(&hspi1,&byte,1,0xFFFF);
}
static uint8_t HAL_SPI_Receive_Byte(void)
{
	uint8_t data = 0xFF;
	HAL_SPI_Receive(&hspi1,&data,1,0xFFFF);
	return data;
}
//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
//0XEF18,��ʾоƬ�ͺ�ΪW25Q256

uint32_t HAL_W25QXX_ReadID(void)
{
	uint32_t Temp = 0;
	/* ѡ����FLASH: CS�͵�ƽ */
	HAL_W25QXX_CS_ENABLE();
	/* ���������ȡоƬ�ͺ�ID */
	HAL_SPI_Send_Byte(W25X_ManufactDeviceID);
	HAL_SPI_Send_Byte(0x00);
	HAL_SPI_Send_Byte(0x00);
	HAL_SPI_Send_Byte(0x00);
	Temp |= HAL_SPI_Receive_Byte()<<8;
	Temp |= HAL_SPI_Receive_Byte();
	HAL_W25QXX_CS_DISABLE();

  return Temp;
}
//��ȡW25QXX��״̬�Ĵ�����W25QXXһ����3��״̬�Ĵ���
//״̬�Ĵ���1��
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
//״̬�Ĵ���2��
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//״̬�Ĵ���3��
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:״̬�Ĵ����ţ���:1~3
//����ֵ:״̬�Ĵ���ֵ
uint8_t HAL_W25QXX_Read_SR(uint8_t regno)
{
	uint8_t byte=0,command=0; 
	switch(regno)
	{
		case 1:
			command=W25X_ReadStatusReg1;    //��״̬�Ĵ���1ָ��
			break;
		case 2:
			command=W25X_ReadStatusReg2;    //��״̬�Ĵ���2ָ��
			break;
		case 3:
			command=W25X_ReadStatusReg3;    //��״̬�Ĵ���3ָ��
			break;
		default:
			command=W25X_ReadStatusReg1;    
			break;
	}    
	HAL_W25QXX_CS_ENABLE();                            //ʹ������   
	HAL_SPI_Send_Byte(command);            //���Ͷ�ȡ״̬�Ĵ�������    
	byte=HAL_SPI_Receive_Byte();          //��ȡһ���ֽ�  
	HAL_W25QXX_CS_DISABLE();                            //ȡ��Ƭѡ     
	return byte;   
}

//дW25QXX״̬�Ĵ���
void HAL_W25QXX_Write_SR(uint8_t regno,uint8_t sr)   
{   
	uint8_t command=0;
	switch(regno)
	{
		case 1:
			command=W25X_WriteStatusReg1;    //д״̬�Ĵ���1ָ��
			break;
		case 2:
			command=W25X_WriteStatusReg2;    //д״̬�Ĵ���2ָ��
			break;
		case 3:
			command=W25X_WriteStatusReg3;    //д״̬�Ĵ���3ָ��
			break;
		default:
			command=W25X_WriteStatusReg1;    
			break;
	}   
	HAL_W25QXX_CS_ENABLE();                            //ʹ������   
	HAL_SPI_Send_Byte(command);            //����дȡ״̬�Ĵ�������    
	HAL_SPI_Send_Byte(sr);                 //д��һ���ֽ�  
	HAL_W25QXX_CS_DISABLE();                            //ȡ��Ƭѡ     	      
} 
//�ȴ�����
void HAL_W25QXX_Wait_Busy(void)   
{   
	while((HAL_W25QXX_Read_SR(1)&0x01)==0x01);   // �ȴ�BUSYλ���
}
//W25QXXдʹ��	
//��WEL��λ   
void HAL_W25QXX_Write_Enable(void)   
{
	HAL_W25QXX_CS_ENABLE();                            //ʹ������   
    HAL_SPI_Send_Byte(W25X_WriteEnable);  				//����дʹ��  
	HAL_W25QXX_CS_DISABLE();                           //ȡ��Ƭѡ     	      
} 
//W25QXXдʹ��	
//��WEL��λ   
void HAL_W25QXX_Write_Disable(void)   
{
	HAL_W25QXX_CS_ENABLE();                            //ʹ������   
  HAL_SPI_Send_Byte(W25X_WriteDisable);  				  //����дʹ��  
	HAL_W25QXX_CS_DISABLE();                           //ȡ��Ƭѡ     	      
}
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
uint8_t HAL_W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i;   										    
	HAL_W25QXX_CS_ENABLE();                       //ʹ������   
	HAL_SPI_Send_Byte(W25X_ReadData);				//���Ͷ�ȡ����  
	HAL_SPI_Send_Byte((uint8_t)((ReadAddr)>>16)); //����24bit��ַ    
	HAL_SPI_Send_Byte((uint8_t)((ReadAddr)>>8));   
	HAL_SPI_Send_Byte((uint8_t)ReadAddr);   
	for(i=0;i<NumByteToRead;i++)
	{ 
		pBuffer[i]=HAL_SPI_Receive_Byte();    //ѭ������  
	}
	HAL_W25QXX_CS_DISABLE();  		
	return 0;	
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void HAL_W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
  HAL_W25QXX_Write_Enable();                  //SET WEL 
	HAL_W25QXX_CS_ENABLE();                            //ʹ������   
	HAL_SPI_Send_Byte(W25X_PageProgram);   //����дҳ����   
	HAL_SPI_Send_Byte((uint8_t)((WriteAddr)>>16)); //����24bit��ַ    
	HAL_SPI_Send_Byte((uint8_t)((WriteAddr)>>8));   
	HAL_SPI_Send_Byte((uint8_t)WriteAddr);   
	for(i=0;i<NumByteToWrite;i++)
		HAL_SPI_Send_Byte(pBuffer[i]);//ѭ��д��  
	HAL_W25QXX_CS_DISABLE();                            //ȡ��Ƭѡ 
	HAL_W25QXX_Wait_Busy();					   //�ȴ�д�����
} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void HAL_W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)
		pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		HAL_W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)
			break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	
			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)
				pageremain=256; //һ�ο���д��256���ֽ�
			else 
				pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	}    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
uint8_t W25QXX_BUFFER[4096];		 
uint8_t HAL_W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t* W25QXX_BUF;	  
	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		HAL_W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			HAL_W25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			HAL_W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  
		}
		else 
			HAL_W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)
			break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

			pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
			NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	
	return 0;	
}
//��������оƬ		  
//�ȴ�ʱ�䳬��...
void HAL_W25QXX_Erase_Chip(void)   
{                                   
	HAL_W25QXX_Write_Enable();                  //SET WEL 
	HAL_W25QXX_Wait_Busy();   
	HAL_W25QXX_CS_ENABLE();                            //ʹ������   
	HAL_SPI_Send_Byte(W25X_ChipErase);        //����Ƭ��������  
	HAL_W25QXX_CS_DISABLE();                            //ȡ��Ƭѡ     	      
	HAL_W25QXX_Wait_Busy();   				   //�ȴ�оƬ��������
}
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ������������ʱ��:150ms
void HAL_W25QXX_Erase_Sector(uint32_t Dst_Addr)   
{  
	//����falsh�������,������   
 	//printf("fe:%x\r\n",Dst_Addr);	  
	Dst_Addr*=4096;
	HAL_W25QXX_Write_Enable();                  //SET WEL 	 
	HAL_W25QXX_Wait_Busy();   
	HAL_W25QXX_CS_ENABLE();                            //ʹ������   
	HAL_SPI_Send_Byte(W25X_SectorErase);   //������������ָ�� 
	HAL_SPI_Send_Byte((uint8_t)((Dst_Addr)>>16));  //����24bit��ַ    
	HAL_SPI_Send_Byte((uint8_t)((Dst_Addr)>>8));   
	HAL_SPI_Send_Byte((uint8_t)Dst_Addr);  
	HAL_W25QXX_CS_DISABLE();                            //ȡ��Ƭѡ     	      
	HAL_W25QXX_Wait_Busy();   				    //�ȴ��������
} 
//�������ģʽ
void HAL_W25QXX_PowerDown(void)   
{ 
	HAL_W25QXX_CS_ENABLE();                            //ʹ������   
	HAL_SPI_Send_Byte(W25X_PowerDown);     //���͵�������  
	HAL_W25QXX_CS_DISABLE();                            //ȡ��Ƭѡ     	      
	HAL_Delay(1);                            //�ȴ�TPD  
}   
//����
void HAL_W25QXX_WAKEUP(void)   
{  
  	HAL_W25QXX_CS_ENABLE();                                //ʹ������   
    HAL_SPI_Send_Byte(W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB    
	HAL_W25QXX_CS_DISABLE();                               //ȡ��Ƭѡ     	      
    HAL_Delay(1);                                //�ȴ�TRES1 �˴���ʱλ1ms
} 

//��ȡ�ļ���Ϣ

/**
  * @}
  */

