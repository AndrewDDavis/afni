#include "3ddata.h"
#include "thd.h"


/*****************************************************************
  A datablock contains the information needed to access the
  data in memory.
******************************************************************/

/*---------------------------------------------------------------
   Count the number of in-memory sub-bricks actually stored in
   a datablock at this instant.  Returns -1 if an error.
-----------------------------------------------------------------*/

int THD_count_databricks( THD_datablock * dblk )
{
   int ibr , count ;

ENTRY("THD_count_databricks") ;

   if( ! ISVALID_DATABLOCK(dblk) || dblk->brick == NULL ) return -1 ;

   count = 0 ;
   for( ibr=0 ; ibr < dblk->nvals ; ibr++ )
      if( DBLK_BRICK(dblk,ibr) != NULL && DBLK_ARRAY(dblk,ibr) != NULL )
        count++ ;

#ifdef THD_DEBUG
printf("  count = %d / %d\n",count,dblk->nvals) ;
#endif

   return count ;
}
