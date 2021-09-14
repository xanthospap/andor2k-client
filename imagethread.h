#ifndef _IMAGETHREAD_H
#define _IMAGETHREAD_H

#include "cliutils.h"
#include "helmos-andor2k/cpp_socket.hpp"
#include <QApplication>
#include <QStringList>
#include <QThread>
#include <cstdio>
#include <cstring>
#include <QWidget>
#include <exception>

class ImageThread : public QThread {
  Q_OBJECT

public:
  explicit ImageThread(andor2k::ClientSocket *sock, QWidget* widg) : socket(sock), widget(widg){};

private:
  andor2k::ClientSocket *socket;
  QWidget *widget;

  void run() override {
    QStringList list;
    QString val;
    char buffer[1024];

    /*
    try {
      andor2k::ClientSocket client_socket("127.0.0.1", 8081);
      std::strcpy(buffer, "Hallo from parallel world!\n");
      client_socket.send(buffer);
    } catch (std::exception& e) {
      printf("<%s> exception caught: %s\n", __func__, e.what());
    }
    */

    // set cursor to waiting mode
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    // widget->setEnabled(false);

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
    // QApplication::restoreOverrideCursor();
    // widget->setEnabled(true);

    // emit resultReady signal
    emit resultReady(QString(buffer));

    printf("--> Closing listening thread on client <--\n");
  }

signals:
  void resultReady(const QString &);
  void newResponseReady(const QString &);
}; // imageThread

#endif
