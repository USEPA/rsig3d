#include <stdio.h>
#include "gpc.h"

int main(void)
{
  gpc_polygon subject, clip, result;
  FILE *sfp, *cfp, *ofp;

  sfp= fopen("subjfile", "r");
  cfp= fopen("clipfile", "r");
  gpc_read_polygon(sfp, 1, &subject);
  gpc_read_polygon(cfp, 1, &clip);

  gpc_polygon_clip(GPC_INT, &subject, &clip, &result);

  ofp= fopen("outfile", "w");
  gpc_write_polygon(ofp, 1, &result);

  gpc_free_polygon(&subject);
  gpc_free_polygon(&clip);
  gpc_free_polygon(&result);

  fclose(sfp);
  fclose(cfp);
  fclose(ofp);
  return 0;
}

