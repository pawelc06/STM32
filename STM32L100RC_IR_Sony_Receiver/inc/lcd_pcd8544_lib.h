//******************************************************************************
//    THE SOFTWARE INCLUDED IN THIS FILE IS FOR GUIDANCE ONLY.
//    AUTHOR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
//    OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
//    FROM USE OF THIS SOFTWARE.
//
//    PROGRAM ZAWARTY W TYM PLIKU PRZEZNACZONY JEST WYLACZNIE
//    DO CELOW SZKOLENIOWYCH. AUTOR NIE PONOSI ODPOWIEDZIALNOSCI
//    ZA ZADNE EWENTUALNE, BEZPOSREDNIE I POSREDNIE SZKODY
//    WYNIKLE Z JEGO WYKORZYSTANIA.
//******************************************************************************


#define LCD_CTRL   	  	GPIOB		//定义TFT数据端口
//#define LCD_DATA   	  	GPIOD		//定义TFT数据端口

#define LCD_CS        	GPIO_Pin_12  //MCU_PB2--->>TFT --PIN_CS
#define LCD_SCL        	GPIO_Pin_13	//PB13--->>TFT --SCL

#define LCD_SDA        	GPIO_Pin_15	//PB15 MOSI--->>TFT --SDA
#define LCD_SDO        	GPIO_Pin_14	//PB14 MISO--->>TFT --SDA

#define LCD_RS         	GPIO_Pin_10	//PB11--->>TFT --RS
#define LCD_RST     	GPIO_Pin_0	//PB10--->>TFT --RST
/* --------------------- */

#define LCDN_GPIO GPIOB      //LCD GPIO port
#define LCDN_SPI  SPI2       //LCD SPI port

#define LCDN_CLK GPIO_Pin_13  //SPI SCK  = LCD CLK line
#define LCDN_DIN GPIO_Pin_14  //SPI MISO = LCD DIN line
#define LCDN_DOU GPIO_Pin_15  //SPI MOSI  - unused, only MCU->LCD communication
#define LCDN_SCE GPIO_Pin_12  //SPI SS   = LCD SCE line
#define LCDN_RES GPIO_Pin_0  //           LCD RES line
#define LCDN_DC  GPIO_Pin_10  //           LCD D/C line

void LCDN_HwConfig(void);
void LCDN_Init(void);
void LCDN_Mode(unsigned char mode);
void LCDN_Clear(void);
void LCDN_WriteChar(unsigned char charCode);
void LCDN_Write_Dec(unsigned int b);
void LCDN_WriteXY(const unsigned char *text, char x, char y);
void LCDN_SetPos(unsigned char x, unsigned char y);
void LCDN_SetPosG(unsigned char x, unsigned char y);
void LCDN_WriteBMP(const unsigned char *buffer);
void LCDN_SPI_Transmit(char cData);
//void LCDN_SPI_CS_Enable(void);
//void LCDN_SPI_CS_Disable(void);
