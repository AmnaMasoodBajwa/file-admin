#include <QApplication>
#include "window.h"

int main(int argc, char* argv[])
{
  QApplication application(argc, argv);
  Window window;
  window.show();
  application.exec();

  return 0;  
}
