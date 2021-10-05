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
  explicit ConnectionTab(andor2k::ClientSocket *&sock,
                         QWidget *parent = nullptr);

private:
  andor2k::ClientSocket **csock;
  char buffer[1024];

  QPushButton *m_connect_button;
  QPushButton *m_disconnect_button;
  QPushButton *m_defaults_button;
  QPushButton *m_shutdown_button;
  QPushButton *m_edit_button;
  QPushButton *m_update_button;

  QLineEdit *m_hostname_ledit;
  QLineEdit *m_port_ledit;
  QLineEdit *m_init_temp;
  QLineEdit *m_observer;

  QLineEdit *m_temp_c;
  QLineEdit *m_con_status;
  QLineEdit *m_server_info;
  QLineEdit *m_time;

  QGridLayout *g_edits_layout;
  QHBoxLayout *h_button_layout;
  QVBoxLayout *v_main_layout;

  void createGui();
  void send_settemp();
  void setEditable();
  void setUnEditable();

signals:

private slots:
  void get_status();
  void sock_connect();
  void reset_defaults();
  void make_editable();
  void disconnect();
  void shutdown_daemon();
  void serverJobDone();
  void serverJobUpdate(const QString &);
};

#endif // CONNECTIONTAB_H
