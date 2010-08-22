#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSplitter>
#include <QTreeView>
#include <QStackedWidget>
#include "FileSystemModel.h"
#include <QFileSystemModel>
#include "window.h"

struct Member
{
public:
  explicit Member();
  ~Member();

public:
  QMenu* fileMenu_;
  QAction* addAction_;
  QAction* exitAction_;

  QMenu* editMenu_;
  QAction* renameAction_;

  QMenu* viewMenu_;

  QMenu* helpMenu_;


  QSplitter* splitter_;
  QTreeView* treeView_;
  QStackedWidget* stackedWidget_;
};

Member::Member()
{
  fileMenu_ = NULL;
  addAction_ = NULL;
  exitAction_ = NULL;

  editMenu_ = NULL;
  renameAction_ = NULL;

  viewMenu_ = NULL;

  helpMenu_ = NULL;


  splitter_ = NULL;
  treeView_ = NULL;
  stackedWidget_ = NULL;
}

Member::~Member()
{
  delete fileMenu_;
  delete addAction_;
  delete exitAction_;

  delete editMenu_;
  delete renameAction_;

  delete viewMenu_;

  delete helpMenu_;


  delete splitter_;
  delete treeView_;
  delete stackedWidget_;
}

namespace
{
  const char* const FILE_TEXT = QT_TRANSLATE_NOOP("Window", "File");
  const char* const ADD_TEXT = QT_TRANSLATE_NOOP("Window", "Add");
  const char* const EXIT_TEXT = QT_TRANSLATE_NOOP("Window", "Exit");

  const char* const EDIT_TEXT = QT_TRANSLATE_NOOP("Window", "Edit");
  const char* const RENAME_TEXT= QT_TRANSLATE_NOOP("Window", "Rename");

  const char* const VIEW_TEXT = QT_TRANSLATE_NOOP("Window", "View");

  const char* const HELP_TEXT = QT_TRANSLATE_NOOP("Window", "Help");
}

Window::Window(QWidget* parent, Qt::WindowFlags flags)
  :QMainWindow(parent, flags),
   data_(new Member)
{
  SetupMainMenu();
  SetupWindowContents();
}

Window::~Window()
{
}

void Window::SetupMainMenu()
{
  SetupFileMenu();
  SetupEditMenu();
  SetupViewMenu();
  SetupHelpMenu();
}

void Window::SetupFileMenu()
{
  data_->fileMenu_ = new QMenu(tr(FILE_TEXT), this);
  data_->addAction_ = data_->fileMenu_->addAction(tr(ADD_TEXT));
  data_->exitAction_ = data_->fileMenu_->addAction(tr(EXIT_TEXT));

  menuBar() -> addMenu(data_ -> fileMenu_);
}

void Window::SetupEditMenu()
{
  data_->editMenu_ = new QMenu(tr(EDIT_TEXT), this);
  data_->renameAction_ = data_->editMenu_->addAction(tr(RENAME_TEXT));

  menuBar() -> addMenu(data_ -> editMenu_);
}

void Window::SetupViewMenu()
{
  data_->viewMenu_ = new QMenu(tr(VIEW_TEXT), this);

  menuBar() -> addMenu(data_ -> viewMenu_);
}

void Window::SetupHelpMenu()
{
  data_->helpMenu_ = new QMenu(tr(HELP_TEXT), this);

  menuBar() -> addMenu(data_ -> helpMenu_);
}

void Window::SetupWindowContents()
{
  data_ -> splitter_ = new QSplitter(Qt::Horizontal, this);
  data_ -> treeView_ = new QTreeView(this);
  data_ -> stackedWidget_ = new QStackedWidget(this);

  FileSystemModel* model = new FileSystemModel("/home/yhj", this);
  data_ -> treeView_ -> setModel(model);
  
  data_ -> splitter_ -> addWidget(data_ -> treeView_);
  data_ -> splitter_ -> addWidget(data_ -> stackedWidget_);

  setCentralWidget(data_ -> splitter_);
}
