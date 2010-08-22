#include "utility.h"

QString FormatFileSize(unsigned long bytes)
{
  const unsigned long KB = 1UL << 10;
  const unsigned long MB = KB << 10;
  const unsigned long GB = MB << 10;

  const unsigned int GBCount = bytes / GB;
  const unsigned int MBCount = (bytes - GBCount * GB) / MB;
  const unsigned int KBCount = (bytes - GBCount * GB - MBCount * MB) / KB;
  const unsigned int remainCount = bytes - GBCount * GB - MBCount * MB - KBCount * KB;

  const unsigned int BIT_AFTER_POINT = 2;
  const unsigned int FACTOR = 100;
  if(GBCount != 0)
    {
      return QString("%1.%2GB").arg(GBCount).arg(MBCount * FACTOR / KB);
    }
  else if(MBCount != 0)
    {
      return QString("%1.%2MB").arg(MBCount).arg(KBCount * FACTOR / KB);
    }
  else if(KBCount != 0)
    {
      return QString("%1.%2KB").arg(KBCount).arg(remainCount * FACTOR / KB);
    }
  else
    {
      return QString("%1B").arg(remainCount);
    }
}
