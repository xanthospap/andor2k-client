#ifndef ADVANCEDSETTINGS_H
#define ADVANCEDSETTINGS_H

#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include "helmos-andor2k/cpp_socket.hpp"

class AdvancedSettings : public QWidget {
  Q_OBJECT
public:
  explicit AdvancedSettings(andor2k::ClientSocket *&sock, QWidget *parent = nullptr);

private:
  void createGui();
  
  andor2k::ClientSocket **csock;
  char buffer[1024];

  QComboBox *m_acquisition_mode;
  QVBoxLayout *main_layout;
  QPushButton *m_set_btn

signals:
};

#endif // ADVANCEDSETTINGS_H
