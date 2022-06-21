#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_posix.h>
#include <dfs_select.h>
#include <arpa/inet.h>
#include <cJSON.h>
#include <agile_console.h>
#include <easyflash.h>
#include <time.h>
#include <netdev.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>

RTM_EXPORT( __errno);
RTM_EXPORT(perror);
RTM_EXPORT(cfsetispeed);
RTM_EXPORT(fprintf);
RTM_EXPORT(cfsetospeed);
RTM_EXPORT(tcgetattr);
RTM_EXPORT(_impure_ptr);
RTM_EXPORT(fputc);
RTM_EXPORT(tcsetattr);
RTM_EXPORT(vfprintf);

//stdio
RTM_EXPORT(fopen);
RTM_EXPORT(fseek);
RTM_EXPORT(fread);
RTM_EXPORT(ftell);
RTM_EXPORT(fclose);
RTM_EXPORT(rewind);

//stdlib
RTM_EXPORT(malloc);
RTM_EXPORT(free);

// string
RTM_EXPORT(atoi);
RTM_EXPORT(strtok);
RTM_EXPORT(strerror);
RTM_EXPORT(strncat);

//termios
RTM_EXPORT(tcflush);


// rtc
RTM_EXPORT(set_date);
RTM_EXPORT(set_time);

// time
//RTM_EXPORT(localtime_r);

// netdev
RTM_EXPORT(netdev_default);

// dfs
RTM_EXPORT(select);

// netdev
RTM_EXPORT(netdev_ipaddr_addr);
RTM_EXPORT(netdev_ip4addr_ntoa);

// cJSON
RTM_EXPORT(cJSON_Parse);
RTM_EXPORT(cJSON_Print);
RTM_EXPORT(cJSON_PrintUnformatted);
RTM_EXPORT(cJSON_PrintBuffered);
RTM_EXPORT(cJSON_Delete);
RTM_EXPORT(cJSON_GetArraySize);
RTM_EXPORT(cJSON_GetArrayItem);
RTM_EXPORT(cJSON_GetObjectItem);
RTM_EXPORT(cJSON_GetErrorPtr);
RTM_EXPORT(cJSON_CreateNull);
RTM_EXPORT(cJSON_CreateTrue);
RTM_EXPORT(cJSON_CreateFalse);
RTM_EXPORT(cJSON_CreateBool);
RTM_EXPORT(cJSON_CreateNumber);
RTM_EXPORT(cJSON_CreateString);
RTM_EXPORT(cJSON_CreateArray);
RTM_EXPORT(cJSON_CreateObject);
RTM_EXPORT(cJSON_CreateIntArray);
RTM_EXPORT(cJSON_CreateFloatArray);
RTM_EXPORT(cJSON_CreateDoubleArray);
RTM_EXPORT(cJSON_CreateStringArray);
RTM_EXPORT(cJSON_AddItemToArray);
RTM_EXPORT(cJSON_AddItemToObject);
RTM_EXPORT(cJSON_AddItemToObjectCS);
RTM_EXPORT(cJSON_AddItemReferenceToArray);
RTM_EXPORT(cJSON_AddItemReferenceToObject);
RTM_EXPORT(cJSON_DetachItemFromArray);
RTM_EXPORT(cJSON_DeleteItemFromArray);
RTM_EXPORT(cJSON_DetachItemFromObject);
RTM_EXPORT(cJSON_DeleteItemFromObject);
RTM_EXPORT(cJSON_InsertItemInArray);
RTM_EXPORT(cJSON_ReplaceItemInArray);
RTM_EXPORT(cJSON_ReplaceItemInObject);
RTM_EXPORT(cJSON_Duplicate);
RTM_EXPORT(cJSON_ParseWithOpts);
RTM_EXPORT(cJSON_Minify);

// agile_console
RTM_EXPORT(agile_console_backend_register);

// easyflash
RTM_EXPORT(ef_get_env_blob);
RTM_EXPORT(ef_set_env_blob);
RTM_EXPORT(ef_set_env);
RTM_EXPORT(ef_del_env);
RTM_EXPORT(ef_env_set_default);
