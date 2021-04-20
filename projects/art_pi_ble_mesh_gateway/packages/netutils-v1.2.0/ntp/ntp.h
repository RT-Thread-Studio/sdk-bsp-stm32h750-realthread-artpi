/*
 * File      : ntp.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-02-10     armink       the first version
 * 2020-07-21     Chenxuan     C++ support
 */

#ifndef __NTP_H__
#define __NTP_H__

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get the UTC time from NTP server
 *
 * @param host_name NTP server host name, NULL: will using default host name
 *
 * @note this function is not reentrant
 *
 * @return >0: success, current UTC time
 *         =0: get failed
 */
time_t ntp_get_time(const char *host_name);

/**
 * Get the local time from NTP server
 *
 * @param host_name NTP server host name, NULL: will using default host name
 *
 * @return >0: success, current local time, offset timezone by NTP_TIMEZONE
 *         =0: get failed
 */
time_t ntp_get_local_time(const char *host_name);

/**
 * Sync current local time to RTC by NTP
 *
 * @param host_name NTP server host name, NULL: will using default host name
 *
 * @return >0: success, current local time, offset timezone by NTP_TIMEZONE
 *         =0: sync failed
 */
time_t ntp_sync_to_rtc(const char *host_name);

#ifdef __cplusplus
}
#endif

#endif /* __NTP_H__ */
