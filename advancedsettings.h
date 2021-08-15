#ifndef ADVANCEDSETTINGS_H
#define ADVANCEDSETTINGS_H

#include "helmos-andor2k/cpp_socket.hpp"
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class AdvancedSettings : public QWidget {
  Q_OBJECT
public:
  explicit AdvancedSettings(andor2k::ClientSocket *&sock,
                            QWidget *parent = nullptr);

private:
  void createGui();
  int make_command(char *);

  andor2k::ClientSocket **csock;
  char buffer[1024];

  QComboBox *m_acquisition_mode;
  QLineEdit *m_kinetic_ct;
  QVBoxLayout *main_layout;
  QPushButton *m_set_btn;

signals:

private slots:
  void send_command();
};

#endif // ADVANCEDSETTINGS_H
