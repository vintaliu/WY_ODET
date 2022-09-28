  /******************************************************************************
  * @file    PT32X005_i2c.c
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/10/19
  * @brief    This file provides firmware functions to manage the following
  *          functionalities of the Inter-Integrated circuit (I2C):
  *           + Initialization and Configuration
  *           + Communications handling
  *           + I2C registers management
  *           + Data transfers management
  *           + Interrupts and flags management
  *          
  ******************************************************************************
  * @attention
  *
  *
  *****************************************************************************/
  
/* Includes ------------------------------------------------------------------*/
#include "PT32X005_i2c_eeprom.h"

/** @defgroup I2C
  * @brief I2C driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_BUFSIZE         9
#define BUFSIZE             8			//slave
#define MAX_TIMEOUT         0x002FFFFF

#define I2CSTATE_IDLE       0x000
#define I2CSTATE_PENDING    0x001
#define I2CSTATE_ACK        0x101
#define I2CSTATE_NACK       0x102
#define I2CSTATE_SLA_NACK   0x103
#define I2CSTATE_ARB_LOSS   0x104

#define I2C_IDLE            0
#define I2C_STARTED         1
#define I2C_RESTARTED       2
#define I2C_REPEATED_START  3
#define DATA_ACK            4
#define DATA_NACK           5
#define I2C_WR_STARTED      6
#define I2C_RD_STARTED      7
#define I2C_BROADCASTED     8
#define I2C_START_FAIL      0xFFFFFFFF

#define SLAVE_ADDR          0x00
#define READ_WRITE          0x01
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*
volatile uint32_t _I2cMode;                       // I2CMASTER or I2CSLAVE

volatile uint32_t I2CMasterState = I2CSTATE_IDLE;
volatile uint32_t I2CSlaveState = I2CSTATE_IDLE;

volatile uint8_t I2CMasterBuffer[I2C_BUFSIZE];    // Master Mode
volatile uint8_t I2CSlaveBuffer[I2C_BUFSIZE];     // Master Mode

volatile uint8_t I2CWrBuffer[I2C_BUFSIZE];        // Slave Mode
volatile uint8_t I2CRdBuffer[I2C_BUFSIZE];        // Slave Mode

volatile uint32_t I2CReadLength;
volatile uint32_t I2CWriteLength;

volatile uint32_t RdIndex;
volatile uint32_t WrIndex;
*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Initializes the I2Cx peripheral according to the specified
  *         parameters in the I2C_InitStruct.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  I2C_InitStruct: pointer to a I2C_InitTypeDef structure that
  *         contains the configuration information for the specified I2C peripheral.
  * @retval None
  */
void I2C_Init(CMSDK_I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct)
{
	uint32_t tmpreg = 0;
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_I2C_OWN_ADDRESS(I2C_InitStruct->I2C_OwnAddress));
	assert_param(IS_I2C_ACK(I2C_InitStruct->I2C_Ack));
	assert_param(IS_SCLK_DIV(I2C_InitStruct->I2C_SCLK_DIV));
	assert_param(IS_I2C_BROADCAST(I2C_InitStruct->I2C_BroadCast));
	/* Disable I2Cx Peripheral */
	I2Cx->CCR = I2C_CCR_MASK;
	/*---------------------------- I2Cx BroadCast Configuration ---------------------*/
	/* ENABLE I2Cx BroadCast */
	if((I2C_InitStruct->I2C_BroadCast) != DISABLE)
	{
		I2Cx->OAR |= I2C_OAR_BCSTEN;
	}
	/* Disable I2Cx BroadCast */
	else
	{
		I2Cx->OAR &= (uint32_t)~((uint32_t)I2C_OAR_BCSTEN);
	}
	/*---------------------------- I2Cx OAR Configuration ---------------------*/
	/* Clear OAR register */
	I2Cx->OAR &= (uint32_t)~I2C_OAR_ADDR;
	/* Configure I2Cx: Own Address */
	tmpreg = (uint32_t)((I2C_InitStruct->I2C_OwnAddress) << 0x01);
	/* Write to I2Cx OAR */
	I2Cx->OAR = tmpreg;
	/*---------------------------- I2Cx ACK Configuration ----------------------*/
	/* ENABLE I2Cx ACK */
	if((I2C_InitStruct->I2C_Ack) == ENABLE)
	{
		I2Cx->CSR = I2C_Ack_Enable;
	}
	/* Disable I2Cx ACK */
	else
	{
		I2Cx->CCR = I2C_Ack_Disable;
	}
	/*---------------------------- I2Cx Frequency division factor Configuration  ----------------------*/
	/* Clear CR register CR bit */
//	I2Cx->CCR = I2C_CCR_CR;
	/* Configure I2Cx: CR[2:0] */
	I2Cx->CSR	= (I2C_InitStruct->I2C_SCLK_DIV << 16);
}

/**
  * @brief  Fills each I2C_InitStruct member with its default value.
  * @param  I2C_InitStruct: pointer to an I2C_InitTypeDef structure which will be initialized.
  * @retval None
  */
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
{
	/*---------------- Reset I2C init structure parameters values --------------*/
	/* Initialize the I2C_OwnAddress1 member */
	I2C_InitStruct->I2C_OwnAddress = 0;
	/* Initialize the I2C_Ack member */
	I2C_InitStruct->I2C_Ack = DISABLE;
	/* Initialize the  Frequency division factor when pclk=24000000 member SCLK=100000 */
	I2C_InitStruct->I2C_SCLK_DIV = I2C_SCLK_DIVX192;
}

/**
  * @brief  Enables or disables the specified I2C peripheral.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2Cx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_Cmd(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable the selected I2C peripheral */
		I2Cx->CSR = I2C_CSR_EN;
	}
	else
	{
		/* Disable the selected I2C peripheral */
		I2Cx->CCR = I2C_CCR_EN;
	}
}


/**
  * @brief  Enables or disables the I2C general call mode.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C general call mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_GeneralCallCmd(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable general call mode */
		I2Cx->OAR |= I2C_OAR_BCSTEN;
	}
	else
	{
		/* Disable general call mode */
		I2Cx->OAR &= (uint32_t)~((uint32_t)I2C_OAR_BCSTEN);
	}
}


/**
  * @brief  Configures the slave address to be transmitted after start generation.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  Address: specifies the slave address to be programmed.
  * @note   This function should be called before generating start condition.
  * @retval None
  */
void I2C_SlaveAddressConfig(CMSDK_I2C_TypeDef* I2Cx, uint16_t Address)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_I2C_SLAVE_ADDRESS(Address));
	/* Set I2Cx DR */
	I2Cx->DR = (uint32_t)Address;
	/* Send Address */
	I2Cx->CCR = I2C_CCR_IS | I2C_CCR_START ;
}


/**
  * @brief  Generates I2Cx communication START condition.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C START condition generation.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_GenerateSTART(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Generate a START condition */
		I2Cx->CCR = I2C_CCR_IS;
		I2Cx->CSR = I2C_CSR_START;
	}
	else
	{
		/* Disable the START condition generation */
		//I2Cx->CCR = I2C_CCR_IS | I2C_CCR_START;
		I2Cx->CCR = I2C_CCR_START;
	}
}

/**
  * @brief  Generates I2Cx communication STOP condition.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C STOP condition generation.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_GenerateSTOP(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Generate a STOP condition */
		I2Cx->CCR = I2C_CCR_IS;
		I2Cx->CSR = I2C_CSR_STOP;
	}
	else
	{
		/* Disable the STOP condition generation */
		I2Cx->CCR = I2C_CCR_IS | I2C_CSR_STOP;
	}
}


/**
  * @brief  Generates I2C communication Acknowledge.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the Acknowledge.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_AcknowledgeConfig(CMSDK_I2C_TypeDef* I2Cx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	if (NewState != DISABLE)
	{
		/* Enable ACK generation */
		I2Cx->CCR = I2C_CCR_IS | I2C_CCR_START | I2C_CCR_STOP ;
		I2Cx->CSR = I2C_CSR_ACK;
	}
	else
	{
		/* Enable NACK generation */
		I2Cx->CCR = I2C_CCR_IS | I2C_CCR_ACK;
	}
}


/**
  * @brief  Sends a data byte through the I2Cx peripheral.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  Data: Byte to be transmitted..
  * @retval None
  */
void I2C_SendData(CMSDK_I2C_TypeDef* I2Cx, uint8_t Data)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	/* Write in the DR register the data to be sent */
	I2Cx->DR = (uint8_t)Data;
	//I2Cx->CCR = I2C_CCR_IS | I2C_CCR_START;
	I2Cx->CCR = I2C_CCR_IS;
}
/**
  * @brief  Sends a data/addr byte through the I2Cx peripheral.  -- then clear STA bit
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  Data: Byte to be transmitted..
  * @retval None
  */
void I2C_SendAddr(CMSDK_I2C_TypeDef* I2Cx, uint8_t Data)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	/* Write in the DR register the data to be sent */
	I2Cx->DR = (uint8_t)Data;
	I2Cx->CCR = I2C_CCR_IS | I2C_CCR_START;
}


/**
  * @brief  Returns the most recent received data by the I2Cx peripheral.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @retval The value of the received data.
  */
uint8_t I2C_ReceiveData(CMSDK_I2C_TypeDef* I2Cx)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	/* Return the data in the DR register */
	return (I2Cx->DR);
}


uint32_t I2C_GetStatus(CMSDK_I2C_TypeDef* I2Cx)
{
	/* Check the parameters */
	assert_param(IS_I2C_ALL_PERIPH(I2Cx));
	/* Return the SR register value */
	return (I2Cx->SR);
}


/**
  * @brief  Enables or disables the I2C automatic end mode (stop condition is
  *         automatically sent when nbytes data are transferred).
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C automatic end mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @note   This function has effect if Reload mode is disabled.
  * @retval None
  */

void I2C_TransferHandling(I2C_InstTypeDef* pInst, uint8_t StatValue)
{
	switch ( StatValue )
	{
		case 0x08:
			/*
			 * A START condition has been transmitted.
			 * We now send the slave address and initialize
			 * the write buffer
			 * (we always start with a write after START+SLA)
			 */
			pInst->WrIndex = 0;
			pInst->pInstance->DR = pInst->i2c_op;
			pInst->pInstance->CCR = (I2C_CCR_IS | I2C_CCR_START);
			pInst->iicState = I2CSTATE_PENDING;
			break;
		case 0x10:
			/*
			 * A repeated START condition has been transmitted.
			 * Now a second, read, transaction follows so we
			 * initialize the read buffer.
			 */
			pInst->RdIndex = 0;
			/* Send SLA with R bit set, */
			pInst->pInstance->DR = pInst->i2c_op;
			pInst->pInstance->CCR = (I2C_CCR_IS | I2C_CCR_START);
			break;
		case 0x18:
			/*
			 * SLA+W has been transmitted; ACK has been received.
			 * We now start writing bytes.
			 */
			pInst->pInstance->DR = pInst->pWriteData[pInst->WrIndex++];
			pInst->pInstance->CCR = I2C_CCR_IS;
			break;
		case 0x20:
			/*
			 * SLA+W has been transmitted; NOT ACK has been received.
			 * Send a stop condition to terminate the transaction
			 * and signal I2CEngine the transaction is aborted.
			 */
			pInst->pInstance->CSR = I2C_CSR_STOP;
			pInst->pInstance->CCR = I2C_CCR_IS;
			pInst->iicState = I2CSTATE_SLA_NACK;
			break;
		case 0x28:
			/*
			 * Data in I2DAT has been transmitted; ACK has been received.
			 * Continue sending more bytes as long as there are bytes to send
			 * and after this check if a read transaction should follow.
			 */
			if ( pInst->WrIndex < pInst->WriteDataLength )
			{
				/* Keep writing as long as bytes avail */
				pInst->pInstance->DR = pInst->pWriteData[pInst->WrIndex++];
			}
			else
			{
				if ( pInst->ReadDatalength != 0 )
				{
					/* Send a Repeated START to initialize a read transaction */
					/* (handled in state 0x10)                                */
					/* Set Repeated-start flag */
					pInst->pInstance->CSR = I2C_CSR_START;
				}
				else
				{
					pInst->iicState = I2CSTATE_ACK;
					pInst->pInstance->CSR = I2C_CSR_STOP;      /* Set Stop flag */
				}
			}
			pInst->pInstance->CCR = I2C_CCR_IS;
			break;
		case 0x30:
			/*
			 * Data byte in I2DAT has been transmitted; NOT ACK has been received
			 * Send a STOP condition to terminate the transaction and inform the
			 * I2CEngine that the transaction failed.
			 */
			pInst->pInstance->CSR = I2C_CSR_STOP;
			pInst->pInstance->CCR = I2C_CCR_IS;
			pInst->iicState = I2CSTATE_NACK;
			break;
		case 0x38:
			/*
			 * Arbitration loss in SLA+R/W or Data bytes.
			 * This is a fatal condition, the transaction did not complete due
			 * to external reasons (e.g. hardware system failure).
			 * Inform the I2CEngine of this and cancel the transaction
			 * (this is automatically done by the I2C hardware)
			 */
			pInst->iicState = I2CSTATE_ARB_LOSS;
			pInst->pInstance->CCR = I2C_CCR_IS;
			break;
		case 0x40:
			/*
			 * SLA+R has been transmitted; ACK has been received.
			 * Initialize a read.
			 * Since a NOT ACK is sent after reading the last byte,
			 * we need to prepare a NOT ACK in case we only read 1 byte.
			 */
			if ( pInst->ReadDatalength == 1 )
			{
				/* last (and only) byte: send a NACK after data is received */
				pInst->pInstance->CCR = I2C_CCR_ACK;
			}
			else
			{
				/* more bytes to follow: send an ACK after data is received */
				pInst->pInstance->CSR = I2C_CSR_ACK;
			}
			pInst->pInstance->CCR = I2C_CCR_IS;
			break;
		case 0x48:
			/*
			 * SLA+R has been transmitted; NOT ACK has been received.
			 * Send a stop condition to terminate the transaction
			 * and signal I2CEngine the transaction is aborted.
			 */
			pInst->pInstance->CSR = I2C_CSR_STOP;
			pInst->pInstance->CCR = I2C_CCR_IS;
			pInst->iicState = I2CSTATE_SLA_NACK;
			break;
		case 0x50:
			/*
			 * Data byte has been received; ACK has been returned.
			 * Read the byte and check for more bytes to read.
			 * Send a NOT ACK after the last byte is received
			 */
			pInst->pReadData[pInst->RdIndex++] = pInst->pInstance->DR;
			if ( pInst->RdIndex < (pInst->ReadDatalength - 1) )
			{
				/* lmore bytes to follow: send an ACK after data is received */
				pInst->pInstance->CSR = I2C_CSR_ACK;
			}
			else
			{
				/* last byte: send a NACK after data is received */
				pInst->pInstance->CCR = I2C_CCR_ACK;
			}
			pInst->pInstance->CCR = I2C_CCR_IS;
			break;
		case 0x58:
			/*
			 * Data byte has been received; NOT ACK has been returned.
			 * This is the last byte to read.
			 * Generate a STOP condition and flag the I2CEngine that the
			 * transaction is finished.
			 */
			pInst->pReadData[pInst->RdIndex++] = pInst->pInstance->DR;
			pInst->iicState = I2CSTATE_ACK;
			/* Set Stop flag */
			pInst->pInstance->CSR = I2C_CSR_STOP;
			/* Clear SI flag */
			pInst->pInstance->CCR = I2C_CCR_IS;
			break;
		/* Slave Mode */
		/* An own SLA_W has been received. */
		case 0x60:
		case 0x68:	//从机收到 Master Write 命令，开始准备接收数据
			pInst->RdIndex = 0;
			/* Assert ACK after SLV_W is received */
			pInst->pInstance->CSR = I2C_CSR_ACK;
			pInst->pInstance->CCR = I2C_CCR_IS;
			pInst->iicState = I2C_WR_STARTED;
			pInst->regAdr = pInst->maxBufSize;
			pInst->buffStat = SLV_REC_SLVW_EVENT;
			break;
		case 0x70:
		case 0x78:
			pInst->RdIndex = 0;
			pInst->iicState = I2C_BROADCASTED;
			/* Assert ACK after data is received */
			pInst->pInstance->CSR = I2C_CSR_ACK;
			pInst->pInstance->CCR = I2C_CCR_IS;
			break;
		case 0x80:	//从机接收数据(Master Write)
			/*  Data receive */
			if ( pInst->iicState == I2C_WR_STARTED )
			{
				if(pInst->RdIndex < pInst->regAdrLen)
				{
					if(pInst->RdIndex == 0)
						pInst->regAdr = 0;
					else
						pInst->regAdr <<= 8;
					pInst->regAdr |= pInst->pInstance->DR;
					pInst->RdIndex++;
				}
				else if(pInst->regAdr < pInst->maxBufSize)
				{
					pInst->pReadData[pInst->regAdr++] = pInst->pInstance->DR;
				}
				/* Assert ACK after data is received */
				pInst->pInstance->CSR = I2C_CSR_ACK;
			}
			else
			{
				/* Assert NACK */
				pInst->pInstance->CCR = I2C_CCR_ACK;
			}
			pInst->pInstance->CCR = I2C_CCR_IS;
			break;
		case 0x90:
		case 0x98:
			if ( pInst->iicState == I2C_BROADCASTED )
			{
				pInst->pReadData[pInst->RdIndex++] = pInst->pInstance->DR;
				/* Assert NACK */
				pInst->pInstance->CCR = I2C_CCR_ACK;
			}
			pInst->pInstance->CCR = I2C_CCR_IS;
			break;
		/* Stop condition or repeated start has  been received */
		case 0xA0:		//从机收到Stop condition位或repeated start位
			/* Assert ACK.  */
			pInst->buffStat |= SLV_REC_STOP_EVENT;
			pInst->pInstance->CSR = I2C_CSR_ACK;
			pInst->pInstance->CCR = I2C_CCR_IS;
			pInst->iicState = I2C_IDLE;
		break;
		case 0xA8:
		/* An own SLA_R has been received. */
		case 0xB0:	//已经收到Master Read 命令，开始发送regAdr为地址的数据数据
			pInst->WrIndex = 0;
			pInst->RdIndex = 0;
			/* Assert ACK after SLV_R is received */
			pInst->pInstance->CSR = I2C_CSR_ACK;
			pInst->iicState = I2C_RD_STARTED;
			if(pInst->regAdr >= pInst->maxBufSize)
				pInst->regAdr = 0;
			pInst->pInstance->DR = pInst->pWriteData[pInst->regAdr++];/* write the same data back to master */
			pInst->pInstance->CCR = I2C_CCR_IS;
			pInst->buffStat = SLV_REC_SLVR_EVENT;
			break;
		/* Data byte has been transmitted */
		case 0xB8:
		case 0xC8:	//发送regAdr为地址的数据数据(Master Read)
			if ( pInst->iicState == I2C_RD_STARTED )
			{
				if(pInst->regAdr >= pInst->maxBufSize)
					pInst->regAdr = 0;
				pInst->pInstance->DR = pInst->pWriteData[pInst->regAdr++];/* write the same data back to master */
				/* Assert ACK  */
				pInst->pInstance->CSR = I2C_CSR_ACK;
			}
			else
			{
				/* Assert NACK  */
				pInst->pInstance->CCR = I2C_CCR_ACK;
			}
			pInst->pInstance->CCR = I2C_CCR_IS;
			break;
		case 0xC0:	//从机响应主机读操作，从机已经发送完成最后一个字节，恢复ACK，下一次应该要收到停止位
			/* response to Master Read--last byte has been transmitted*/
			/* Data byte has been transmitted, NACK */
			/* Assert NACK  */
			pInst->pInstance->CCR = I2C_CCR_ACK;
			pInst->pInstance->CCR = I2C_CCR_IS;
			pInst->iicState = DATA_NACK;
			/* assert ACK */
			pInst->pInstance->CSR = I2C_CSR_ACK;
			pInst->WrIndex = 0x0;
			pInst->buffStat |= SLV_REC_STOP_EVENT;
			break;
		default:
			pInst->pInstance->CCR = I2C_CCR_IS;
			if (pInst->iicState == I2CSLAVE)
			{
				pInst->pInstance->CSR = I2C_CSR_EN;
			}
			break;
	}
}

/**
  * @brief  Create I2C start condition, a timeout value is set if the I2C never gets started,
  *         and timed out. It's a fatal error.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C automatic end mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @note   This function has effect if Reload mode is disabled.
  * @retval None
  */

uint32_t I2C_StartHandling(I2C_InstTypeDef* pInst)
{
	uint32_t timeout = 0;
	/*--- Issue a start condition ---*/
	/* Set Start flag and enable I2C peripheral.*/
	pInst->pInstance->CSR = I2C_CSR_EN | I2C_CSR_START;
	while((pInst->iicState != I2CSTATE_PENDING) && (timeout < MAX_TIMEOUT))
	{
		timeout++;
	}
	return (timeout < MAX_TIMEOUT);
}

/**
  * @brief  Create I2C stop condition, a timeout value is set if the I2C never gets started,
  *         and timed out. It's a fatal error.
  * @param  I2Cx: where x can be 1 or 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C automatic end mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @note   This function has effect if Reload mode is disabled.
  * @retval None
  */
uint32_t I2C_StopHandling(CMSDK_I2C_TypeDef* I2Cx)
{
	uint32_t timeout = 0;
	/* Set Stop flag */
	I2Cx->CSR = I2C_CSR_STOP;
	/* Clear SI flag */
	I2Cx->CCR = I2C_CCR_IS;
	/*--- Wait for STOP detected ---*/
	while((I2Cx->CSR & I2C_CSR_STOP) && (timeout < MAX_TIMEOUT))
	{
		timeout++;
	}
	return (timeout >= MAX_TIMEOUT);
}


/**
  * @brief  The routine to complete a I2C transaction from start to stop. All the intermitten
  *         steps are handled in the interrupt handler.Before this routine is called, the read
  *         Before this routine is called, the read length, write length and I2C master buffer
  *         need to be filled.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @retval Any of the I2CSTATE
  */
uint32_t i2cEngine(I2C_InstTypeDef* pInst)
{
	uint32_t timeout0 = 0;
	pInst->iicState = I2CSTATE_IDLE;
	pInst->RdIndex = 0;
	pInst->WrIndex = 0;
	pInst->pInstance->CSR = I2C_Ack_Enable;
//	if ( I2C_StartHandling(pInst) != TRUE )
//	{
//		I2C_StopHandling(pInst->pInstance);
//		return ( I2C_START_FAIL );
//	}
	/* wait until the state is a terminal state */
	while((pInst->iicState < 0x100) && (timeout0 < MAX_TIMEOUT))
	{
		timeout0++;
	}
	if(timeout0 == MAX_TIMEOUT)
	{
		return ( I2C_START_FAIL );
	}
	return ( pInst->iicState );
}


/**
  * @brief  Set Data and datalength .    
  * @param1  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
		@param2		data array	
				data[0] == devAdr + WR/RD bit
				data[n] ..
		@param3		data array total length
  * @retval Any of the I2CSTATE
* @}
  */

uint8_t		Set_IIC_TrsnsBuffer(I2C_InstTypeDef* pInst,uint8_t opByte,uint8_t * pData,uint32_t length)
{
	if(pInst == 0)	return 0;
	if(pInst->iicState != I2C_IDLE)	return 0;
	pInst->i2c_op = opByte;
	if(opByte & 0x01)
	{
		pInst->pReadData = pData;
		pInst->ReadDatalength = length;
		pInst->pWriteData = pData;
		pInst->WriteDataLength = 1;
	}
	else
	{
		pInst->pWriteData = pData;
		pInst->WriteDataLength = length;
		pInst->pReadData = pData;
		pInst->ReadDatalength = 0;
	}
	return length;
}


/**
  * @brief  The routine to complete a I2C transaction from start to stop. All the intermitten
  *         steps are handled in the interrupt handler.Before this routine is called, the read
  *         Before this routine is called, the read length, write length and I2C master buffer
  *         need to be filled.    
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.       
  * @retval Any of the I2CSTATE
  */
uint32_t i2cMaster_TransmitStart(I2C_InstTypeDef* pInst,uint8_t opByte,uint8_t *pData,uint32_t datLen)
{
	uint32_t timeout0 = 0;
	
	assert_param(pInst->i2c_mode == I2CMASTER);
	pInst->iicState = I2CSTATE_IDLE;
	Set_IIC_TrsnsBuffer(pInst,opByte,pData,datLen);
	pInst->RdIndex = 0;
	pInst->WrIndex = 0;
	if ( I2C_StartHandling(pInst) != TRUE )
	{
		I2C_StopHandling(pInst->pInstance);
		return ( I2C_START_FAIL );
	}

  /* wait until the state is a terminal state */
	while((pInst->iicState < 0x100) && (timeout0 < MAX_TIMEOUT))
	{
		timeout0++;
	}
	if(timeout0==MAX_TIMEOUT)
	{
		return ( I2C_START_FAIL );
	}
		
	return ( pInst->iicState );
}

/**
  * @}
  */