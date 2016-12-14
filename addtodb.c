/*

 Adding domain and edit distance results to the database

*/

#include <stdio.h>
#include <db.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

int addentry(char *domain, char *v){
  DB *dbp; /* DB structure handle */
  DBT key, data;

  char *keyname = domain, *verdict = (char *)v;
  
  // initialize key, data
  memset(&data, 0, sizeof(DBT));
  memset(&key, 0, sizeof(DBT));

  u_int32_t flags; /* database open flags */
  int ret; /* function return value */

  /* so the environment pointer is NULL. */
  ret = db_create(&dbp, NULL, 0);
  if (ret != 0) {
   dbp->err(dbp, 0, "db_create");
   return(EXIT_ERR);
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
   return EXIT_ERR;
  }

  key.data = keyname;
  key.size = strlen(keyname) + 1;

  data.data = verdict;
  data.size = strlen(verdict) + 1;

  printf("adding data information to database ...\n");
  if((ret = dbp->put(dbp, NULL, &key, &data, 0)) == 0){
    printf("caching the data, db was updated\n");
    /* we are done here
       closing the database
   */
    if (dbp != NULL){
      dbp->close(dbp, 0);
    }

    return EXIT_SUCCESS;
  }else{
      dbp->err(dbp, ret, "DB->put");
  }
  
  /* When we're done with the database, close it. */
  if (dbp != NULL){
    dbp->close(dbp, 0);
  }

  return EXIT_ERR;
}                
