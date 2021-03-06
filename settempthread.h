#ifndef _TEMP_THREAD_H_
#define _TEMP_THREAD_H_

#include "cliutils.h"
#include "helmos-andor2k/cpp_socket.hpp"
#include <QApplication>
#include <QStringList>
#include <QThread>
#include <cstdio>
#include <cstring>
#include <QWidget>
#include <exception>

class SetTempThread : public QThread {
  Q_OBJECT

public:
  explicit SetTempThread(andor2k::ClientSocket *sock, QWidget* widg) : socket(sock), widget(widg) {};

private:
  andor2k::ClientSocket *socket;
  QWidget *widget;

  void run() override {
    QStringList list;
    QString val;
    char buffer[1024];

    // set to waiting mode ...
    //QApplication::setOverrideCursor(Qt::WaitCursor);
 
    // keep updating menu while server responds
    bool server_done = false;
    while (!server_done) {
      std::memset(buffer, 0, 1024);
      if (socket->recv(buffer, 1024) < 0) {
        printf("--- ERROR failed to receiver server message ---\n");
      }
#ifdef DEBUG
      printf("<----- Server status: [%s]\n", buffer);
#endif

      if (!std::strncmp(buffer, "done", 4) ||
          std::strstr(buffer, "done") != nullptr) {
        server_done = true;
#ifdef DEBUG
        printf("server signaled work done!\n");
#endif
      } else {
        emit newResponseReady(QString(buffer));
      }
    }

    //QApplication::restoreOverrideCursor();
    emit resultReady();
  }

signals:
  void resultReady();
  void newResponseReady(const QString &);
}; // setTempThread

#endif
