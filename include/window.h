#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

struct Member;

class Window : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit Window(QWidget* parent = 0, Qt::WindowFlags flags = 0);
  ~Window();

private:
  void SetupMainMenu();
  void SetupFileMenu();
  void SetupEditMenu();
  void SetupViewMenu();
  void SetupHelpMenu();

  void SetupWindowContents();

  
private:
  Member* data_;
};

#endif //WINDOW_H
