#ifndef ADVANCEDSETTINGS_H
#define ADVANCEDSETTINGS_H

#include "helmos-andor2k/cpp_socket.hpp"
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <chrono>

class AdvancedSettings : public QWidget {
  Q_OBJECT
public:
  explicit AdvancedSettings(andor2k::ClientSocket *&sock,
                            QWidget *parent = nullptr);
private:
  int make_acquisition_command(char *buffer);
  int make_temp_command(char *buffer);
  int check_socket();
  int send_command(const char *command);
  void createGui();
  int make_command(char *);
  void enable_temperature_edits(bool);

  andor2k::ClientSocket **csock;
  std::chrono::system_clock::time_point start_set_temp;

  QComboBox *m_acquisition_mode;
  QLineEdit *m_kinetic_ct;
  
  QLineEdit *m_set_temp;
  QLineEdit *m_ctemp;
  QLineEdit *m_temp_status;
  QLineEdit *m_temp_et;
  
  QVBoxLayout *main_layout;
  QPushButton *m_set_btn;
  QPushButton *m_settmp_btn;

signals:

private slots:
  void send_acquisition_command();
  void send_temperature_command();
  void serverJobDone(const QString &);
  void serverJobUpdate(const QString &);
};

#endif // ADVANCEDSETTINGS_H
