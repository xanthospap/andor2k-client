#include "tabdialog.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  TabDialog window;
  window.show();

  return app.exec();
}
