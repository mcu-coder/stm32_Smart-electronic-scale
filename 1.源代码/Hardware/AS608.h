#ifndef __FPMXX_H__
#define __FPMXX_H__


extern unsigned char FPM10A_RECEICE_BUFFER[32];
extern uint8_t FPM10A_RECEICE_BUFFER_INdex;
extern uint8_t AS608_Rx_Flag;

void FPM10A_Cmd_Get_Img(void);
void FPM10A_Cmd_Check(void);
void Device_Check(void);
void FPM10A_Receive_Data(unsigned char ucLength);

void FPM10A_Cmd_Search_Finger(void);

void FPM10A_Cmd_Reg_Model();
void FPM10A_Cmd_Save_Finger( unsigned int storeID );
void FINGERPRINT_Cmd_Delete_All_Finger(void);

void AS608_Add_Fingerprint();
void AS608_Find_Fingerprint();
void AS608_Delete_All_Fingerprint();

#endif