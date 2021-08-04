#ifndef EXPOSURETAB_H
#define EXPOSURETAB_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include "helmos-andor2k/cpp_socket.hpp"

class ExposureTab : public QWidget {
  Q_OBJECT
public:
  explicit ExposureTab(andor2k::ClientSocket *&sock, char *sbuf, QWidget *parent = nullptr);

private:
  void createGui();
  int make_command(char *buf);

  QPushButton *m_cancel_button;
  QPushButton *m_start_button;
  QLineEdit *m_filename_ledit;
  QLineEdit *m_exposure_ledit;
  QLineEdit *m_nimages_ledit;
  QLineEdit *m_vbin_ledit;
  QLineEdit *m_hbin_ledit;
  QLineEdit *m_vstart_pix;
  QLineEdit *m_hstart_pix;
  QLineEdit *m_vend_pix;
  QLineEdit *m_hend_pix;
  QComboBox *m_type_cbox;
  QCheckBox *m_tel_cb;
  QLineEdit *m_tel_tries;
  QLabel *m_label;
  QVBoxLayout *m_layout;
  
  andor2k::ClientSocket **csock;
  char *buffer = nullptr;

signals:

private slots:
  void set_exposure();
};

#endif // EXPOSURETAB_H
