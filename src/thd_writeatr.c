#include "3ddata.h"
#include "thd.h"


/*---------------------------------------------------------------------*/

Boolean THD_write_atr( THD_datablock * blk )
{
   THD_diskptr * dkptr ;
   int ia , code , ii ;
   ATR_any * atr_any ;
   Boolean good = True ;
   FILE * header_file ;

   /*--- sanity checks ---*/

   if( ! ISVALID_DATABLOCK(blk) ) return False ;

   dkptr = blk->diskptr ;
   if( ! ISVALID_DISKPTR(dkptr) || strlen(dkptr->header_name) == 0 ) return False ;

   header_file = fopen( dkptr->header_name , "w" ) ;
   if( header_file == NULL ){
      fprintf(stderr,
              "*** ERROR: failed to open file %s for attribute writing\n",
              dkptr->header_name) ;
      return False ;
   }

   for( ia=0 ; ia < blk->natr ; ia++ ){

      atr_any = &(blk->atr[ia]) ;
      if( atr_any == NULL ) continue ;

      switch( atr_any->type ){
         default: code = FAIL ; break ;

         case ATR_FLOAT_TYPE:{
            ATR_float * atr_flo = (ATR_float *) atr_any ;

#ifdef THD_DEBUG
fprintf(stderr,"THD_write_atr: FLOAT %s\n",atr_flo->name);
#endif

            code = fprintf( header_file ,
                            "\ntype  = %s\nname  = %s\ncount = %d\n" ,
                            ATR_typestr[ATR_FLOAT_TYPE] ,
                            atr_flo->name , atr_flo->nfl ) ;

            if( code < 1 ){ code = FAIL ; break ; }

            for( ii=0 ; ii < atr_flo->nfl ; ii++ ){
               code = fprintf( header_file , " %14.7g" , atr_flo->fl[ii] ) ;
               if( ii % 5 == 4 && ii < atr_flo->nfl-1 )
                  fprintf( header_file , "\n" ) ;
            }

            code = (code < 1 ) ? FAIL : SUCCESS ;
         }
         break ;

         case ATR_INT_TYPE:{
            ATR_int * atr_int = (ATR_int *) atr_any ;

#ifdef THD_DEBUG
fprintf(stderr,"THD_write_atr: INT %s\n",atr_int->name);
#endif

            code = fprintf( header_file ,
                            "\ntype = %s\nname = %s\ncount = %d\n" ,
                            ATR_typestr[ATR_INT_TYPE] ,
                            atr_int->name , atr_int->nin ) ;

            if( code < 1 ){ code = FAIL ; break ; }

            for( ii=0 ; ii < atr_int->nin ; ii++ ){
               code = fprintf( header_file , " %d" , atr_int->in[ii] ) ;
               if( ii % 5 == 4 && ii < atr_int->nin-1 )
                  fprintf( header_file , "\n" ) ;
            }

            code = (code < 1 ) ? FAIL : SUCCESS ;
         }
         break ;

         case ATR_STRING_TYPE:{
            ATR_string * atr_str = (ATR_string *) atr_any ;

#ifdef THD_DEBUG
fprintf(stderr,"THD_write_atr: STRING %s\n",atr_str->name);
#endif

            code = fprintf( header_file ,
                            "\ntype = %s\nname = %s\ncount = %d\n" ,
                            ATR_typestr[ATR_STRING_TYPE] ,
                            atr_str->name , atr_str->nch ) ;


            if( code < 1 ){ code = FAIL ; break ; }

            THD_zblock( atr_str->nch , atr_str->ch ) ;

            fprintf( header_file , "'" ) ;
            for( ii=0 ; ii < atr_str->nch ; ii++ ){
               code = fprintf( header_file , "%c" , atr_str->ch[ii] ) ;
            }

            code = (code < 1) ? FAIL : SUCCESS ;

            THD_unzblock( atr_str->nch , atr_str->ch ) ;
         }
         break ;

      } /* end of switch on atr type */

      good = good && (code != FAIL) ;  /* all must not FAIL */
      fprintf(header_file,"\n") ;

#ifdef THD_DEBUG
if( code == FAIL ) fprintf(stderr," ** attribute write failure!\n") ;
#endif


   } /* end of loop over all atr's */

   if( good == False ){
      fprintf(stderr,
              "*** WARNING: error in output to attribute file %s\n",
              dkptr->header_name) ;
   }

   /*--- close it down ---*/

#ifdef THD_DEBUG
fprintf(stderr,"THD_write_atr: closing\n") ;
#endif

   fclose( header_file ) ;
   return True ;
}
