#ifndef KEYPAD_INTERFACE_H_
#define KEYPAD_INTERFACE_H_

#define KEYPAD_NO_KEY	0x00


void HAL_Keypad_voidInit(void);
u8   HAL_Keypad_u8GetKey(void);



#endif /* KEYPAD_INTERFACE_H_ */