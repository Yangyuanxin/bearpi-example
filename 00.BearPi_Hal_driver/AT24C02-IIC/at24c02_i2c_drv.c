/**
 * @Copyright 			(c) 2019,mculover666 All rights reserved	
 * @filename  			at24c02_i2c_drv.c
 * @breif				Drive AT24C02(EEPROM) based on iic1 commucation interface
 * @version
 *            			v1.0    ��ɻ�����������              mculover666    2019/7/15
 * @note            	��ֲ˵�����ǳ���Ҫ����	
 *						1. �޸�at24c02_i2c_drv.h�ļ��е�AT24C02��д��ַ���ɣ�
 * 						2. ������������ҪSTM32CubeMX���ɵ�I2C��ʼ���ļ�i2c.h��i2c.c֧�֡�
 */

#include "at24c02_i2c_drv.h"
#include "i2c.h"

/**
 * @brief		AT24C02�����ַдһ���ֽ�����
 * @param		addr ���� д���ݵĵ�ַ��0-255��
 * @param		dat  ���� ���д�����ݵĵ�ַ
 * @retval		�ɹ� ���� HAL_OK
*/
uint8_t At24c02_Write_Byte(uint16_t addr, uint8_t* dat)
{
		return HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, addr, I2C_MEMADD_SIZE_8BIT, dat, 1, 0xFFFF);
}
/**
 * @brief		AT24C02�����ַ��һ���ֽ�����
 * @param		addr ���� �����ݵĵ�ַ��0-255��
 * @param		read_buf ���� ��Ŷ�ȡ���ݵĵ�ַ
 * @retval		�ɹ� ���� HAL_OK
*/
uint8_t At24c02_Read_Byte(uint16_t addr, uint8_t* read_buf)
{
		return HAL_I2C_Mem_Read(&hi2c1, AT24C02_ADDR_READ, addr, I2C_MEMADD_SIZE_8BIT, read_buf, 1, 0xFFFF);
}
/**
 * @brief		AT24C02�����ַ����д����ֽ�����
 * @param		addr ���� д���ݵĵ�ַ��0-255��
 * @param		dat  ���� ���д�����ݵĵ�ַ
 * @retval		�ɹ� ���� HAL_OK
*/
uint8_t At24c02_Write_Amount_Byte(uint16_t addr, uint8_t* dat, uint16_t size)
{
		uint8_t i = 0;
		uint16_t cnt = 0;		//д���ֽڼ���
		
		/* ������ʼ��ַ��������������ֱ��ж� */
		if(0 == addr % 8 )
		{
			/* ��ʼ��ַ�պ���ҳ��ʼ��ַ */
			
			/* ����д����ֽ�����������������ֱ��ж� */
			if(size <= 8)
			{
				//д����ֽ���������һҳ��ֱ��д��
				return HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, addr, I2C_MEMADD_SIZE_8BIT, dat, size, 0xFFFF);
			}
			else
			{
				//д����ֽ�������һҳ���Ƚ���ҳѭ��д��
				for(i = 0;i < size/8; i++)
				{
						HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, addr, I2C_MEMADD_SIZE_8BIT, &dat[cnt], 8, 0xFFFF);
						addr += 8;
						cnt += 8;
				}
				//��ʣ����ֽ�д��
				return HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, addr, I2C_MEMADD_SIZE_8BIT, &dat[cnt], size - cnt, 0xFFFF);
			}
		}
		else
		{
			/* ��ʼ��ַƫ��ҳ��ʼ��ַ */
			/* ����д����ֽ�����������������ֱ��ж� */
			if(size <= (8 - addr%8))
			{
				/* �ڸ�ҳ����д�� */
				return HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, addr, I2C_MEMADD_SIZE_8BIT, dat, size, 0xFFFF);
			}
			else
			{
				/* ��ҳд���� */
				//�Ƚ���ҳд��
				cnt += 8 - addr%8;
				HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, addr, I2C_MEMADD_SIZE_8BIT, dat, cnt, 0xFFFF);
				addr += cnt;
				
				//ѭ��д��ҳ����
				for(i = 0;i < (size - cnt)/8; i++)
				{
						HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, addr, I2C_MEMADD_SIZE_8BIT, &dat[cnt], 8, 0xFFFF);
						addr += 8;
						cnt += 8;
				}
				
				//��ʣ�µ��ֽ�д��
				return HAL_I2C_Mem_Write(&hi2c1, AT24C02_ADDR_WRITE, addr, I2C_MEMADD_SIZE_8BIT, &dat[cnt], size - cnt, 0xFFFF);
			}			
		}
}
/**
 * @brief		AT24C02�����ַ����������ֽ�����
 * @param		addr ���� �����ݵĵ�ַ��0-255��
 * @param		dat  ���� ��Ŷ������ݵĵ�ַ
 * @retval		�ɹ� ���� HAL_OK
*/
uint8_t At24c02_Read_Amount_Byte(uint16_t addr, uint8_t* recv_buf, uint16_t size)
{
	return HAL_I2C_Mem_Read(&hi2c1, AT24C02_ADDR_READ, addr, I2C_MEMADD_SIZE_8BIT, recv_buf, size, 0xFFFF);
}
