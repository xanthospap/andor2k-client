#include "tabdialog.h"
#include <QApplication>
#include "helmos-andor2k/cpp_socket.hpp"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  
  andor2k::ClientSocket *csock {nullptr};

  TabDialog window(csock);
  window.show();
  
  printf("[DEBUG][ANDOR2K::client::%25s] Main Socket at %p" , __func__, &csock);
  if (csock) printf(" -> %p\n", csock);
  else printf(" -> nowhere!\n");

  return app.exec();
}
