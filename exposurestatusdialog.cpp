#include "exposurestatusdialog.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <cstring>

using andor2k::ClientSocket;
using andor2k::Socket;

ExposureStatusDialog::ExposureStatusDialog(andor2k::ClientSocket *socket, int nimages, QWidget *parent)
    : QDialog(parent) {

  printf("[DEBUG][ANDOR2K::client::%15s] Constructing ExposureStatusDialog\n", __func__);

  csock = socket;
  num_images = nimages;

  t_edit = new QLineEdit;
  t_edit->setText("some temperature ...");
  t_edit->setReadOnly(true);
  
  t_count = new QLineEdit;
  t_count->setReadOnly(true);
  
  t_info = new QLineEdit;
  t_info->setReadOnly(true);

  QGroupBox *server_sgbx = new QGroupBox(tr("ANDOR2K Info:"));
  QGridLayout *sgbx_layout = new QGridLayout;
  sgbx_layout->addWidget(new QLabel(tr("Last Acquired Temperature in degC:")), 0,0);
  sgbx_layout->addWidget(t_edit, 0,1);
  sgbx_layout->addWidget(new QLabel(tr("Exposures Acquired:")), 1,0);
  sgbx_layout->addWidget(t_count, 1,1);
  sgbx_layout->addWidget(new QLabel(tr("Server Info:")), 2,0);
  sgbx_layout->addWidget(t_info, 2,1);
  server_sgbx->setLayout(sgbx_layout);
  
  m_layout = new QVBoxLayout;
  m_layout->addWidget(server_sgbx);

  setLayout(m_layout);
  
  setWindowTitle(tr("Exposure Status"));

  printf("[DEBUG][ANDOR2K::client::%15s] ExposureStatusDialog Socket at %p", __func__,
         &csock);
  if (csock)
    printf(" -> %p\n", csock);
  else
    printf(" -> nowhere!\n");
  printf("[DEBUG][ANDOR2K::client::%15s] Finished constructing ExposureStatusDialog\n",
         __func__);

  // keep on reading from socket, untill the string "done" is returned
  
  bool keep_working = true;
  while (keep_working) {
    std::memset(buffer, 0, 1024);
    if ( csock->recv(buffer, 1024) < 0 ) {
      t_info->setText("Communication error");
    } else {
      t_info->setText(buffer);
    }
    if (!(*buffer) || !std::strncmp(buffer, "done", 4)) {
      keep_working = false;
      printf("server signaled work done!\n");
    }
  }
  return;
}
