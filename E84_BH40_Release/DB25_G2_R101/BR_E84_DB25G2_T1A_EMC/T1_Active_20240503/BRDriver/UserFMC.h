//void FMC_RW(void);
//uint32_t FMC_R(uint32_t u32StartAddr);
uint32_t FMC_R(void);
//void FMC_W(uint32_t u32StartAddr, uint32_t  u32Data);
void FMC_W(uint32_t u32Pattern);

//int32_t  VerifyData(uint32_t u32StartAddr, uint32_t u32EndAddr, uint32_t u32Pattern);

//#define APROM_TEST_BASE             0x3000
//#define APROM_TEST_END              0xF000
#define DATA_FLASH_TEST_BASE       0x11000-0x200//0x10FFF-8
#define DATA_FLASH_TEST_END         0x11000//0x10FFF-8
