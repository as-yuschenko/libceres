/*MIT License   ENG

Copyright (c) 2025 Aleksandr Yuschenko

For Contact:
https://github.com/as-yuschenko/libceres
https://t.me/Alex_Yuschenko
mailto:a.s.yuschenko@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/*MIT Лицензия  РУС

Copyright © «2025» «Ющенко Александр Сергеевич»

Для связи:
https://github.com/as-yuschenko/libceres
https://t.me/Alex_Yuschenko
mailto:a.s.yuschenko@gmail.com

Данная лицензия разрешает лицам, получившим копию данного программного обеспечения и
сопутствующей документации (в дальнейшем именуемыми «Программное Обеспечение»),
безвозмездно использовать Программное Обеспечение без ограничений,
включая неограниченное право на использование, копирование, изменение, слияние,
публикацию, распространение, сублицензирование и/или продажу копий Программного Обеспечения,
а также лицам, которым предоставляется данное Программное Обеспечение, при соблюдении следующих условий:

Указанное выше уведомление об авторском праве и данные условия должны быть
включены во все копии или значимые части данного Программного Обеспечения.

ДАННОЕ ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ ПРЕДОСТАВЛЯЕТСЯ «КАК ЕСТЬ», БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ,
ЯВНО ВЫРАЖЕННЫХ ИЛИ ПОДРАЗУМЕВАЕМЫХ, ВКЛЮЧАЯ ГАРАНТИИ ТОВАРНОЙ ПРИГОДНОСТИ,
СООТВЕТСТВИЯ ПО ЕГО КОНКРЕТНОМУ НАЗНАЧЕНИЮ И ОТСУТСТВИЯ НАРУШЕНИЙ, НО НЕ ОГРАНИЧИВАЯСЬ ИМИ.
НИ В КАКОМ СЛУЧАЕ АВТОРЫ ИЛИ ПРАВООБЛАДАТЕЛИ НЕ НЕСУТ ОТВЕТСТВЕННОСТИ ПО КАКИМ-ЛИБО ИСКАМ,
ЗА УЩЕРБ ИЛИ ПО ИНЫМ ТРЕБОВАНИЯМ, В ТОМ ЧИСЛЕ, ПРИ ДЕЙСТВИИ КОНТРАКТА, ДЕЛИКТЕ ИЛИ ИНОЙ СИТУАЦИИ,
ВОЗНИКШИМ ИЗ-ЗА ИСПОЛЬЗОВАНИЯ ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ ИЛИ ИНЫХ ДЕЙСТВИЙ С ПРОГРАММНЫМ ОБЕСПЕЧЕНИЕМ.
*/

#ifndef CERES_H_INCLUDED
#define CERES_H_INCLUDED


#define CERES_SIZE_CARD_CODE                8
#define CERES_SIZE_STATES_ARR               5
#define CERES_SIZE_DEV_NAMES_ARR            53

#define CERES_ET_NO_EVENT                   0
#define CERES_ET_COMMON                     1
#define CERES_ET_ALARM                      2
#define CERES_ET_ARM                        4
#define CERES_ET_TROUBLE                    8
#define CERES_ET_RECOVERY                   16
#define CERES_ET_RELAY                      32
#define CERES_ET_ACCESS                     64
#define CERES_ET_TECH                       128


struct ceres_st_states
{
    unsigned char addr;
    unsigned char num;
    unsigned char obtained;
    unsigned char values[CERES_SIZE_STATES_ARR];
};

struct ceres_st_buffer_datetime
{
    unsigned char h;
    unsigned char m;
    unsigned char s;
    unsigned char d;
    unsigned char M;
    unsigned char y;
};

struct ceres_st_event
{
    unsigned char               code;
    unsigned char               zone;
    unsigned char               addr;
    unsigned char               part_mb;
    unsigned char               part_lb;
    unsigned char               relay;
    unsigned char               prog;
    unsigned char               reader;
    unsigned char               user;
    unsigned char               card_code[CERES_SIZE_CARD_CODE];
    char                        dt_str[27];
    ceres_st_buffer_datetime    dt;
};

extern ceres_st_states      ceres_states;
extern ceres_st_event       ceres_event;

void ceres_init (unsigned char* buffer, int* len);
void ceres_free ();
void ceres_set_dev_key(unsigned char addr, unsigned char key);


/*--------DEVICE--------*/
void ceres_q_dev_info(unsigned char addr);

char ceres_r_dev_info(unsigned char addr, unsigned char* type_dest, unsigned char* ver_dest);


/*--------GO SEC MODE--------*/
void ceres_q_sec_begin(unsigned char addr);

char ceres_r_sec_begin(unsigned char addr);


/*-------SET DEV TIME-------*/
void ceres_q_set_date_time(unsigned char addr, unsigned char _hour, unsigned char _min, unsigned char _sec, unsigned char _day, unsigned char _mon, unsigned char _year);//do nothing in this version

char ceres_r_set_date_time(unsigned char addr);//do nothing in this version


/*--------STATES--------*/
//SIMPLE
void ceres_q_state_simp(unsigned char addr, unsigned char zone);

char ceres_r_state_simp(unsigned char addr, unsigned char zone);

//EXTENDED
void ceres_q_state_ext(unsigned char addr, unsigned char zone);//do nothing in this version

char ceres_r_state_ext(unsigned char addr, unsigned char zone);//do nothing in this version

//RELAY
void ceres_q_state_relay(unsigned char addr, unsigned char relay);//do nothing in this version

char ceres_r_state_relay(unsigned char addr, unsigned char relay);//do nothing in this version


/*--------EVENTS--------*/
void ceres_q_read_event(unsigned char addr);

void ceres_q_load_event(unsigned char addr);

int ceres_09_event(unsigned char addr); //S2000-KDL

int ceres_10_event(unsigned char addr);//do nothing in this version //S2000-2


/*--------ALARMS--------*/
void ceres_q_drop_alarm(unsigned char addr);//do nothing in this version

char ceres_r_drop_alarm(unsigned char addr);//do nothing in this version


/*--------COUNTERS-------*/
void ceres_q_counter(unsigned char addr, unsigned char zone);//do nothing in this version

char ceres_r_counter(unsigned char addr, unsigned char zone, long long unsigned int* counter_dest);//do nothing in this version


/*--------ZONES-------*/
void ceres_q_zone_arm(unsigned char addr, unsigned char zone);

char ceres_r_zone_arm(unsigned char addr, unsigned char zone);

void ceres_q_zone_disarm(unsigned char addr, unsigned char zone);

char ceres_r_zone_disarm(unsigned char addr, unsigned char zone);

void ceres_q_zone_firetest(unsigned char addr, unsigned char zone, unsigned short test_time);//do nothing in this version

char ceres_r_zone_firetest(unsigned char addr, unsigned char zone);//do nothing in this version

void ceres_q_zone_disable(unsigned char addr, unsigned char zone);//do nothing in this version

char ceres_r_zone_disable(unsigned char addr, unsigned char zone);//do nothing in this version

void ceres_q_zone_enable(unsigned char addr, unsigned char zone);//do nothing in this version

char ceres_r_zone_enable(unsigned char addr, unsigned char zone);//do nothing in this version

/*--------RELAY-------*/
void ceres_q_relay_on(unsigned char addr, unsigned char relay);

char ceres_r_relay_on(unsigned char addr, unsigned char relay);

void ceres_q_relay_off(unsigned char addr, unsigned char relay);

char ceres_r_relay_off(unsigned char addr, unsigned char relay);


/*--------ACCESS MANAGEMENT--------*/
void ceres_q_access_unblock(unsigned char addr, unsigned char reader_num);//do nothing in this version

char ceres_r_access_unblock(unsigned char addr, unsigned char reader_num);//do nothing in this version

void ceres_q_access_block(unsigned char addr, unsigned char reader_num);//do nothing in this version

char ceres_r_access_block(unsigned char addr, unsigned char reader_num);//do nothing in this version

void ceres_q_access_reset(unsigned char addr, unsigned char reader_num);//do nothing in this version

char ceres_r_access_reset(unsigned char addr, unsigned char reader_num);//do nothing in this version

void ceres_q_access_grant_card(unsigned char addr, unsigned char reader_num);//do nothing in this version

char ceres_r_access_grant_card(unsigned char addr, unsigned char reader_num);//do nothing in this version

void ceres_q_access_grant_noname(unsigned char addr, unsigned char reader_num);//do nothing in this version

char ceres_r_access_grant_noname(unsigned char addr, unsigned char reader_num);//do nothing in this version

void ceres_q_access_deny(unsigned char addr, unsigned char reader_num);//do nothing in this version

char ceres_r_access_deny(unsigned char addr);//do nothing in this version


/*--------ADC-------*/
void ceres_q_adc_v1(unsigned char addr, unsigned char zone);//do nothing in this version

unsigned char* ceres_r_adc_v1(unsigned char addr);//do nothing in this version

void ceres_q_adc_v2(unsigned char addr, unsigned char zone);//do nothing in this version

unsigned char* ceres_r_adc_v2(unsigned char addr);//do nothing in this version

void ceres_extract_adc(double* dest);//do nothing in this version
void ceres_extract_adc(long int* dest);//do nothing in this version

/*--------NAMES-------*/
const char* ceres_name_dev(unsigned char type);
const char* ceres_name_event(unsigned char event);
const char* ceres_name_event_trnslt(unsigned char event);
unsigned char ceres_event_priority(unsigned char event);

/*--------ABOUT-------*/
void ceres_about(char* buff);

#endif // CERES_H_INCLUDED
