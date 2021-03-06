#ifndef SRC_ADC
#define SRC_ADC


#define ADC1_DR_Address    				((u32)0x40012400+0x4c)		// ����� �������� ���������� �������������� ���
#define DMA_BUFF_SIZE 					160							// ������ ������

#define K_Isens			8.064f				// ����������� ��� ��������� �� ��� � ������ ���� ������
#define K_Uzpt			0.097f				// ����������� ��� ��������� �� ��� � ����� ���
#define K_Un			9.77f				// ����������� ��� ��������� �� ��� � ����� ������
#define K_Ibreak		0.244f				// ����������� ��� ��������� �� ��� � ������ �� �����



void init_adc(void);
extern void ADC1_2_IRQHandler(void);
extern void DMA1_Channel1_IRQHandler(void);


#endif // SRC_ADC
