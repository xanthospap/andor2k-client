#ifndef TEMPERATURETAB_H
#define TEMPERATURETAB_H

#include "helmos-andor2k/cpp_socket.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class TemperatureTab : public QWidget {
  Q_OBJECT
public:
  explicit TemperatureTab(andor2k::ClientSocket *&sock, char *sbuf,
                          QWidget *parent = nullptr);

  void createGui();

  QPushButton *m_set_button;
  QLineEdit *m_set_temp_ledit;
  QLineEdit *m_show_temp_ledit;
  QLabel *m_label;
  QGridLayout *m_layout;

  andor2k::ClientSocket **csock;
  char *buffer = nullptr;

signals:

private slots:
  void set_temperature();
};

#endif // TEMPERATURETAB_H
