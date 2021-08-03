#ifndef STATUSTAB_H
#define STATUSTAB_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include "helmos-andor2k/cpp_socket.hpp"

class StatusTab : public QWidget {
  Q_OBJECT
public:
  explicit StatusTab(andor2k::ClientSocket *&sock, char *sbuf, QWidget *parent = nullptr);

  void createGui();

  QLineEdit *m_status_string;
  QLineEdit *m_temperature;
  QLineEdit *m_last_update;
  QLabel *m_label;
  QGridLayout *m_layout;
  
  andor2k::ClientSocket **csock;
  char *buffer = nullptr;
  char status_buf[1024];

signals:

private slots:
  // void get_status();
};

#endif // STATUSTAB_H
