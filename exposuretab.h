#ifndef EXPOSURETAB_H
#define EXPOSURETAB_H

#include "helmos-andor2k/cpp_socket.hpp"
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QWidget>

class ExposureTab : public QWidget {
  Q_OBJECT
public:
  explicit ExposureTab(andor2k::ClientSocket *&sock, QWidget *parent = nullptr);

private:
  void createGui();
  int make_command(char *buf);
  void setEditable(bool);

  andor2k::ClientSocket **csock;
  char buffer[1024];

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
  QLineEdit *m_obj_name;
  QLineEdit *m_filter_name;

  QLineEdit *m_server_info;
  QLineEdit *m_server_status;
  QLineEdit *m_server_time;
  QLineEdit *m_image_nr;
  QProgressBar *m_prog_bar;

  QHBoxLayout *main_layout;

  QLabel *m_label;

signals:

private slots:
  void set_exposure();
  void serverJobDone(const QString&);
  void serverJobUpdate(const QString &);
};

#endif // EXPOSURETAB_H
