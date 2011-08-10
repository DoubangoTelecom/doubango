#include <errno.h>


int errno;

int *_errno(void)
{
  return (&errno);
}
