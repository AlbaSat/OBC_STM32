/* USER CODE BEGIN Header */
/**
  * @file    user_diskio.c
  * @brief   This file implements a RAM disk driver for FATFS.
  * @attention
  *          Copyright (c) 2024 STMicroelectronics.
  *          All rights reserved.
  *          This software is licensed under terms that can be found in the LICENSE file
  *          in the root directory of this software component.
  *          If no LICENSE file comes with this software, it is provided AS-IS.
  */
 /* USER CODE END Header */

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/*
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future.
 * Kept to ensure backward compatibility with previous CubeMx versions when
 * migrating projects.
 * User code previously added there should be copied in the new user sections before
 * the section contents can be deleted.
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include "ff_gen_drv.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Define the RAM disk size */
#define RAM_DISK_SECTOR_SIZE  512          // Define the sector size (512 bytes, typical for FATFS)
#define RAM_DISK_TOTAL_SECTORS 150           // Define total sectors (150 * 512 = 75kB)

/* Private variables ---------------------------------------------------------*/
static volatile DSTATUS Stat = STA_NOINIT;  // Disk status variable

/* Allocate memory to act as the RAM disk (size: 75kB) */
static BYTE ram_disk[RAM_DISK_SECTOR_SIZE * RAM_DISK_TOTAL_SECTORS];

/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read,
#if  _USE_WRITE
  USER_write,
#endif  /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_initialize (
	BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
  /* USER CODE BEGIN INIT */
  if (pdrv == 0) {
    // Clear the memory (optional but recommended for safety)
    memset(ram_disk, 0, sizeof(ram_disk));
    Stat = 0;  // Disk ready (no error)
  }

  return Stat;
  /* USER CODE END INIT */
}

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_status (
	BYTE pdrv       /* Physical drive number to identify the drive */
)
{
  /* USER CODE BEGIN STATUS */
  if (pdrv == 0) {
    return Stat;  // Always return the current status
  }

  return STA_NOINIT;  // Return not initialized for other drives
  /* USER CODE END STATUS */
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USER_read (
	BYTE pdrv,      /* Physical drive nmuber to identify the drive */
	BYTE *buff,     /* Data buffer to store read data */
	DWORD sector,   /* Sector address in LBA */
	UINT count      /* Number of sectors to read */
)
{
  /* USER CODE BEGIN READ */
  if (pdrv != 0) {
    return RES_PARERR;  // Only support one drive (drive 0)
  }

  if (Stat & STA_NOINIT) {
    return RES_NOTRDY;  // Disk not initialized
  }

  if (sector + count > RAM_DISK_TOTAL_SECTORS) {
    return RES_PARERR;  // Out of bounds
  }

  // Copy data from the RAM disk to the provided buffer
  memcpy(buff, ram_disk + (sector * RAM_DISK_SECTOR_SIZE), count * RAM_DISK_SECTOR_SIZE);

  return RES_OK;  // Success
  /* USER CODE END READ */
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USER_write (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	const BYTE *buff,   /* Data to be written */
	DWORD sector,       /* Sector address in LBA */
	UINT count          /* Number of sectors to write */
)
{
  /* USER CODE BEGIN WRITE */
  if (pdrv != 0) {
    return RES_PARERR;  // Only support one drive (drive 0)
  }

  if (Stat & STA_NOINIT) {
    return RES_NOTRDY;  // Disk not initialized
  }

  if (sector + count > RAM_DISK_TOTAL_SECTORS) {
    return RES_PARERR;  // Out of bounds
  }

  // Copy data from the provided buffer to the RAM disk
  memcpy(ram_disk + (sector * RAM_DISK_SECTOR_SIZE), buff, count * RAM_DISK_SECTOR_SIZE);

  return RES_OK;  // Success
  /* USER CODE END WRITE */
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USER_ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
)
{
  /* USER CODE BEGIN IOCTL */
  if (pdrv != 0) {
    return RES_PARERR;  // Only support one drive (drive 0)
  }

  DRESULT res = RES_ERROR;

  switch (cmd) {
    case CTRL_SYNC:
      // No action needed for RAM disk, always synchronized
      res = RES_OK;
      break;

    case GET_SECTOR_COUNT:
      // Return the total number of sectors
      *(DWORD*)buff = RAM_DISK_TOTAL_SECTORS;
      res = RES_OK;
      break;

    case GET_SECTOR_SIZE:
      // Return the sector size
      *(WORD*)buff = RAM_DISK_SECTOR_SIZE;
      res = RES_OK;
      break;

    case GET_BLOCK_SIZE:
      // Return the block size in units of sectors (typically 1 for RAM disk)
      *(DWORD*)buff = 1;  // Block size of 1 sector
      res = RES_OK;
      break;

    default:
      res = RES_PARERR;  // Invalid command
      break;
  }

  return res;
  /* USER CODE END IOCTL */
}
#endif /* _USE_IOCTL == 1 */

