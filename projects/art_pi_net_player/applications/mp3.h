#ifndef __MP3_H__
#define __MP3_H__

void http_mp3(char* url);
void play_prev(void);
void play_next(void);
void play_start(void);
void play_pause(void);
void player_init(void);
void player_deinit(void);
char *get_song_name(void);
int get_song_time(void);
void volume_set(int mode);
int volume_get(void);
int source_get(void);
void source_change(void);
void mode_change(void);
int mode_get(void);
void player_init(void);

#endif
