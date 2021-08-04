#include "statustab.h"
#include <QApplication>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <cstring>

using andor2k::ClientSocket;
using andor2k::Socket;

StatusTab::StatusTab(ClientSocket *&csocket, char *sock_buffer, QWidget *parent)
    : QWidget(parent) {
  printf("[DEBUG][ANDOR2K::client::%15s] Constructing StatusTab\n", __func__);
  createGui();
  setLayout(m_layout);
  csock = &csocket;
  buffer = sock_buffer;
  // connect(m_set_button, SIGNAL(clicked()), this, SLOT(get_status()));

  printf("[DEBUG][ANDOR2K::client::%15s] StatusTab Socket at %p -> %p",
         __func__, &csock, csock);
  if (*csock)
    printf(" -> %p\n", *csock);
  else
    printf(" -> nowhere!\n");
  printf("[DEBUG][ANDOR2K::client::%15s] Finished constructing StatusTab\n",
         __func__);
}

void StatusTab::createGui() {

  m_status_string = new QLineEdit;
  m_temperature = new QLineEdit;
  m_last_update = new QLineEdit;

  m_label = new QLabel;
  m_label->setFrameStyle(QFrame::Box | QFrame::Plain);

  m_layout = new QGridLayout;
  m_layout->addWidget(new QLabel(tr("Current Temperature (C):")), 0, 0);
  m_layout->addWidget(m_temperature, 0, 1);
  m_layout->addWidget(new QLabel(tr("Status String:")), 1, 0);
  m_layout->addWidget(m_status_string, 1, 1);
  m_layout->addWidget(new QLabel(tr("Last Status Update:")), 2, 0);
  m_layout->addWidget(m_last_update, 2, 1);
  m_layout->setSizeConstraint(QLayout::SetFixedSize);
}