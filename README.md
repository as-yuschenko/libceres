# Опрос и управление устройствами системы "Орион" компании Болид.

## Реализованный в библиотеке функционал в настоящее время позволяет:

+ Получать информацию о событиях приборов;
+ По запросу получать общее или расширенное состояние шлейфов, выходов и приборов;
+ Управлять взятием и снятием шлейфов с охраны;
+ Включать или отключать шлейфы;
+ Управлять клапанами СДУ и ПДВ через С2000-СП4;
+ Управлять включением и выключением реле;
+ Переводить извещатели в режим пожарного тестирования;
+ Получать данные счетчиков;
+ Получать данные о температуре, влажности и т.д.;
+ Получать коды проксимити-карт при событиях СКУД;
+ Управлять замками СКУД;
+ Перезапускать приборы;
+ Получать информацию о типах и версиях приборов.

Обмен данными с приборами компании Болид производится по "зашифрованному" протоколу "Орион". 

## Для связи:
https://t.me/Alex_Yuschenko <br />
mailto:a.s.yuschenko@gmail.com <br />

## Light версия библиотеки.

Эта версия имеет ограниченный функционал. Функции, недоступные в light версии помечены тэгом **[FULL]**

## Связанные видео:

Управление С2000-М с помощью  одноплатника без дополнительных устройств,  лог событий.

https://youtu.be/i4qdD2S-6xo<br />

Управление Орион Про с помощью  одноплатника без дополнительных устройств,  лог событий.

https://youtu.be/S8kyMtFvjSc<br />

# MAN
Для работы библиотеки требуется буфер обмена, который вы можете создать любым, удобным вам способом. Рекомендуемый размер буфера - 256 байт. 

Ниже приведено описание функций и констант библиотеки.

### Типы событий.

**CERES_ET_NO_EVENT** нет событий

**CERES_ET_COMMON** общее событие

**CERES_ET_ALARM** событие тревога

**CERES_ET_ARM** событие взятие / снятие

**CERES_ET_TROUBLE** событие неисправность

**CERES_ET_RECOVERY** событие восстановление

**CERES_ET_RELAY** событие реле (выхода)

**CERES_ET_ACCESS** событие СКУД

**CERES_ET_TECH** событие техлоногическое

### Структуры.

Для получения информации о событиях приборов, состояний шлейфов, выходов и самих приборах реализованы две структуры.

Структура **ceres_st_states** сохраняет в себе состояния шлейфа, выхода или прибора, после выполнения запроса состояния.

```cpp
struct ceres_st_states
{
    unsigned char addr; // адрес прибора
    unsigned char num; // номер шлейфа или выхода (реле)
	unsigned char obtained; // число полученных состояний
    unsigned char values[CERES_SIZE_STATES_ARR]; // коды полученных состояний (CERES_SIZE_STATES_ARR = 5)
};
```

Структура **ceres_st_event** сохраняет в себе данные о событии прибора.

```cpp
struct ceres_st_event
{
    unsigned char code; // код события
    unsigned char zone; // номер зоны (0 - прибор)
	unsigned char addr; // адрес прибора
    unsigned char part_mb; // номер раздела, старший байт
    unsigned char part_lb; // номер раздела, младший байт
    unsigned char relay; // номер реле
    unsigned char prog; // программа управления реле
    unsigned char reader; // номер считывателя
	unsigned char user; // номер пользователя
    unsigned char card_code[CERES_SIZE_CARD_CODE]; // Код проксимити карты (CERES_SIZE_CARD_CODE = 8)
    char dt_str[27];// Время наступления события, если оно считано из буфера, в формате нуль-терминированной строки.
    ceres_st_buffer_datetime dt; // Вспомогательная структура для хранения времени при обработке запроса.
};
```

Структуры **ceres_st_states** и **ceres_st_event** инициализированы в переменных **ceres_states** и **ceres_event** соответственно.
После выполнения запросов на чтение события или состояния, для получения данных, необходимо обращаться к этим переменным.

### Инициализация библиотеки.

```cpp
/** Инициализация библиотеки.
* @param unsigned char* buffer - указатель на буфер обмена.
* @param int* len - указатель на переменную для получения количества принятых байт или сохранения количества байт подготовленных для передачи.
* @return void
*/
void ceres_init (unsigned char* buffer, int* len);
```

### Высвобождение памяти.

```cpp
/** Высвобождение памяти.
* @return void
*/
void ceres_free ();
```

### Ключи приборов.

Ключи приборов генерируются автоматически при инициализации библиотеки. Если Вам нужно назначить прибору определенный ключ, можно воспользоваться следующей функцией.

```cpp
/** Назначение ключа прибору.
* @param unsigned char addr - адрес прибора.
* @param unsigned char key - ключ прибора.
* @return void
*/
void ceres_set_dev_key(unsigned char addr, unsigned char key);
```

### Общее для всех функций.

Функции, начинающиеся с **"ceres_q_..."**, формируют последовательность байт запроса и помещают его в буфер обмена [buffer], при этом в переменную [len] сохраняется количество байт, подготовленных к передаче.

Функции, начинающиеся с **"ceres_r_..."**, проверяют достоверность полученных данных и сохраняют запрошенные данные в переменные **ceres_states** и **ceres_event** или иные, передаваемые через указатели.  

**Внимание!** Во всех функциях **unsigned char addr** - адрес прибора, которому посылается запрос.  
  
## Функции для работы с приборами.
  
### Получения типа и версии устройства.

```cpp
/** Запрос для получения информации о устройстве.
* @return void
*/
ceres_q_dev_info(unsigned char addr);


/** Обработка ответа.
* @param unsigned char* type_dest - указатель на переменную для сохранения типа устройства.
* @param unsigned char* ver_dest - указатель на переменную для сохранения версии устройства.
* @return
* 0 - успешно.
* -1 - ошибка принятых данных.
*/
char ceres_r_dev_info(unsigned char addr, unsigned char* type_dest, unsigned char* ver_dest);
```

### Переход на зашифрованный протокол обмена "Орион".

```cpp
/** Запрос на переход на зашифрованный протокол обмена.
* @return void
*/
void ceres_q_sec_begin(unsigned char addr);


/** Обработка ответа.
* @return
* 0 - успешно.
* -1 - ошибка принятых данных.
*/
char ceres_r_sec_begin(unsigned char addr);
```

### Установка даты и времени прибора. **[FULL]**

```cpp
/** Запрос на установку даты и времени прибора.
* @param unsigned char _hour - часы.
* @param unsigned char _min	- минуты.
* @param unsigned char _sec	- секунды.
* @param unsigned char _day	- число.
* @param unsigned char _mon	- месяц.
* @param unsigned char _year - последние 2 цифры года.
*
* @return void
*/
void ceres_q_set_date_time(unsigned char addr, unsigned char _hour, unsigned char _min, unsigned char _sec, unsigned char _day, unsigned char _mon, unsigned char _year);


/** Обработка ответа.
* @return
* 0 - успешно.
* -1 - ошибка принятых данных.
*/
int ceres_r_set_date_time(unsigned char addr);
```

### Перезапуск прибора. **[FULL]**

```cpp
/** Запрос на перезапуск прибора.
*
* @return void
*/
void ceres_q_dev_reboot(unsigned char addr);


/** Обработка ответа.
* @return
* 0 - успешно.
* -1 - ошибка принятых данных.
*/
int ceres_r_dev_reboot(unsigned char addr);
```

### Общее состояние шлейфа, выхода или прибора.

**Внимание!** Состояния сохраняются в **ceres_states**. Описание приведено выше. Перед сохранением **ceres_states** заполняется нулями. 

Для получения состояния прибора передать zone равным нулю. 

```cpp
/** Запрос на получение общего состояния шлейфа, выхода или прибора.
* @param unsigned char zone - номер шлейфа или выхода.
* @return void
*/
void ceres_q_state_simp(unsigned char addr, unsigned char zone);


/** Обработка ответа.
* @param unsigned char zone - номер шлейфа или выхода.
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_state_simp(unsigned char addr, unsigned char zone);
```

### Расширенное состояние шлейфа, выхода или прибора. **[FULL]**

**Внимание!** Состояния сохраняются в **ceres_states**. Описание приведено выше. Перед сохранением **ceres_states** обнуляется. 

Для получения состояния прибора передать zone равным нулю. 

```cpp
/** Запрос на получение расширенного состояния шлейфа, выхода или прибора.
* @param unsigned char zone - номер шлейфа или выхода.
* @return void
*/
void ceres_q_state_ext(unsigned char addr, unsigned char zone);


/** Обработка ответа.
* @param unsigned char zone - номер шлейфа или выхода.
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_state_ext(unsigned char addr, unsigned char zone);
```

### Текущая программа управления реле (выхода). **[FULL]**

**Внимание!** Программа управления сохраняется в **ceres_states**. Описание приведено выше. Перед сохранением **ceres_states** заполняется нулями. 


```cpp
/** Запрос на получение текущей программы управления реле (выхода).
* @param unsigned char relay - номер реле (выхода).
* @return void
*/
void ceres_q_state_relay(unsigned char addr, unsigned char relay);


/** Обработка ответа.
* @param unsigned char relay - номер реле (выхода).
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_state_relay(unsigned char addr,  unsigned char relay);
```

### Получение события прибора.

Алгоритм чтения событий:

1. Сформировать запрос на чтение события с помощью функции ceres_q_read_event(...) и отправить его прибору; 

2. Обработать ответ с помощью функции ceres_xxx_event(...), где ххх - тип прибора, при этом получаем тип события, определяемый константами CERES_ET_..., если событие есть - информация о событии сохраняется в переменную **ceres_event**;

3.1. В случае если событие есть - сформировать запрос на загрузку следующего события с помощью функции ceres_q_load_event(...) и отправить его прибору. Внимание! Ответа от прибора не будет. Если ведется опрос одного прибора рекомендуется небольшой тайм-аут.

3.2. Если события нет, переходим к пункту 1.


**Чтение события прибора.**

```cpp
/** Запрос на чтение события прибора.
* @return void
*/
void ceres_q_read_event(unsigned char addr);
```


**Загрузка следующего события прибора.**

```cpp
/** Запрос на загрузку следующего события прибора.
* @return void
*/
void ceres_q_load_event(unsigned char addr);
```


**Обработка событий для С2000-КДЛ:**

```cpp
/** Обработка ответа.
* @return
* CERES_ET_xxx, где xxx - тип события 
* -1 - ошибка принятых данных.
*/
int ceres_09_event(unsigned char addr);
```

**Обработка событий для С2000-2:** В процессе разработки.

```cpp
/** Обработка ответа.
* @return
* CERES_ET_xxx, где xxx - тип события 
* -1 - ошибка принятых данных.
*/
int ceres_10_event(unsigned char addr);
```

### Сброс тревог. **[FULL]**

```cpp
/** Запрос сброса тревоги.
* @return void
*/
void ceres_q_drop_alarm(unsigned char addr);


/** Обработка ответа.
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_drop_alarm(unsigned char addr);
```

### Управление шлейфами.

**Взятие шлейфа на охрану.**

```cpp
/** Запрос на взятие шлейфа под охрану.
* @param unsigned char zone - номер шлейфа.
* @return void
*/
void ceres_q_zone_arm(unsigned char addr, unsigned char zone);


/** Обработка ответа.
* @param unsigned char zone - номер шлейфа или выхода.
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_zone_arm(unsigned char addr, unsigned char zone);
```

**Снятие шлейфа с охраны.**

```cpp
/** Запрос на снятие шлейфа с охраны.
* @param unsigned char zone - номер шлейфа.
* @return void
*/
void ceres_q_zone_disarm(unsigned char addr, unsigned char zone);


/** Обработка ответа.
* @param unsigned char zone - номер шлейфа.
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_zone_disarm(unsigned char addr, unsigned char zone);
```

**Перевод пожарного шлейфа в режим тестирования.** **[FULL]**

```cpp
/** Запрос на перевод пожарного шлейфа в режим тестирования.
* @param unsigned char zone - номер шлейфа.
* @param unsigned short test_time - время тестировния в секундах.
* @return void
*/
void ceres_q_zone_firetest(unsigned char addr, unsigned char zone, unsigned short test_time);


/** Обработка ответа.
* @param unsigned char zone - номер шлейфа.
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_zone_firetest(unsigned char addr, unsigned char zone);
```

**Отключение контроля шлейфа (для пожарных шлейфов без права снятия).** **[FULL]**

```cpp
/** Запрос на отключение контроля шлейфа.
* @param unsigned char zone - номер шлейфа.
* @return void
*/
void ceres_q_zone_disable(unsigned char addr, unsigned char zone);


/** Обработка ответа.
* @param unsigned char zone - номер шлейфа.
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_zone_disable(unsigned char addr, unsigned char zone);
```

**Включение контроля шлейфа (для пожарных шлейфов без права снятия).** **[FULL]**

```cpp
/** Запрос на включение контроля шлейфа.
* @param unsigned char zone - номер шлейфа.
* @return void
*/
void ceres_q_zone_enable(unsigned char addr, unsigned char zone);


/** Обработка ответа.
* @param unsigned char zone - номер шлейфа.
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_zone_enable(unsigned char addr, unsigned char zone);
```

### Управление реле (выходами).

```cpp
/** Запрос на включение реле.
* @param unsigned char relay - номер реле (выхода).
* @return void
*/
void ceres_q_relay_on(unsigned char addr, unsigned char relay);


/** Обработка ответа.
* @param unsigned char relay- номер реле (выхода).
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_relay_on(unsigned char addr, unsigned char relay);


/** Запрос на выключение реле.
* @param unsigned char relay - номер реле (выхода).
* @return void
*/
void ceres_q_relay_off(unsigned char addr, unsigned char relay);


/** Обработка ответа.
* @param unsigned char relay - номер реле (выхода).
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_relay_off(unsigned char addr, unsigned char relay);
```

### Управление клапанами С2000-СП4.

Данные команды применяются для управления клапанами СДУ и ПДВ.

**Перевод клапана в рабочее состояние** **[FULL]**
```cpp
/** Запрос на перевод клапана в рабочее состояние.
* @param unsigned char valve - адрес клапана на шине КДЛ (1й из 5ти).
* @return void
*/
void ceres_q_valve_mode_work(unsigned char addr, unsigned char valve);


/** Обработка ответа.
* @param unsigned char valve - адрес клапана на шине КДЛ (1й из 5ти).
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
int ceres_r_valve_mode_work(unsigned char addr, unsigned char valve);
```

**Перевод клапана в исходное состояние** **[FULL]**
```cpp
/** Запрос на перевод клапана в исходное состояние.
* @param unsigned char valve - адрес клапана на шине КДЛ (1й из 5ти).
* @return void
*/
void ceres_q_valve_mode_ready(unsigned char addr, unsigned char valve);


/** Обработка ответа.
* @param unsigned char valve - адрес клапана на шине КДЛ (1й из 5ти).
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
int ceres_r_valve_mode_ready(unsigned char addr, unsigned char valve);
```

### Управление доступом.

Данные запросы применяются для контроллера доступа С2000-2 и позволяют:

1. Разблокировать дверь (замок будет открыт до восстановления штатного доступа или блокировки двери, вне зависимости от предъявляемого идентификатора);

2. Заблокировать дверь (замок будет закрыт до восстановления штатного доступа или разблокировки двери, вне зависимости от предъявляемого идентификатора);

3. Восстановить штатный режим доступа (замок будет закрыт и открываться или не открываться, в зависимости от предъявляемого идентификатора).

4. Предоставить доступ пользователю, предъявившему идентификатор, номера которого нет в контроллере.

5. Запретить доступ пользователю, предъявившему идентификатор, номера которого нет в контроллере.

6. Предоставить доступ однократно (открыть дверь).

**1. Разблокировать дверь.** **[FULL]**

```cpp
/** Запрос на разблокировку двери.
* @param unsigned char reader_num - номер считывателя (реле).
* @return void
*/
void ceres_q_access_unblock(unsigned char addr, unsigned char reader_num);

/** Обработка ответа.
* @param unsigned char reader_num - номер считывателя (реле).
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_access_unblock(unsigned char addr, unsigned char reader_num);
```

**2. Заблокировать дверь.** **[FULL]**

```cpp
/** Запрос на блокировку двери.
* @param unsigned char reader_num - номер считывателя (реле).
* @return void
*/
void ceres_q_access_block(unsigned char addr, unsigned char reader_num);

/** Обработка ответа.
* @param unsigned char reader_num - номер считывателя (реле).
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_access_block(unsigned char addr, unsigned char reader_num);
```

**3. Восстановить штатный режим доступа.** **[FULL]**

```cpp
/** Запрос восстановления доступа.
* @param unsigned char reader_num - номер считывателя (реле).
* @return void
*/
void ceres_q_access_reset(unsigned char addr, unsigned char reader_num);

/** Обработка ответа.
* @param unsigned char reader_num - номер считывателя (реле).
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_access_reset(unsigned char addr, unsigned char reader_num);
```

**4. Предоставить доступ пользователю по предъявленному идентификатору.** **[FULL]**

```cpp
/** Запрос для предоставления доступа пользователю по предъявленному идентификатору
* @param unsigned char reader_num - номер считывателя (реле).
* @return void
*/
void ceres_q_access_grant_card(unsigned char addr, unsigned char reader_num);

/** Обработка ответа.
* @param unsigned char reader_num - номер считывателя (реле).
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_access_grant_card(unsigned char addr, unsigned char reader_num);
```

**5. Запретить доступ пользователю по предъявленному идентификатору.** **[FULL]**

```cpp
/** Запрос для запрета доступа пользователю по предъявленному идентификатору
* @param unsigned char reader_num - номер считывателя (реле).
* @return void
*/
void ceres_q_access_deny(unsigned char addr, unsigned char reader_num);

/** Обработка ответа.
* @param unsigned char reader_num - номер считывателя (реле).
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_access_deny(unsigned char addr);
```

**6. Предоставить доступ однократно (открыть дверь).** **[FULL]**

```cpp
/** Запрос для предоставления однократного доступа
* @param unsigned char reader_num - номер считывателя (реле).
* @return void
*/
void ceres_q_access_grant_noname(unsigned char addr, unsigned char reader_num);

/** Обработка ответа.
* @param unsigned char reader_num - номер считывателя (реле).
* @return
* 0 - успешно.
* 1 - ошибка команды.
* -1 - ошибка принятых данных.
*/
char ceres_r_access_grant_noname(unsigned char addr, unsigned char reader_num);
```

### Запрос информации о шлейфе в Unicode строку. **[FULL]**

Данные запросы применяется для получения информации:

+ о температуре и влажности получаемых от С2000-ВТ,  
+ о напряжении, токе и т.д. от РИП12-RS,
+ и т.д.
  
```cpp

/*Версия 1*/

/** Запрос информации о шлейфе в Unicode строку.
* @param unsigned char zone - номер шлейфа.
* @return void
*/
void ceres_q_adc_v1(unsigned char addr, unsigned char zone);


/** Обработка ответа.
* @return
* Указатель на нуль-терминированную строку данных, в случае успеха.
* null - ошибка принятых данных.
*/
unsigned char* ceres_r_adc_v1(unsigned char addr);


/*Версия 2*/

/** Запрос информации о шлейфе в Unicode строку.
* @param unsigned char zone - номер шлейфа.
* @return void
*/
void ceres_q_adc_v2(unsigned char addr, unsigned char zone);


/** Обработка ответа.
* @return
* Указатель на нуль-терминированную строку данных, в случае успеха.
* null - ошибка принятых данных.
*/
unsigned char* ceres_r_adc_v2(unsigned char addr);
```

Для удобства реализованы 2 функции для извлечения значений типа long int и double из полученной Unicode строки. **[FULL]**

```cpp
/** Извлекает знаковое значение типа long int из Unicode строки.
* @param unsigned char* frame - Нуль-терминированная С-строка, после обработки ответа функциями ceres_r_adc_v1(_v2).
* @param long int* dest - указатель на переменную для сохранения значения.
* @return void
*/
void ceres_extract_adc(unsigned char* frame, long int* dest);

/** Извлекает значение типа double из Unicode строки.
* @param unsigned char* frame - Нуль-терминированная С-строка, после обработки ответа функциями ceres_r_adc_v1(_v2).
* @param double* dest - указатель на переменную для сохранения значения.
* @return void
*/
void ceres_extract_adc(unsigned char* frame, double* dest);
```

### Запрос данных счетчика. **[FULL]**

```cpp
/** Запрос данных счетчика.
* @param unsigned char zone - адрес счетчика (номер зоны).
* @return void
*/
void ceres_q_counter(unsigned char addr, unsigned char zone);


/** Обработка ответа.
* @param unsigned char zone - адрес счетчика (номер зоны).
* @param long long unsigned int* - указатель на переменную для сохранения значения счетчика.
* @return
* 0 - успешно.
* -1 - ошибка принятых данных.
*/
char ceres_r_counter(unsigned char addr, unsigned char zone, long long unsigned int* counter_dest);
```

### Текстовые значения.

```cpp
/** Получение строки с названием прибора. Кириллица.
* @param unsigned char event - тип прибора.
* @return 
* Указатель на строку данных.
*/
const char* ceres_name_dev(unsigned char type);

/** Получение строки с именем события. Кириллица.
* @param unsigned char event - код события.
* @return 
* Указатель на строку данных.
*/
const char* ceres_name_event(unsigned char event);

/** Получение строки с именем события. Латиница. Транслит.
* @param unsigned char event - код события.
* @return 
* Указатель на строку данных.
*/
const char* ceres_name_event_trnslt(unsigned char event);
```

### Приоритет события.

```cpp
/** Получение приоритета события.
* @param unsigned char event - код события.
* @return 
* Числовое значение приоритета события.
*/
unsigned char ceres_event_priority(unsigned char event);
```
