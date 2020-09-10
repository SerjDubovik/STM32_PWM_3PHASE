#ifndef SRC_PWM
#define SRC_PWM


#define period               4500	//	4500 - 16kHz COMPL_MODE		// 360 - 100 kHz PP_MODE
#define half_period			period / 2
#define DEADTIME            	20

void init_pwm(void);
void test_pwm_step(void);








#endif // SRC_PWM
