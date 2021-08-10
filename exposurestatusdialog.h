#ifndef EXPOSURESTATUSDIALOG_H
#define EXPOSURESTATUSDIALOG_H

#include "helmos-andor2k/cpp_socket.hpp"
#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

class ExposureStatusDialog : public QDialog {
  Q_OBJECT
public:
  explicit ExposureStatusDialog(andor2k::ClientSocket *sock_ptr, int num_images,
                                QWidget *parent = nullptr);

private:
  int num_images;
  char buffer[1024];
  andor2k::ClientSocket *csock;

  QLineEdit *t_edit;
  QLineEdit *t_count;
  QLineEdit *t_info;
  QVBoxLayout *m_layout;
};

#endif // EXPOSURESTATUSDIALOG_H
