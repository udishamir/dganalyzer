/*
  search for existing key
*/

#include <stdio.h>
#include <db.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

int ventry(char *domain){
  DB *dbp; /* DB structure handle */
  DBT key, data;

  char *keyname = domain;
  
  // initialize key, data
  memset(&data, 0, sizeof(DBT));
  memset(&key, 0, sizeof(DBT));

  u_int32_t flags; /* database open flags */
  int ret; /* function return value */

  /* so the environment pointer is NULL. */
  ret = db_create(&dbp, NULL, 0);
  if (ret != 0) {
   dbp->err(dbp, 0, "db_create");
   exit(EXIT_FAILURE);
  }

  /* Database open flags */
  flags = DB_CREATE; /* If the database does not exist, create it */

  // open the database
  ret = dbp->open(dbp,
                 NULL,
                 BDB,
                 NULL,
                 DB_BTREE,
                 flags,
                 0);
  if(ret != 0){ 
    dbp->err(dbp, 0, "dbp-open");
    return EXIT_FAILURE;
  }

  key.data = keyname;
  key.size = strlen(keyname) + 1;

  /* trying to retrieve the data */
  if((ret = dbp->get(dbp, NULL, &key, &data, 0)) == 0){
    printf("we have hit !!!, key:%s value:%d\n", (char *)key.data, *(int *)data.data);
    /* assuming edit distance return an int */
    ret = *(int *)data.data;
    /* data extracted, we can safely close the db */
    if(dbp != NULL){
      dbp->close(dbp, 0);
    }

    // verdict 
    return ret;
  }

  /* close the db */
  if (dbp != NULL){
   dbp->close(dbp, 0);
  }

  /* we have a miss */
  return EXIT_ERR;
}                
