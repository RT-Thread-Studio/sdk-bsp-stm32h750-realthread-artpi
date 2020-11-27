#ifndef __WANGYIYUN_H__
#define __WANGYIYUN_H__

#define NETEASE_MAX_SONG_NUM 30

typedef struct
{
    char *song_id;
    char *song_name;
} netease_song_data_t;

typedef struct
{
    int  song_data_num;
    netease_song_data_t data[NETEASE_MAX_SONG_NUM];
} netease_song_list_t;

extern netease_song_list_t *songs_list;

void get_songs_info(void);
void free_songs_info(void);

#endif /* __WANGYIYUN_H__ */
