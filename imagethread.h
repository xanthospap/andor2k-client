#ifndef _IMAGETHREAD_H
#define _IMAGETHREAD_H

#include "cliutils.h"
#include "helmos-andor2k/cpp_socket.hpp"
#include <QApplication>
#include <QStringList>
#include <QThread>
#include <cstdio>
#include <cstring>

class ImageThread : public QThread {
  Q_OBJECT

public:
  explicit ImageThread(andor2k::ClientSocket *sock) : socket(sock){};

private:
  andor2k::ClientSocket *socket;

  void run() override {
    QStringList list;
    QString val;
    char buffer[1024];

    // set cursor to waiting mode
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // keep updating menu while server responds
    bool server_done = false;
    printf("--> Starting listening thread on client <--\n");
    while (!server_done) {
      std::memset(buffer, 0, 1024);
      if (socket->recv(buffer, 1024) < 0) {
        printf("--- ERROR failed to receiver server message ---\n");
      }

      if (!std::strncmp(buffer, "done", 4) ||
          std::strstr(buffer, "done") != nullptr) {
        server_done = true;
      }

      // emit newResponseReady signal
      emit newResponseReady(QString(buffer));
    }

    // unset waiting mode on cursor
    QApplication::restoreOverrideCursor();

    // emit resultReady signal
    emit resultReady(QString(buffer));

    printf("--> Closing listening thread on client <--\n");
  }

signals:
  void resultReady(const QString &);
  void newResponseReady(const QString &);
}; // imageThread

#endif
