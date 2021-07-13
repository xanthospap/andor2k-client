#ifndef CONNECTIONTAB_H
#define CONNECTIONTAB_H

#include "helmos-andor2k/cpp_socket.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class ConnectionTab : public QWidget {
  Q_OBJECT
public:
  explicit ConnectionTab(andor2k::ClientSocket *sock,
                         QWidget *parent = nullptr);

private:
  void createGui();

  andor2k::ClientSocket *csock = nullptr;

  QPushButton *m_connect_button;
  QPushButton *m_disconnect_button;
  QPushButton *m_defaults_button;
  QLineEdit *m_hostname_ledit;
  QLineEdit *m_port_ledit;
  QGridLayout *g_edits_layout;
  QHBoxLayout *h_button_layout;
  QVBoxLayout *v_main_layout;

signals:

private slots:
  void sock_connect();
  void reset_defaults();
  void disconnect();
};

#endif // CONNECTIONTAB_H
