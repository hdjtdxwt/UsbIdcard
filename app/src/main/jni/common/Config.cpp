#include "Config.h"
#include <stddef.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include "Android_log.h"

/*
 * 去除字符串右端的空格
 */

char *strtrimr(char *pstr) {
	int i;
	i = strlen(pstr) - 1;
	while (isspace(pstr[i]) && (i >= 0))
		pstr[i--] = '\0';
	return pstr;
}
/*
 * 去除字符串左端的空格
 */
char *strtriml(char *pstr) {
	int i = 0, j;
	j = strlen(pstr) - 1;
	while (isspace(pstr[i]) && (i <= j)) {
		i++;
	}
	if (0 < i) {
		strcpy(pstr, &pstr[i]);
	}
	return pstr;
}
/*
 * 去除字符串左右端的空格
 */
char *strtrim(char *pstr) {
	char *p = strtrimr(pstr);
	return strtriml(p);
}
/*
 * 从配置文件读出key和value,并返回（成功时返回值为0)
 */
int get_item_from_line(char *line, ITEM*item) {
	char *p = strtrim(line);
	int len = strlen(p);
	if (len == 0) {
		return 1; //空行
	} else if (p[0] == '#') {
		return 2; //注释行
	} else {
		char *p2 = strchr(p, '=');
		*p2++ = '\0';
		item->key = (char*) malloc(strlen(p) + 1);
		item->value = (char*) malloc(strlen(p2) + 1);
		strcpy(item->key, p);
		strcpy(item->value, p2);
		return 0; //查询成功
	}
}
int file_to_items(const char *file, ITEM *items, int *num) {
	char line[1024];
	FILE *fp;
	fp = fopen(file, "r");
	if (fp == NULL)
		return 1;
	int i = 0;
	while (fgets(line, 1023, fp)) {
		char *p = strtrim(line);
		int len = strlen(p);
		if (len <= 0) {
			continue;
		} else if (p[0] == '#') {
			continue;
		} else {
			char *p2 = strchr(p, '=');
			if (p2 == NULL) {
				continue;
			}
			*p2++ = '\0';
			items[i].key = (char*) malloc(strlen(p) + 1);
			strcpy(items[i].key,p);
			items[i].value = (char*) malloc(strlen(p2) + 1);
			strcpy(items[i].value,p2);
			i++;
		}
	}
	*num = i;
	fclose(fp);
	return 0;
}
/*
 * 从文件里读取key对应的value，成功则返回0
 */
int read_conf_value(const char*key, const char *value, const char*file) {
	char line[1024];
	FILE *fp;
	fp = fopen(file, "r");
	if (fp == NULL)
		return 1;
	while (fgets(line, 1023, fp)) {
		ITEM item;
		if (get_item_from_line(line, &item) == 0) {
			if (!strcmp(item.key, key)) {
				strcpy((char*)value, item.value);
				fclose(fp);
				free(item.key);
				free(item.value);
				return 0; //打到指定key对应的value
			}
		}
	}
	return 1;
}
/*
 * 更新配置项的值
 */
int update_conf_value(const char*key, const char*value, const char *file) {
	ITEM items[20]; //默认最多支持20个配置项
	int num = 0;
	file_to_items(file, items, &num);
	if (num > 0) {
		int i = 0;
		for (i = 0; i < num; i++) {
			if (!strcmp(items[i].key, key)) { //找到对应key，则更新
				strcpy(items[i].value, value);
				break;
			}
		}
		if (i == num) { //没找到
			for (i = 0; i < num; i++) {
				free(items[i].key); //释放内存
				free(items[i].value); //释放内存
			}
			return 1;
		}
		FILE *fp;
		fp = fopen(file, "w");
		if (fp == NULL) {
			for (i = 0; i < num; i++) {
				free(items[i].key); //释放内存
				free(items[i].value); //释放内存
			}
			return 1;
		}
		i = 0;
		for (i = 0; i < num; i++) {
			fprintf(fp, "%s=%s\n", items[i].key, items[i].value);

			free(items[i].key); //释放内存
			free(items[i].value); //释放内存
		}
		fclose(fp);
		return 0;
	}
	return 1;
}
/*
 * 添加配置项,如配置项存在，则进行更新
 */
int write_conf_value(const char*key, const char*value, const char *file) {
	if (update_conf_value(key, value, file) == 0) { //先进行更新操作
		return 0;
	}
	FILE *fp;
	fp = fopen(file, "a+");
	if (fp == NULL) {
		return 1;
	}
	fprintf(fp, "%s=%s\n", key, value);
	fclose(fp);
	return 0;
}
