#ifndef SRC_PWM
#define SRC_PWM


#define period              	360	//	4500 - 16kHz COMPL_MODE		// 360 - 100 kHz PP_MODE
#define half_period				period / 2
#define DEADTIME            	20

void init_pwm_3phase(void);
void init_pwm_PP_mode(void);
void pwm_PP_stop(void);
void test_pwm_step(void);
void init_timer3(void);

unsigned short get_pwm_value();
unsigned short get_deadtime_value();








#endif // SRC_PWM
