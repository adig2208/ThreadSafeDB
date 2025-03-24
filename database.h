#ifndef DATABASE_H
#define DATABASE_H

int db_write(char *name, char *data, int len);
int db_read(char *name, char *buf);
int db_delete(char *name);
#endif
