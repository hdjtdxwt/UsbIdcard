#ifndef _CONFIG_H
#define _CONFIG_H
typedef struct item_t {
	char *key;
	char *value;
} ITEM;
char *strtrimr(char *pstr);
char *strtriml(char *pstr);
char *strtrim(char *pstr);
int get_item_from_line(char *line, ITEM*item);
int file_to_items(const char *file,ITEM *items,int *num);
int read_conf_value(const char*key,const char *value,const char*file);
int update_conf_value(const char*key,const char*value,const char *file);
int write_conf_value(const char*key,const char*value,const char *file);
#endif
