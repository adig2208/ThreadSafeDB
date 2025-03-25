#ifndef DATABASE_H
#define DATABASE_H

struct db_record {
    char record_name[31];
    int status;
};

extern struct db_record db_table[200];

int db_write(char *name, char *data, int len);
int db_read(char *name, char *buf);
int db_delete(char *name);
int count_valid_objects();
void db_cleanup(void);

#endif
