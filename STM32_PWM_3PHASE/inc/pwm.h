#ifndef SRC_PWM
#define SRC_PWM


#define period              	1200 //	4500 - 16kHz COMPL_MODE		// 360 - 100 kHz PP_MODE // 720 - 50 kHz PP_MODE
#define half_period				period / 2
#define DEADTIME            	60

void init_pwm_3phase(void);
void init_pwm_PP_mode(void);
void init_pwm_Compl(void);
void pwm_PP_stop(void);
void test_pwm_step(void);
void init_timer3(void);

void set_pwm_value(unsigned short pwm_fb);
void set_pwm_value_buck(unsigned short pwm_zpt);
unsigned short get_deadtime_value();
unsigned short get_frequency_pwm();









#endif // SRC_PWM
