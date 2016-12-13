/*
  dga database interface, this function will create the database if its not exist
  cache and return values all ready cached
.
*/

#include <stdio.h>
#include <db.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

int main(int argc, char *argv[]){
  DB *dbp; /* DB structure handle */
  DBT key, data;

  char *keyname = argv[1], *verdict = argv[2];
  
  // initialize key, data
  memset(&data, 0, sizeof(DBT));
  memset(&key, 0, sizeof(DBT));

  u_int32_t flags; /* database open flags */
  int ret; /* function return value */

  /* Initialize the structure. This
  * database is not opened in an environment,
  * so the environment pointer is NULL. */
  ret = db_create(&dbp, NULL, 0);
  if (ret != 0) {
   /* Error handling goes here */
   perror("db_create failed");
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

 /* this is for test only, pointing argv as the key, value
    the key is an array
 */

  key.data = keyname;
  key.size = strlen(keyname) + 1;

  data.data = verdict;
  data.size = strlen(verdict) + 1;

  // trying to retrieve the data
  if((ret = dbp->get(dbp, NULL, &key, &data, 0)) == 0){
    printf("we have hit !!!, key:%s value:%s\n", (char *)key.data, (char *)data.data);
    // data extracted, we can safely close the db
    if(dbp != NULL){
      dbp->close(dbp, 0);
    }

  // return data value 
  return atoi(argv[2]);

  }else{
    // we have a miss !!!, adding values //
    dbp->err(dbp, ret, "DB->get");
    printf("adding data\n");
    if((ret = dbp->put(dbp, NULL, &key, &data, 0)) == 0){
     printf("caching the data, db was updated\n");
    }else{
      dbp->err(dbp, ret, "DB->put");
    }
  }
  
  /* When we're done with the database, close it. */
  if (dbp != NULL){
   dbp->close(dbp, 0);
  }
}                
