#include <stdio.h>
#include "M0518.h"
#include "fmc.h"
#include "UserFMC.h"
//#include "Motor.h"
#define TEST_PATTERN                0x00006060

#define CONFIG0_DFEN                0x01
#define LOCK						0x02
#define CONFIG0_DFVSEN              0x04

static int  SetDataFlashBase(uint32_t u32DFBA)
{
    uint32_t au32Config[2];

    FMC_ReadConfig(au32Config, 1);

    if(!(au32Config[0] & 0x1))
        return 0;

    FMC_EnableConfigUpdate();

    FMC_Erase(FMC_CONFIG_BASE);

   // au32Config[0] &= ~(CONFIG0_DFEN | CONFIG0_DFVSEN ) | LOCK;
    au32Config[0] &= ~(CONFIG0_DFEN | CONFIG0_DFVSEN ) ;

    au32Config[1] = u32DFBA;

    if(FMC_WriteConfig(au32Config, 2))
        return -1;

    SYS->IPRSTC1 |= SYS_IPRSTC1_CHIP_RST_Msk;

    return 0;
}


int32_t FillDataPattern(uint32_t u32StartAddr, uint32_t u32EndAddr, uint32_t u32Pattern)
{
    uint32_t u32Addr;
    for(u32Addr = u32StartAddr; u32Addr <= u32EndAddr; u32Addr += 4)
    {
        FMC_Write(u32Addr, u32Pattern);
    }

    return 0;
}


uint32_t FMC_R(void)
{
    uint32_t u32Data;

    SYS_UnlockReg();

    FMC_Open();

    if(SetDataFlashBase(DATA_FLASH_TEST_BASE) < 0)
    {
        goto lexit;
    }

    if(FMC_GetBootSource() == 0)
    {}
    else
    {
        goto lexit;
    }
    u32Data = FMC_ReadDataFlashBaseAddr();
    u32Data = FMC_Read(DATA_FLASH_TEST_BASE+4);

lexit:

    FMC_Close();

    SYS_LockReg();
    return u32Data;	
}
void FMC_W(uint32_t u32Pattern)
{
    uint32_t u32Data;

    SYS_UnlockReg();

    FMC_Open();

    if(SetDataFlashBase(DATA_FLASH_TEST_BASE) < 0)
    {
        goto lexit;
    }

    if(FMC_GetBootSource() == 0)
    {}
    else
    {
        goto lexit;
    }
    u32Data = FMC_ReadDataFlashBaseAddr();
    FMC_Erase(DATA_FLASH_TEST_BASE);
    if(FillDataPattern(DATA_FLASH_TEST_BASE, DATA_FLASH_TEST_BASE + FMC_FLASH_PAGE_SIZE, u32Pattern) < 0)
    {
        	goto lexit;
    }
lexit:

    FMC_Close();

    SYS_LockReg();

}
