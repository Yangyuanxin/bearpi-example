#include "bsp_irda.h"

uint32_t frame_data=0;    /* һ֡���ݻ��� */
uint8_t  frame_cnt=0;
uint8_t  frame_flag=0;    /* һ֡���ݽ�����ɱ�־ */

void IrDa_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  IRDA_RCC_CLK_ENABLE();

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = IRDA_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IRDA_GPIO, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(IRDA_EXTI_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(IRDA_EXTI_IRQn);
}

/**
  * ��������: ��ȡ�ߵ�ƽ��ʱ��
  * �������: ��
  * �� �� ֵ: ʱ��
  * ˵    ������
  */
uint8_t Get_Pulse_Time(void)
{
  uint8_t time = 0;
  while( IrDa_DATA_IN() )
  {
    time ++;
    HAL_Delay(2);     // ��ʱ 20us
    if(time == 250)
      return time;   // ��ʱ���   
  }
  return time;
}

/**
  * ��������: ��ȡ֡����
  * �������: ��
  * �� �� ֵ: ֡����
  * ˵    ����֡������4���ֽڣ���һ���ֽ���ң�ص�ID���ڶ����ֽ���
  *           ��һ���ֽڵķ��룻������������ң�ص������ļ�ֵ��
  *           ���ĸ��ֽ��ǵ������ֽڵķ���
  */
uint8_t IrDa_Process(void)
{
  uint8_t first_byte, sec_byte, tir_byte, fou_byte;  
  
  first_byte = frame_data >> 24;
  sec_byte = (frame_data>>16) & 0xff;
  tir_byte = frame_data >> 8;
  fou_byte = frame_data;
  
  /* �ǵ����־λ */
  frame_flag = 0;
  
  if( (first_byte==(uint8_t)~sec_byte) && (first_byte==IRDA_ID) )
  {
    if( tir_byte == (uint8_t)~fou_byte )
      return tir_byte;
  }
  
  return 0;   /* ���󷵻� */
}
