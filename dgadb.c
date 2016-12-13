#include <stdio.h>
#include <db.h>
#include <unistd.h>
#include <stdlib.h>
#include "global.h"

int main(){
  DB *dbp; /* DB structure handle */
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
   perror("open database failed");
   exit(EXIT_FAILURE);
  }


  /* When we're done with the database, close it. */
if (dbp != NULL)
 dbp->close(dbp, 0);
}
                 
