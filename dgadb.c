/*

 BerkleyDB test app, call it with two args: one for the key and one for the value.
 the app will search the database for key and will print its value. In case its missing it will add the pair.

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

  if(argc != 3){
    printf("dga BDB test app\nusage:%s key value\n", argv[0]);
    exit(EXIT_SUCCESS);
  }

  char *keyname = argv[1], *verdict = argv[2];
  
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

 /* this is for test only, pointing argv as the key, value
    the key, values are arrays
 */

  key.data = keyname;
  key.size = strlen(keyname) + 1;

  data.data = verdict;
  data.size = strlen(verdict) + 1;

  /* trying to retrieve the data */
  if((ret = dbp->get(dbp, NULL, &key, &data, 0)) == 0){
    printf("we have hit !!!, key:%s value:%s\n", (char *)key.data, (char *)data.data);
    /* assuming edit distance return an int */
    ret = atoi(data.data);
    /* data extracted, we can safely close the db */
    if(dbp != NULL){
      dbp->close(dbp, 0);
    }

    return ret;

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

  return EXIT_SUCCESS;
}                
